// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "phy_dummy_fapi_p7_sector_adaptor.h"
#include "ocudu/fapi/common/error_indication_builder.h"
#include "ocudu/fapi/common/error_indication_notifier.h"
#include "ocudu/fapi/p7/builders/crc_indication_builder.h"
#include "ocudu/fapi/p7/builders/rx_data_indication_builder.h"
#include "ocudu/fapi/p7/builders/slot_indication_builder.h"
#include "ocudu/fapi/p7/builders/srs_indication_builder.h"
#include "ocudu/fapi/p7/builders/uci_indication_builder.h"
#include "ocudu/fapi/p7/messages/dl_tti_request.h"
#include "ocudu/fapi/p7/messages/tx_data_request.h"
#include "ocudu/fapi/p7/messages/ul_dci_request.h"
#include "ocudu/fapi/p7/messages/ul_tti_request.h"
#include "ocudu/fapi/p7/p7_indications_notifier.h"
#include "ocudu/fapi/p7/p7_slot_indication_notifier.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/sch/sch_constants.h"
#include "ocudu/ran/srs/srs_constants.h"
#include "ocudu/support/ocudu_assert.h"
#include <algorithm>
#include <utility>

using namespace ocudu;
using namespace fapi_adaptor;

namespace {

class error_indication_notifier_dummy : public fapi::error_indication_notifier
{
public:
  void on_error_indication(const fapi::error_indication& msg) override {}
};

class p7_slot_indication_notifier_dummy : public fapi::p7_slot_indication_notifier
{
public:
  void on_slot_indication(const fapi::slot_indication& msg) override {}
};

class p7_indications_notifier_dummy : public fapi::p7_indications_notifier
{
public:
  void on_rx_data_indication(const fapi::rx_data_indication& msg) override {}
  void on_crc_indication(const fapi::crc_indication& msg) override {}
  void on_uci_indication(const fapi::uci_indication& msg) override {}
  void on_srs_indication(const fapi::srs_indication& msg) override {}
  void on_rach_indication(const fapi::rach_indication& msg) override {}
};

} // namespace

static error_indication_notifier_dummy   dummy_error_notifier;
static p7_slot_indication_notifier_dummy dummy_slot_notifier;
static p7_indications_notifier_dummy     dummy_p7_notifier;

/// Uplink SINR reported in all the indications.
static constexpr float ul_sinr_dB = 30.0F;

/// MAC subheader of a padding subPDU, as per TS 38.321, Section 6.2.1.
static constexpr uint8_t padding_subheader = 0x3f;

/// Returns the minimum number of consecutive slots the per-slot tables hold.
static unsigned get_min_nof_tracked_slots(unsigned ul_indication_delay_slots, unsigned max_processing_delay_slots)
{
  // Margin expressed in number of slots for requests arriving around the slot boundary.
  static constexpr unsigned margin = 4;

  return ul_indication_delay_slots + max_processing_delay_slots + margin;
}

phy_dummy_fapi_p7_sector_adaptor::phy_dummy_fapi_p7_sector_adaptor(const phy_dummy_fapi_sector_adaptor_config& config,
                                                                   ocudulog::basic_logger&                     logger_,
                                                                   phy_dummy_fapi_sector_executor& executor_) :
  sector_id(config.sector_id),
  scs(config.scs),
  ul_indication_delay_slots(config.ul_indication_delay_slots),
  max_processing_delay_slots(config.max_processing_delay_slots),
  late_request_detection(config.late_request_detection),
  logger(logger_),
  executor(executor_),
  // The tables round their size up to a power of two, which divides the number of slots of a slot point period, so
  // slots map to entries without collisions when the slot count wraps around.
  completed_slots(get_min_nof_tracked_slots(config.ul_indication_delay_slots, config.max_processing_delay_slots)),
  late_slots(completed_slots.size()),
  ul_repository(completed_slots.size()),
  padding_pdu(tbs_lbrm_default.value(), padding_subheader),
  error_notifier(&dummy_error_notifier),
  slot_notifier(&dummy_slot_notifier),
  p7_notifier(&dummy_p7_notifier)
{
  ocudu_assert(ul_indication_delay_slots > 0, "The uplink indication delay must be at least one slot");
}

