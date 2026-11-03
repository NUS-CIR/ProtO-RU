// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "integrity_engine_nia2_non_cmac.h"
#include "ocudu/security/security.h"
#include "ocudu/security/ssl.h"

using namespace ocudu;
using namespace security;

integrity_engine_nia2_non_cmac::integrity_engine_nia2_non_cmac(sec_128_key        k_128_int_,
                                                               uint8_t            bearer_id_,
                                                               security_direction direction_,
                                                               bool               allow_unprotected_) :
  k_128_int(k_128_int_),
  bearer_id(bearer_id_),
  direction(direction_),
  logger(ocudulog::fetch_basic_logger("SEC")),
  allow_unprotected(allow_unprotected_)
{
  std::array<uint8_t, 16> l;

  int ret = crypto_init();
  if (ret != 0) {
    report_error("Failure in initializing crypto PSA");
    return;
  }
  ret = aes_setkey_enc(&ctx, k_128_int.data(), 128);
  if (ret != 0) {
    report_error("Failure in setting AES security key");
    return;
  }

  // subkey L generation
  aes_crypt_ecb(&ctx, aes_encrypt, zeros.data(), l.data());

  // subkey K1 generation
  for (uint32_t i = 0; i < 15; i++) {
    k1[i] = (l[i] << 1) | ((l[i + 1] >> 7) & 0x01);
  }
  k1[15] = l[15] << 1;
  if (l[0] & 0x80) {
    k1[15] ^= 0x87;
  }

  // subkey K2 generation
  for (uint32_t i = 0; i < 15; i++) {
    k2[i] = (k1[i] << 1) | ((k1[i + 1] >> 7) & 0x01);
  }
  k2[15] = k1[15] << 1;
  if (k1[0] & 0x80) {
    k2[15] ^= 0x87;
  }
}

integrity_engine_nia2_non_cmac::~integrity_engine_nia2_non_cmac() = default;

security_status
integrity_engine_nia2_non_cmac::compute_mac(security::sec_mac& mac, const byte_buffer_view v, uint32_t count)
{
  uint32_t len      = v.length();
  uint32_t len_bits = len * 8;

  std::array<uint8_t, 16> tmp_mac;
  std::array<uint8_t, 16> tmp;

  const uint32_t msg_len_block_8_with_padding = len + 8 + 16;
  ocudu_assert(msg_len_block_8_with_padding <= sec_max_pdu_size,
               "{}: Maximum PDU length exceeded. len={} max_len={}",
               __FUNCTION__,
               msg_len_block_8_with_padding,
               sec_max_pdu_size);

  span<uint8_t> msg(msg_buf.data(), msg_len_block_8_with_padding);

  // construct msg from preample, PDU and padding
  msg[0] = (count >> 24) & 0xff;
  msg[1] = (count >> 16) & 0xff;
  msg[2] = (count >> 8) & 0xff;
  msg[3] = count & 0xff;
  msg[4] = (bearer_id << 3) | (to_number(direction) << 2);
  std::fill(msg.begin() + 5, msg.begin() + 8, 0);
  std::copy(v.begin(), v.end(), msg.begin() + 8);
  std::fill(msg.begin() + len + 8, msg.end(), 0);

  // MAC-I generation
  const uint32_t n = std::ceil((float)(len + 8) / (float)(16));
  std::fill(tmp_mac.begin(), tmp_mac.end(), 0);
  for (uint32_t i = 0; i < (n - 1); i++) {
    for (uint32_t j = 0; j < 16; j++) {
      tmp[j] = tmp_mac[j] ^ msg[i * 16 + j];
    }
    aes_crypt_ecb(&ctx, aes_encrypt, tmp.data(), tmp_mac.data());
  }
  uint32_t pad_bits = ((len_bits) + 64) % 128;
  if (pad_bits == 0) {
    for (uint32_t j = 0; j < 16; j++) {
      tmp[j] = tmp_mac[j] ^ k1[j] ^ msg[(n - 1) * 16 + j];
    }
    aes_crypt_ecb(&ctx, aes_encrypt, tmp.data(), tmp_mac.data());
  } else {
    pad_bits = (128 - pad_bits) - 1;
    msg[(n - 1) * 16 + (15 - (pad_bits / 8))] |= 0x1 << (pad_bits % 8);
    for (uint32_t j = 0; j < 16; j++) {
      tmp[j] = tmp_mac[j] ^ k2[j] ^ msg[(n - 1) * 16 + j];
    }
    aes_crypt_ecb(&ctx, aes_encrypt, tmp.data(), tmp_mac.data());
  }

  // copy first 4 bytes
  std::copy(tmp_mac.begin(), tmp_mac.begin() + 4, mac.begin());

  return security_status::success;
}

