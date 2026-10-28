// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cu_up_unit_config_validator.h"
#include "apps/helpers/xnu/xnu_appconfig_validator.h"
#include "cu_up_unit_config.h"
#include "ocudu/adt/span.h"
#include "ocudu/ran/plmn_identity.h"
#include <algorithm>

using namespace ocudu;

/// Validates the given QoS configuration. Returns true on success, otherwise false.
static bool validate_qos_appconfig(span<const cu_up_unit_qos_config> config)
{
  for (const auto& qos : config) {
    if (qos.mode != "am" && qos.mode != "um-bidir") {
      fmt::println("RLC mode is neither \"am\" or \"um-bidir\". {} mode={}", qos.five_qi, qos.mode);
      return false;
    }
  }
  return true;
}

static bool validate_cu_up_trace_appconfig(const cu_up_unit_trace_config& exec_cfg, bool tracing_enabled)
{
  if (exec_cfg.cu_up_enable && !tracing_enabled) {
    fmt::println("Tracing requested for CU-UP executors, but tracing is disabled");
    return false;
  }
  return true;
}

static bool validate_cu_up_test_mode_appconfig(const cu_up_unit_test_mode_config& config)
{
  if (config.attach_detach_period.count() != 0 && config.reestablish_period.count() != 0) {
    fmt::println("Cannot run attach-detach tests simultaniously with re-establish tests");
    return false;
  }
  return true;
}

/// Validates the given NG-U socket configuration. Returns true on success, otherwise false.
static bool validate_ngu_sockets_appconfig(span<const cu_up_unit_ngu_socket_config> config)
{
  return std::all_of(config.begin(), config.end(), [](const cu_up_unit_ngu_socket_config& sock) {
    if (sock.sd.has_value() and not sock.sst.has_value()) {
      fmt::println("NG-U socket has an SD, but no SST. Configure the SST of the S-NSSAI too. bind_addr={} sd={:#x}",
                   sock.bind_addr,
                   *sock.sd);
      return false;
    }
    return true;
  });
}

static bool validate_plmn_list(span<const std::string> plmns)
{
  if (plmns.empty()) {
    fmt::println("CU-UP PLMN list must contain at least one entry");
    return false;
  }
  if (plmns.size() > 12) {
    fmt::println("CU-UP PLMN list exceeds maximum of 12 entries (maxnoofSPLMNs)");
    return false;
  }
  for (const auto& p : plmns) {
    if (!plmn_identity::parse(p).has_value()) {
      fmt::println("Invalid PLMN identity: {}", p);
      return false;
    }
  }
  return true;
}

bool ocudu::validate_cu_up_unit_config(const cu_up_unit_config& config, bool tracing_enabled)
{
  if (!validate_qos_appconfig(config.qos_cfg)) {
    return false;
  }

  if (!validate_cu_up_trace_appconfig(config.trace_cfg, tracing_enabled)) {
    return false;
  }

  if (!validate_cu_up_test_mode_appconfig(config.test_mode_cfg)) {
    return false;
  }

  if (!validate_plmn_list(config.plmn_list)) {
    return false;
  }

  if (!validate_xnu_sockets_appconfig(config.xnu_cfg.sockets_cfg)) {
    return false;
  }

  if (!validate_ngu_sockets_appconfig(config.ngu_cfg.ngu_socket_cfg)) {
    return false;
  }

  return true;
}