void phy_dummy_fapi_p7_sector_adaptor::handle_slot_boundary(slot_point_extended                   slot,
                                                            std::chrono::system_clock::time_point time_point)
{
  executor.defer("slot boundary", [this, slot, time_point]() { process_slot_boundary(slot, time_point); });
}

phy_dummy_fapi_p7_sector_adaptor::slot_boundary_progress phy_dummy_fapi_p7_sector_adaptor::get_progress() const
{
  uint32_t value = progress.load(std::memory_order_acquire);
  if (value == invalid_progress) {
    return {};
  }
  return {.slot = slot_point(scs, value >> 1U), .running = (value & 1U) != 0};
}

void phy_dummy_fapi_p7_sector_adaptor::publish_progress(slot_point slot)
{
  progress.store((slot.count() << 1U) | (running && mac_completed_slots ? 1U : 0U), std::memory_order_release);
}

void phy_dummy_fapi_p7_sector_adaptor::start(unsigned nof_rx_antennas_)
{
  nof_rx_antennas     = nof_rx_antennas_;
  running             = true;
  mac_completed_slots = false;
  current_slot.reset();
  std::fill(completed_slots.begin(), completed_slots.end(), std::nullopt);
  std::fill(late_slots.begin(), late_slots.end(), std::nullopt);
}

void phy_dummy_fapi_p7_sector_adaptor::request_stop(phy_dummy_fapi_p7_stop_notifier& notifier)
{
  // The sector stops at the next slot boundary, so that the requests of the last notified slot complete.
  stop_notifier = &notifier;
}

void phy_dummy_fapi_p7_sector_adaptor::process_slot_boundary(slot_point_extended                   slot,
                                                             std::chrono::system_clock::time_point time_point)
{
  slot_point notified_slot = slot.without_hyper_sfn();

  if (running && stop_notifier != nullptr) {
    running = false;
    publish_progress(notified_slot);
    std::exchange(stop_notifier, nullptr)->on_p7_stopped();
    return;
  }

  if (!running) {
    publish_progress(notified_slot);
    return;
  }

  current_slot = notified_slot;

  // The MAC discards the slot indications that precede the activation of the cell, so the clock does not wait for
  // their last requests.
  if (!mac_completed_slots) {
    publish_progress(notified_slot);
  }

  // The notified slot is ahead of the current time by the processing delay, and the uplink indications are delayed with
  // respect to the reception time. Only slots whose requests completed in time hold valid uplink contexts.
  slot_point ul_slot = notified_slot - static_cast<int>(max_processing_delay_slots + ul_indication_delay_slots);
  std::optional<slot_point>& completed_slot = completed_slots[ul_slot.count()];
  if (completed_slot == ul_slot) {
    if (auto context = ul_repository.read(ul_slot.count())) {
      notify_ul_indications(ul_slot, *context);
    }
  }
  // The completed and late flags of a slot are consumed once, so a later slot mapped to the same entry cannot match
  // them.
  completed_slot.reset();
  late_slots[ul_slot.count()].reset();

  // Requests of a slot are only valid after its SLOT.indication, so older contexts stored for it are discarded.
  ul_repository.clear(notified_slot.count());
  slot_notifier->on_slot_indication(fapi::build_slot_indication(slot, time_point));
}

void phy_dummy_fapi_p7_sector_adaptor::process_last_message(slot_point slot)
{
  if (!running) {
    return;
  }

  // The uplink contexts of a slot with late requests are discarded.
  if (late_slots[slot.count()] != slot) {
    completed_slots[slot.count()] = slot;
  }
  mac_completed_slots = true;
  publish_progress(slot);
}

