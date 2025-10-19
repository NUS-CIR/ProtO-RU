
#pragma once

#include "srsran/ofh/ethernet/ethernet_frame_builder.h"
#include "srsran/ofh/ofh_constants.h"
#include "srsran/ofh/serdes/ofh_cplane_message_builder.h"
#include "srsran/ofh/serdes/ofh_uplane_message_builder.h"
#include "srsran/ofh/transmitter/ofh_transmitter_timing_parameters.h"
#include "srsran/ran/bs_channel_bandwidth.h"
#include "srsran/ran/cyclic_prefix.h"
#include "srsran/ran/tdd/tdd_ul_dl_config.h"

namespace srsran {
namespace ofh {

/// Open Fronthaul transmitter configuration.
struct uplink_data_flow_config {
  /// Radio sector identifier.
  unsigned sector;
  /// Channel bandwidth.
  bs_channel_bandwidth bw;
  /// Subcarrier spacing.
  subcarrier_spacing scs;
  /// Cyclic prefix.
  cyclic_prefix cp;
  /// Uplink eAxC.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
  /// PRACH eAxC.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> prach_eaxc;
  /// Destination MAC address.
  ether::mac_address mac_dst_address;
  /// Source MAC address.
  ether::mac_address mac_src_address;
  /// MTU size.
  units::bytes mtu_size;
  /// Tag control information field for C-Plane.
  std::optional<uint16_t> tci_cp;
  /// Tag control information field for U-Plane.
  std::optional<uint16_t> tci_up;
  /// RU working bandwidth.
  bs_channel_bandwidth ru_working_bw;
  /// Uplink compression parameters.
  ru_compression_params ul_compr_params;
  /// PRACH compression parameters.
  //ru_compression_params prach_compr_params;
  /// Uplink static compression header flag.
  bool is_uplink_static_compr_hdr_enabled;
  /// IQ samples scaling factor.
  float iq_scaling;
};

} // namespace ofh
} // namespace srsran