security_status integrity_engine_nia2_non_cmac::protect_integrity(byte_buffer& buf, uint32_t count)
{
  byte_buffer_view v{buf.begin(), buf.end()};

  logger.debug("Applying integrity protection. count={}", count);
  logger.debug(v.begin(), v.end(), "Message input:");

  security::sec_mac mac    = {};
  security_status   status = compute_mac(mac, v, count);

  if (status != security_status::success) {
    return status;
  }

  if (not buf.append(mac)) {
    return security_status::buffer_failure;
  }

  logger.debug("K_int: {}", k_128_int);
  logger.debug("MAC-I: {}", mac);
  logger.debug(buf.begin(), buf.end(), "Message output:");

  return security_status::success;
}

security_status integrity_engine_nia2_non_cmac::verify_integrity(byte_buffer& buf, uint32_t count)
{
  if (buf.length() <= sec_mac_len) {
    return security_status::integrity_failure;
  }

  byte_buffer_view v{buf, 0, buf.length() - sec_mac_len};
  byte_buffer_view m_rx{buf, buf.length() - sec_mac_len, sec_mac_len};

  // Compute MAC-I.
  security::sec_mac mac    = {};
  security_status   status = compute_mac(mac, v, count);

  if (status != security_status::success) {
    return status;
  }
  span m_exp{mac.data(), sec_mac_len};

  // Verify MAC-I.
  if (!std::equal(mac.begin(), mac.end(), m_rx.begin(), m_rx.end())) {
    if (allow_unprotected) {
      // Unprotected PDUs are expected to fail the integrity check but must have zero MAC-I.
      static constexpr security::sec_mac zero_mac = {};
      if (std::equal(zero_mac.begin(), zero_mac.end(), m_rx.begin(), m_rx.end())) {
        // Integrity passed (as unprotected).
        logger.debug("Integrity check passed as unprotected with zero MAC-I. count={}", count);
        logger.debug("K_int: {}", k_128_int);
        logger.debug(v.begin(), v.end(), "Message input:");

        // Trim MAC-I from PDU.
        buf.trim_tail(sec_mac_len);
        return security_status::success_unprotected;
      }
    }
    // Integrity failure.
    security::sec_mac mac_rx;
    std::copy(m_rx.begin(), m_rx.end(), mac_rx.begin());
    logger.warning("Integrity check failed. count={}", count);
    logger.warning("K_int: {}", k_128_int);
    logger.warning("MAC-I received: {}", mac_rx);
    logger.warning("MAC-I expected: {}", m_exp);
    logger.warning(v.begin(), v.end(), "Message input:");
    return security_status::integrity_failure;
  }
  // Integrity passed (as protected).
  logger.debug("Integrity check passed. count={}", count);
  logger.debug("K_int: {}", k_128_int);
  logger.debug("MAC-I: {}", mac);
  logger.debug(v.begin(), v.end(), "Message input:");

  // trim MAC-I from PDU.
  buf.trim_tail(sec_mac_len);
  return security_status::success;
}