bool phy_dummy_fapi_p7_sector_adaptor::is_late(slot_point slot) const
{
  // The current time lags the last notified slot by the processing delay.
  return current_slot.has_value() && slot < *current_slot - static_cast<int>(max_processing_delay_slots);
}

void phy_dummy_fapi_p7_sector_adaptor::check_request(slot_point slot, fapi::message_type_id msg_id)
{
  if (late_request_detection) {
    executor.defer("request check", [this, slot, msg_id]() { process_request(slot, msg_id); });
  }
}

void phy_dummy_fapi_p7_sector_adaptor::process_request(slot_point slot, fapi::message_type_id msg_id)
{
  if (!running || !is_late(slot)) {
    return;
  }

  logger.warning(
      "Sector#{}: Real-time failure in PHY dummy FAPI adaptor: Received late request with msg_id={} for slot {}",
      sector_id,
      fmt::underlying(msg_id),
      slot);

  // The uplink contexts of a slot with late requests are discarded.
  late_slots[slot.count()] = slot;

  // As in the PHY, every late request is reported, so that the MAC discards all the affected channels.
  bool is_tti_request =
      msg_id == fapi::message_type_id::dl_tti_request || msg_id == fapi::message_type_id::ul_tti_request;
  error_notifier->on_error_indication(is_tti_request
                                          ? fapi::build_out_of_sync_error_indication(slot, msg_id, *current_slot)
                                          : fapi::build_invalid_sfn_error_indication(slot, msg_id, *current_slot));
}

void phy_dummy_fapi_p7_sector_adaptor::send_dl_tti_request(const fapi::dl_tti_request& msg)
{
  check_request(msg.slot, fapi::message_type_id::dl_tti_request);
}

/// Saves the indication contexts of a PUSCH PDU.
static void save_pusch_context(ul_slot_ind_context& context, const fapi::ul_pusch_pdu& pdu)
{
  pusch_ind_context& pusch = context.puschs.emplace_back();
  pusch.rnti               = pdu.rnti;
  pusch.harq_id            = pdu.harq_id;
  pusch.rapid              = pdu.rapid;
  pusch.tb_size            = pdu.pusch_data.has_value() ? std::optional(pdu.pusch_data->tb_size) : std::nullopt;
  pusch.has_uci            = pdu.pusch_uci.has_value();
  pusch.harq_ack_bits      = pusch.has_uci ? pdu.pusch_uci->harq_ack_bit : units::bits(0);
  pusch.csi_part1_bits     = pusch.has_uci ? pdu.pusch_uci->csi_part1_bit : units::bits(0);
}

/// Saves the indication contexts of a PUCCH PDU.
static void save_pucch_context(ul_slot_ind_context& context, const fapi::ul_pucch_pdu& pdu)
{
  pucch_ind_context pucch{.rnti           = pdu.rnti,
                          .format         = pucch_format::FORMAT_0,
                          .sr_bits        = units::bits(0),
                          .harq_ack_bits  = units::bits(0),
                          .csi_part1_bits = units::bits(0)};

  if (const auto* f0 = std::get_if<fapi::ul_pucch_pdu_format_0>(&pdu.format)) {
    pucch.sr_bits       = units::bits(f0->sr_present ? 1 : 0);
    pucch.harq_ack_bits = f0->bit_len_harq;
  } else if (const auto* f1 = std::get_if<fapi::ul_pucch_pdu_format_1>(&pdu.format)) {
    pucch.format        = pucch_format::FORMAT_1;
    pucch.sr_bits       = units::bits(f1->sr_present ? 1 : 0);
    pucch.harq_ack_bits = f1->bit_len_harq;
  } else if (const auto* f2 = std::get_if<fapi::ul_pucch_pdu_format_2>(&pdu.format)) {
    pucch.format         = pucch_format::FORMAT_2;
    pucch.sr_bits        = units::bits(static_cast<unsigned>(f2->sr_bit_len));
    pucch.harq_ack_bits  = f2->bit_len_harq;
    pucch.csi_part1_bits = f2->csi_part1_bit_length;
  } else if (const auto* f3 = std::get_if<fapi::ul_pucch_pdu_format_3>(&pdu.format)) {
    pucch.format         = pucch_format::FORMAT_3;
    pucch.sr_bits        = units::bits(static_cast<unsigned>(f3->sr_bit_len));
    pucch.harq_ack_bits  = f3->bit_len_harq;
    pucch.csi_part1_bits = f3->csi_part1_bit_length;
  } else if (const auto* f4 = std::get_if<fapi::ul_pucch_pdu_format_4>(&pdu.format)) {
    pucch.format         = pucch_format::FORMAT_4;
    pucch.sr_bits        = units::bits(static_cast<unsigned>(f4->sr_bit_len));
    pucch.harq_ack_bits  = f4->bit_len_harq;
    pucch.csi_part1_bits = f4->csi_part1_bit_length;
  } else {
    return;
  }

  context.pucchs.push_back(pucch);
}

