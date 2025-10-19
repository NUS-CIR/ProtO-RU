#pragma once

#include <atomic>
#include <chrono>
#include <tuple>
#include "srsran/ran/slot_point.h"

/// Difference between Unix seconds to GPS seconds.
/// GPS epoch: 1980.1.6 00:00:00 (UTC); Unix time epoch: 1970:1.1 00:00:00 UTC
/// Last leap second added in 31st Dec 2016 by IERS.
/// 1970:1.1 - 1980.1.6: 3657 days
/// 3657*24*3600=315 964 800 seconds (Unix seconds value at 1980.1.6 00:00:00 (UTC))
/// There are 18 leap seconds inserted after 1980.1.6 00:00:00 (UTC), which means GPS is 18 seconds larger.
static constexpr uint64_t UNIX_TO_GPS_SECONDS_OFFSET = 315964800ULL - 18ULL;

/// Offset for converting from UTC to GPS time including Alpha and Beta parameters.
static std::chrono::nanoseconds gps_offset = std::chrono::seconds(UNIX_TO_GPS_SECONDS_OFFSET);;
using namespace srsran;

namespace {

  /// A GPS clock implementation.
  struct gps_clock {
    using duration   = std::chrono::nanoseconds;
    using rep        = duration::rep;
    using period     = duration::period;
    using time_point = std::chrono::time_point<gps_clock>;
    // static constexpr bool is_steady = false;
  
    static time_point now()
    {
      ::timespec ts;
      ::clock_gettime(CLOCK_REALTIME, &ts);
  
      time_point now(std::chrono::seconds(ts.tv_sec) + std::chrono::nanoseconds(ts.tv_nsec));
  
      return now - gps_offset;
    }

    static uint64_t to_uint64(const gps_clock::time_point& tp) {
      // calculate the duration from GPS epoch to tp(nanoseconds.
      auto dur = tp.time_since_epoch();  // dur is gps_clock::duratio, which is nanoseconds
    
      // extract integer value from lower layer（nanoseconds）
      gps_clock::rep ns = dur.count();   // rep is duration::rep，usually int64_t
  
      return static_cast<uint64_t>(ns);
    }

    static time_point from_uint64(uint64_t ns_since_gps) {
      return time_point(duration(ns_since_gps));
    }

    // 返回：年, 月, 日, 时, 分, 秒, 毫秒
    static std::tuple<int,int,int,int,int,int,int> to_ymd_hms_ms(uint64_t ns_since_gps) {
      // 重建 GPS time_point
      time_point tp_gps = from_uint64(ns_since_gps);
      // 转成 system_clock time_point
      auto tp_sys = std::chrono::time_point<std::chrono::system_clock, duration>(
                        tp_gps.time_since_epoch() + gps_offset
                    );
      // 拆分到毫秒
      auto tp_ms  = std::chrono::time_point_cast<std::chrono::milliseconds>(tp_sys);
      auto tp_sec = std::chrono::time_point_cast<std::chrono::seconds>(tp_sys);
      // 毫秒部分 = ms_since_epoch % 1000
      int ms = static_cast<int>(
          (tp_ms.time_since_epoch() - std::chrono::duration_cast<std::chrono::milliseconds>(tp_sec.time_since_epoch()))
          .count()
      );
      // 转成 time_t 秒
      std::time_t tt = tp_sec.time_since_epoch().count();
      std::tm gmt = *std::gmtime(&tt);

      int year   = gmt.tm_year + 1900;
      int month  = gmt.tm_mon  + 1;
      int day    = gmt.tm_mday;
      int hour   = gmt.tm_hour;
      int minute = gmt.tm_min;
      int second = gmt.tm_sec;

      return {year, month, day, hour, minute, second, ms};
    }
    
    /// Calculates the fractional part inside a second from the given time point.
    static std::chrono::nanoseconds calculate_ns_fraction_from(gps_clock::time_point tp)
    {
      auto tp_sec = std::chrono::time_point_cast<std::chrono::seconds>(tp);
      return tp - tp_sec;
    }

    /// Calculates the current slot point given the GPS time.
    static slot_point
    calculate_slot_point(subcarrier_spacing scs, uint64_t gps_seconds, uint32_t fractional_us, uint32_t slot_duration_us)
    {
      unsigned subframe_index      = (fractional_us / (SUBFRAME_DURATION_MSEC * 1000)) % NOF_SUBFRAMES_PER_FRAME;
      unsigned slot_subframe_index = (fractional_us / slot_duration_us) % get_nof_slots_per_subframe(scs);

      static constexpr unsigned NUM_FRAMES_PER_SEC = 100;
      static constexpr unsigned FRAME_DURATION_US  = SUBFRAME_DURATION_MSEC * NOF_SUBFRAMES_PER_FRAME * 1000;
      unsigned                  sfn = (gps_seconds * NUM_FRAMES_PER_SEC + (fractional_us / FRAME_DURATION_US)) % NOF_SFNS;

      return {to_numerology_value(scs), sfn, subframe_index, slot_subframe_index};
    }
    /// Calculates the current ofh slot point now.
    static slot_point get_ofh_slot_now(subcarrier_spacing scs)
    {
      auto now = gps_clock::now();
      auto ns_fraction = gps_clock::calculate_ns_fraction_from(now);
      slot_point gps_slot = gps_clock::calculate_slot_point(scs,
                        std::chrono::time_point_cast<std::chrono::seconds>(now).time_since_epoch().count(),
                        std::chrono::duration_cast<std::chrono::microseconds>(ns_fraction).count(),
                        1000 / get_nof_slots_per_subframe(scs));
      slot_point ofh_slot(gps_slot.numerology(), gps_slot.sfn()% NOF_OFH_SFNS, gps_slot.slot_index());
      return ofh_slot;
    }

    /// Returns the symbol index inside a second.
    static unsigned get_symbol_index(std::chrono::nanoseconds                 fractional_ns,
                                    std::chrono::duration<double, std::nano> symbol_duration)
    {
      // Perform operation with enough precision to avoid rounding errors when the amount of fractional nanoseconds is big.
      return fractional_ns / symbol_duration;
    }
  };
  
} // namespace