/// Saves the indication contexts of an SRS PDU.
static void save_srs_context(ul_slot_ind_context& context, const fapi::ul_srs_pdu& pdu)
{
  if (!pdu.enable_normalized_iq_matrix_report && !pdu.enable_positioning_report) {
    return;
  }

  context.srss.push_back({.rnti               = pdu.rnti,
                          .nof_tx_ports       = static_cast<uint8_t>(pdu.num_ant_ports),
                          .iq_matrix_report   = pdu.enable_normalized_iq_matrix_report,
                          .positioning_report = pdu.enable_positioning_report});
}

void phy_dummy_fapi_p7_sector_adaptor::send_ul_tti_request(const fapi::ul_tti_request& msg)
{
  check_request(msg.slot, fapi::message_type_id::ul_tti_request);

  auto context = ul_repository.write(msg.slot.count());
  if (context == nullptr) {
    logger.warning("Sector#{}: PHY dummy FAPI adaptor dropped UL_TTI.request for slot {}. Cause: slot entry is busy",
                   sector_id,
                   msg.slot);
    return;
  }

  context->clear();
  for (const auto& pdu : msg.pdus) {
    if (const auto* pusch = std::get_if<fapi::ul_pusch_pdu>(&pdu.pdu)) {
      save_pusch_context(*context, *pusch);
    } else if (const auto* pucch = std::get_if<fapi::ul_pucch_pdu>(&pdu.pdu)) {
      save_pucch_context(*context, *pucch);
    } else if (const auto* srs = std::get_if<fapi::ul_srs_pdu>(&pdu.pdu)) {
      save_srs_context(*context, *srs);
    }
  }
}

void phy_dummy_fapi_p7_sector_adaptor::send_ul_dci_request(const fapi::ul_dci_request& msg)
{
  check_request(msg.slot, fapi::message_type_id::ul_dci_request);
}

void phy_dummy_fapi_p7_sector_adaptor::send_tx_data_request(const fapi::tx_data_request& msg)
{
  check_request(msg.slot, fapi::message_type_id::tx_data_request);
}

void phy_dummy_fapi_p7_sector_adaptor::on_last_message(slot_point slot)
{
  executor.defer("last message", [this, slot]() { process_last_message(slot); });
}

void phy_dummy_fapi_p7_sector_adaptor::notify_ul_indications(slot_point slot, const ul_slot_ind_context& context)
{
  for (const pusch_ind_context& pusch : context.puschs) {
    notify_pusch_indications(slot, pusch);
  }
  for (const pucch_ind_context& pucch : context.pucchs) {
    notify_pucch_indication(slot, pucch);
  }
  for (const srs_ind_context& srs : context.srss) {
    notify_srs_indication(slot, srs);
  }
}

/// Returns the detection status of a successfully decoded UCI with the given total number of bits.
static uci_pusch_or_pucch_f2_3_4_detection_status get_uci_detection_status(units::bits nof_uci_bits)
{
  // UCI longer than this number of bits is protected with a CRC, as per TS 38.212, Section 6.3.1.2.1.
  static constexpr units::bits max_nof_bits_without_crc(11);
  return (nof_uci_bits <= max_nof_bits_without_crc) ? uci_pusch_or_pucch_f2_3_4_detection_status::no_dtx
                                                    : uci_pusch_or_pucch_f2_3_4_detection_status::crc_pass;
}

/// Returns a HARQ-ACK payload with all bits set to ACK.
static uci_payload_type get_harq_ack_payload(units::bits nof_bits)
{
  uci_payload_type payload(nof_bits.value());
  payload.fill(0, nof_bits.value(), true);
  return payload;
}

void phy_dummy_fapi_p7_sector_adaptor::notify_pusch_indications(slot_point slot, const pusch_ind_context& pusch)
{
  static constexpr phy_time_unit time_advance = phy_time_unit::from_seconds(0.0);

  if (pusch.has_uci) {
    fapi::uci_indication         msg;
    fapi::uci_indication_builder builder(msg);
    builder.set_slot(slot);

    fapi::uci_pusch_pdu_builder builder_pdu = builder.add_pusch_pdu();
    builder_pdu.set_ue_specific_parameters(pusch.rnti);
    builder_pdu.set_metrics_parameters(ul_sinr_dB, time_advance, std::nullopt, std::nullopt);

    uci_pusch_or_pucch_f2_3_4_detection_status status =
        get_uci_detection_status(pusch.harq_ack_bits + pusch.csi_part1_bits);
    if (pusch.harq_ack_bits.value() > 0) {
      builder_pdu.set_harq_parameters(status, pusch.harq_ack_bits, get_harq_ack_payload(pusch.harq_ack_bits));
    }
    if (pusch.csi_part1_bits.value() > 0) {
      builder_pdu.set_csi_part1_parameters(
          status, pusch.csi_part1_bits, uci_payload_type(pusch.csi_part1_bits.value()));
    }

    p7_notifier->on_uci_indication(msg);
  }

  if (!pusch.tb_size.has_value()) {
    return;
  }

  fapi::crc_indication         crc_msg;
  fapi::crc_indication_builder crc_builder(crc_msg);
  crc_builder.set_slot(slot);
  crc_builder.set_pdu(pusch.rnti, pusch.harq_id, true, ul_sinr_dB, time_advance, std::nullopt, std::nullopt);
  if (pusch.rapid.has_value()) {
    crc_builder.set_rapid_parameter(*pusch.rapid);
  }
  p7_notifier->on_crc_indication(crc_msg);

  if (pusch.tb_size->value() > padding_pdu.size()) {
    logger.warning("Sector#{}: PHY dummy FAPI adaptor truncated the transport block of rnti={} in slot {} to {} bytes",
                   sector_id,
                   pusch.rnti,
                   slot,
                   padding_pdu.size());
  }

  fapi::rx_data_indication         rx_msg;
  fapi::rx_data_indication_builder rx_builder(rx_msg);
  rx_builder.set_slot(slot);
  rx_builder.set_pdu(
      pusch.rnti,
      pusch.harq_id,
      span<const uint8_t>(padding_pdu).first(std::min<size_t>(pusch.tb_size->value(), padding_pdu.size())));
  if (pusch.rapid.has_value()) {
    rx_builder.set_rapid_parameter(*pusch.rapid);
  }
  p7_notifier->on_rx_data_indication(rx_msg);
}

void phy_dummy_fapi_p7_sector_adaptor::notify_pucch_indication(slot_point slot, const pucch_ind_context& pucch)
{
  static constexpr phy_time_unit time_advance = phy_time_unit::from_seconds(0.0);

  fapi::uci_indication         msg;
  fapi::uci_indication_builder builder(msg);
  builder.set_slot(slot);

  if (pucch.format == pucch_format::FORMAT_0 || pucch.format == pucch_format::FORMAT_1) {
    fapi::uci_pucch_pdu_format_0_1_builder builder_pdu = builder.add_format_0_1_pucch_pdu();
    builder_pdu.set_ue_specific_parameters(pucch.rnti).set_format(pucch.format);
    builder_pdu.set_time_advance(time_advance).set_metrics_parameters(ul_sinr_dB, std::nullopt, std::nullopt);
    if (pucch.sr_bits.value() > 0) {
      builder_pdu.set_sr_parameters(false);
    }
    if (pucch.harq_ack_bits.value() > 0) {
      static_vector<uci_pucch_f0_or_f1_harq_values, fapi::uci_harq_format_0_1::MAX_NUM_HARQ> harq(
          std::min<size_t>(pucch.harq_ack_bits.value(), fapi::uci_harq_format_0_1::MAX_NUM_HARQ),
          uci_pucch_f0_or_f1_harq_values::ack);
      builder_pdu.set_harq_parameters(harq);
    }
    p7_notifier->on_uci_indication(msg);
    return;
  }

  fapi::uci_pucch_pdu_format_2_3_4_builder builder_pdu = builder.add_format_2_3_4_pucch_pdu();
  builder_pdu.set_ue_specific_parameters(pucch.rnti).set_format(pucch.format);
  builder_pdu.set_metrics_parameters(ul_sinr_dB, time_advance, std::nullopt, std::nullopt);

  uci_pusch_or_pucch_f2_3_4_detection_status status =
      get_uci_detection_status(pucch.sr_bits + pucch.harq_ack_bits + pucch.csi_part1_bits);
  if (pucch.sr_bits.value() > 0) {
    builder_pdu.set_sr_parameters(
        bounded_bitset<fapi::sr_pdu_format_2_3_4::MAX_SR_PAYLOAD_SIZE_BITS>(pucch.sr_bits.value()));
  }
  if (pucch.harq_ack_bits.value() > 0) {
    builder_pdu.set_harq_parameters(status, pucch.harq_ack_bits, get_harq_ack_payload(pucch.harq_ack_bits));
  }
  if (pucch.csi_part1_bits.value() > 0) {
    builder_pdu.set_csi_part1_parameters(status, pucch.csi_part1_bits, uci_payload_type(pucch.csi_part1_bits.value()));
  }
  p7_notifier->on_uci_indication(msg);
}

void phy_dummy_fapi_p7_sector_adaptor::notify_srs_indication(slot_point slot, const srs_ind_context& srs)
{
  static constexpr phy_time_unit time_advance = phy_time_unit::from_seconds(0.0);

  fapi::srs_indication         msg;
  fapi::srs_indication_builder builder(msg);
  builder.set_slot(slot);

  fapi::srs_indication_pdu_builder builder_pdu = builder.set_pdu(srs.rnti);
  builder_pdu.set_metrics_parameters(time_advance);

  if (srs.iq_matrix_report) {
    unsigned nof_tx_ports =
        std::clamp(static_cast<unsigned>(srs.nof_tx_ports), 1U, static_cast<unsigned>(srs_constants::max_nof_tx_ports));
    srs_channel_matrix matrix(nof_rx_antennas, nof_tx_ports);
    for (unsigned i_port = 0, e = std::min(nof_rx_antennas, nof_tx_ports); i_port != e; ++i_port) {
      matrix.set_coefficient(cf_t(1.0F, 0.0F), i_port, i_port);
    }
    builder_pdu.set_codebook_report_matrix(matrix);
  }

  if (srs.positioning_report) {
    builder_pdu.set_positioning_report_parameters(time_advance, std::nullopt, std::nullopt, std::nullopt);
  }

  p7_notifier->on_srs_indication(msg);
}
