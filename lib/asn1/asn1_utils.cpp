// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/asn1_utils.h"
#include "ocudu/adt/bounded_bitset.h"
#include "ocudu/asn1/asn1_ap_utils.h"
#include <cstring>

using ocudu::byte_buffer;
using ocudu::span;

namespace asn1 {

OCUDUASN_CODE pack_bits(bit_ref& bref, std::initializer_list<bool> flags)
{
  // Flags are packed in words, with the first flag in the most significant bit.
  const bool* flag = flags.begin();
  for (auto nof_left = static_cast<uint32_t>(flags.size()); nof_left > 0;) {
    uint32_t n_bits = std::min(nof_left, 64U);
    uint64_t word   = 0;
    for (uint32_t i = 0; i != n_bits; ++i, ++flag) {
      word = (word << 1U) | static_cast<uint64_t>(*flag);
    }
    HANDLE_CODE(bref.pack(word, n_bits));
    nof_left -= n_bits;
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE unpack_bits(cbit_ref& bref, std::initializer_list<bool*> flags)
{
  bool* const* flag = flags.begin();
  for (auto nof_left = static_cast<uint32_t>(flags.size()); nof_left > 0;) {
    uint32_t n_bits = std::min(nof_left, 64U);
    uint64_t word   = 0;
    HANDLE_CODE(bref.unpack(word, n_bits));
    for (uint32_t i = 0; i != n_bits; ++i, ++flag) {
      **flag = ((word >> (n_bits - 1U - i)) & 1U) != 0;
    }
    nof_left -= n_bits;
  }
  return OCUDUASN_SUCCESS;
}

/************************
       dyn_array
************************/

namespace {

void construct_elems(const detail::dyn_array_ops& ops, void* dst, uint32_t n)
{
  if (n == 0) {
    return;
  }
  if (ops.construct != nullptr) {
    ops.construct(dst, n);
  } else {
    std::memset(dst, 0, size_t(n) * ops.elem_size);
  }
}

void copy_elems(const detail::dyn_array_ops& ops, void* dst, const void* src, uint32_t n)
{
  if (n == 0) {
    return;
  }
  if (ops.copy != nullptr) {
    ops.copy(dst, src, n);
  } else {
    std::memcpy(dst, src, size_t(n) * ops.elem_size);
  }
}

void relocate_elems(const detail::dyn_array_ops& ops, void* dst, void* src, uint32_t n)
{
  if (n == 0) {
    return;
  }
  if (ops.relocate != nullptr) {
    ops.relocate(dst, src, n);
  } else {
    std::memmove(dst, src, size_t(n) * ops.elem_size);
  }
}

void destroy_elems(const detail::dyn_array_ops& ops, void* ptr, uint32_t n)
{
  if (n != 0 and ops.destroy != nullptr) {
    ops.destroy(ptr, n);
  }
}

void* elem_at(const detail::dyn_array_ops& ops, void* data, uint32_t idx)
{
  return static_cast<uint8_t*>(data) + size_t(idx) * ops.elem_size;
}

} // namespace

void detail::dyn_array_base::reallocate(const dyn_array_ops& ops, uint32_t new_cap)
{
  void* new_data = new_cap > 0 ? ::operator new(size_t(new_cap) * ops.elem_size) : nullptr;
  relocate_elems(ops, new_data, data_, size_);
  ::operator delete(data_);
  data_ = new_data;
  cap_  = new_cap;
}

void detail::dyn_array_base::assign_impl(const dyn_array_ops& ops, const void* src, uint32_t nof_items)
{
  destroy_elems(ops, data_, size_);
  size_ = 0;
  if (nof_items > cap_) {
    reallocate(ops, nof_items);
  }
  copy_elems(ops, data_, src, nof_items);
  size_ = nof_items;
}

void detail::dyn_array_base::move_impl(const dyn_array_ops& ops, dyn_array_base& other)
{
  release_impl(ops);
  data_       = other.data_;
  size_       = other.size_;
  cap_        = other.cap_;
  other.data_ = nullptr;
  other.size_ = 0;
  other.cap_  = 0;
}

void detail::dyn_array_base::reserve_impl(const dyn_array_ops& ops, uint32_t new_cap)
{
  if (new_cap > cap_) {
    reallocate(ops, new_cap);
  }
}

void detail::dyn_array_base::resize_impl(const dyn_array_ops& ops, uint32_t new_size, uint32_t new_cap)
{
  if (new_size <= size_) {
    destroy_elems(ops, elem_at(ops, data_, new_size), size_ - new_size);
    size_ = new_size;
    return;
  }
  if (new_size > cap_) {
    reallocate(ops, std::max(new_size, new_cap));
  }
  construct_elems(ops, elem_at(ops, data_, size_), new_size - size_);
  size_ = new_size;
}

void detail::dyn_array_base::erase_impl(const dyn_array_ops& ops, uint32_t idx)
{
  destroy_elems(ops, elem_at(ops, data_, idx), 1);
  relocate_elems(ops, elem_at(ops, data_, idx), elem_at(ops, data_, idx + 1), size_ - idx - 1);
  size_--;
}

void detail::dyn_array_base::release_impl(const dyn_array_ops& ops)
{
  destroy_elems(ops, data_, size_);
  ::operator delete(data_);
  data_ = nullptr;
  size_ = 0;
  cap_  = 0;
}

OCUDUASN_CODE detail::pack_dyn_seq_of_items(bit_ref&     bref,
                                            const void*  items,
                                            uint32_t     nof_items,
                                            uint32_t     item_size,
                                            uint32_t     lb,
                                            uint32_t     ub,
                                            bool         aligned,
                                            pack_item_fn pack_item,
                                            void*        packer)
{
  HANDLE_CODE(pack_length(bref, nof_items, lb, ub, aligned));
  const auto* item = static_cast<const uint8_t*>(items);
  for (uint32_t i = 0; i != nof_items; ++i, item += item_size) {
    HANDLE_CODE(pack_item(bref, item, packer));
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE detail::unpack_dyn_seq_of_items(cbit_ref&      bref,
                                              void*          seqof,
                                              resize_seq_fn  resize,
                                              uint32_t       item_size,
                                              uint32_t       lb,
                                              uint32_t       ub,
                                              bool           aligned,
                                              unpack_item_fn unpack_item,
                                              void*          unpacker)
{
  uint32_t nof_items;
  HANDLE_CODE(unpack_length(nof_items, bref, lb, ub, aligned));
  auto* item = static_cast<uint8_t*>(resize(seqof, nof_items));
  for (uint32_t i = 0; i != nof_items; ++i, item += item_size) {
    HANDLE_CODE(unpack_item(bref, item, unpacker));
  }
  return OCUDUASN_SUCCESS;
}

/************************
        logging
************************/

void warn_assert(bool cond, const char* filename, int lineno)
{
  if (cond) {
    log_warning("Assertion in [{}][{}] failed.", filename, lineno);
  }
}

void invalid_enum_number(int value, const char* name)
{
  log_error("The provided enum value={} of type {} cannot be translated into a number", value, name);
}

void assert_choice_type(uint32_t val, uint32_t choice_id)
{
  if (OCUDU_UNLIKELY(val != choice_id)) {
    log_invalid_access_choice_id(val, choice_id);
  }
}

/************************
     error handling
************************/

void log_error_code(OCUDUASN_CODE code, const char* filename, int line)
{
  switch (code) {
    case OCUDUASN_ERROR_ENCODE_FAIL:
      log_error("[{}][{}] Encoding failure.", filename, line);
      break;
    case OCUDUASN_ERROR_DECODE_FAIL:
      log_error("[{}][{}] Decoding failure.", filename, line);
      break;
    default:
      log_warning("[{}][{}] OCUDUASN_CODE={} not recognized.", filename, line, (int)code);
  }
}

const char* convert_enum_idx(const char* array[], uint32_t nof_types, uint32_t enum_val, const char* enum_type)
{
  if (enum_val >= nof_types) {
    if (enum_val == nof_types) {
      log_error("The enum of type {} was not initialized.", enum_type);
    } else {
      log_error("The enum value={} of type {} is not valid.", enum_val, enum_type);
    }
    return "";
  }
  return array[enum_val];
}

bool convert_enum_str(const char* array[],
                      uint32_t    nof_types,
                      const char* str,
                      uint32_t&   enum_val,
                      const char* enum_type)
{
  for (uint32_t i = 0; i < nof_types; ++i) {
    if (strcmp(str, array[i]) == 0) {
      enum_val = i;
      return true;
    }
  }
  log_error("The string '{}' is not a valid value for enum type {}.", str, enum_type);
  return false;
}

template <class ItemType>
ItemType map_enum_number(ItemType* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type)
{
  if (enum_val >= nof_types) {
    if (enum_val == nof_types) {
      log_error("The enum of type {} is not initialized.", enum_type);
    } else {
      log_error("The enum value={} of type {} cannot be converted to a number.", enum_val, enum_type);
    }
    return 0;
  }
  return array[enum_val];
}
template const uint8_t
map_enum_number<const uint8_t>(const uint8_t* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type);
template const uint16_t
map_enum_number<const uint16_t>(const uint16_t* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type);
template const uint32_t
map_enum_number<const uint32_t>(const uint32_t* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type);
template const uint64_t
map_enum_number<const uint64_t>(const uint64_t* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type);
template const int8_t
map_enum_number<const int8_t>(const int8_t* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type);
template const int16_t
map_enum_number<const int16_t>(const int16_t* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type);
template const int32_t
map_enum_number<const int32_t>(const int32_t* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type);
template const int64_t
map_enum_number<const int64_t>(const int64_t* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type);
template const float
map_enum_number<const float>(const float* array, uint32_t nof_types, uint32_t enum_val, const char* enum_type);

/*********************
       bit_ref
*********************/

int bit_ref::distance_bytes() const
{
  return static_cast<int>(writer.length());
}

int bit_ref::distance() const
{
  return 8 * (static_cast<int>(writer.length()) - (offset == 0 ? 0 : 1)) + offset;
}

OCUDUASN_CODE bit_ref::pack(uint64_t val, uint32_t n_bits)
{
  ocudu_assert(n_bits <= 64, "Invalid number of bits passed to pack()");
  while (n_bits > 0) {
    if (offset == 0) {
      if (not writer.append(0)) {
        return OCUDUASN_ERROR;
      }
    }
    // Bitmap of n_bits ones, avoiding the undefined 64-bit shift.
    uint64_t mask = n_bits == 64 ? ~0UL : ((1UL << n_bits) - 1UL);
    val           = val & mask;
    if (static_cast<uint32_t>(8U - offset) > n_bits) {
      // in case n_bits < number of bits left in current byte (ie, last iteration).
      auto shifted_val = static_cast<uint8_t>(val << (8u - offset - n_bits));
      writer.back() += shifted_val;
      offset += n_bits;
      n_bits = 0;
    } else {
      // in case, space in current byte is lower or equal to n_bits (ie, not the last iteration).
      auto shifted_val = static_cast<uint8_t>(val >> (n_bits - 8u + offset));
      writer.back() += shifted_val;
      n_bits -= 8U - offset;
      offset = 0;
    }
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE bit_ref::pack_bytes(ocudu::span<const uint8_t> bytes)
{
  if (bytes.empty()) {
    return OCUDUASN_SUCCESS;
  }
  if (offset == 0) {
    // Aligned case
    HANDLE_CODE(writer.append(bytes) ? OCUDUASN_SUCCESS : OCUDUASN_ERROR_ENCODE_FAIL);
  } else {
    for (uint8_t byte : bytes) {
      HANDLE_CODE(pack(byte, 8U));
    }
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE bit_ref::pack_bytes(ocudu::byte_buffer_view bytes)
{
  if (bytes.empty()) {
    return OCUDUASN_SUCCESS;
  }
  if (offset == 0) {
    // Aligned case.
    HANDLE_CODE(writer.append(bytes) ? OCUDUASN_SUCCESS : OCUDUASN_ERROR_ENCODE_FAIL);
  } else {
    for (uint8_t byte : bytes) {
      HANDLE_CODE(pack(byte, 8U));
    }
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE bit_ref::align_bytes_zero()
{
  offset = 0;
  return OCUDUASN_SUCCESS;
}

cbit_ref cbit_ref::subview(uint32_t offset_bytes, uint32_t len_bytes) const
{
  const uint32_t buffer_rem_bytes = buffer.end() - it;
  const uint32_t nof_bytes_needed = len_bytes + ((offset != 0) ? 1 : 0); // account for the last bits in the last byte.
  if (offset_bytes + nof_bytes_needed > buffer_rem_bytes) {
    log_error("subview: Buffer size limit was achieved");
    return cbit_ref(ocudu::byte_buffer_view{});
  }

  auto     start_it = it + offset_bytes;
  cbit_ref v{ocudu::byte_buffer_view(start_it, start_it + nof_bytes_needed)};
  v.offset = offset;
  return v;
}

int cbit_ref::distance_bytes() const
{
  return static_cast<int>(it - buffer.begin()) + (offset != 0 ? 1 : 0);
}

int cbit_ref::distance() const
{
  return (int)offset + 8 * static_cast<int>(it - buffer.begin());
}

int cbit_ref::distance(const cbit_ref& other) const
{
  return static_cast<int>(it - other.it) * 8 + (static_cast<int>(offset) - static_cast<int>(other.offset));
}

template <class T>
OCUDUASN_CODE cbit_ref::unpack(T& val, uint32_t n_bits)
{
  if (n_bits > sizeof(T) * 8) {
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  val = 0;
  while (n_bits > 0) {
    if (it == buffer.end()) {
      log_error("{}: Buffer size limit {} was reached.", __FUNCTION__, buffer.length());
      return OCUDUASN_ERROR_DECODE_FAIL;
    }
    if ((uint32_t)(8 - offset) > n_bits) {
      uint8_t mask = (uint8_t)(1u << (8u - offset)) - (uint8_t)(1u << (8u - offset - n_bits));
      val += ((uint32_t)((*it) & mask)) >> (8u - offset - n_bits);
      offset += n_bits;
      n_bits = 0;
    } else {
      auto mask = static_cast<uint8_t>((1u << (8u - offset)) - 1u);
      val += ((uint64_t)((*it) & mask)) << (n_bits - 8 + offset);
      n_bits -= 8 - offset;
      offset = 0;
      ++it;
    }
  }
  return OCUDUASN_SUCCESS;
}
template OCUDUASN_CODE cbit_ref::unpack<bool>(bool&, uint32_t n_bits);
template OCUDUASN_CODE cbit_ref::unpack<uint8_t>(uint8_t&, uint32_t n_bits);
template OCUDUASN_CODE cbit_ref::unpack<uint16_t>(uint16_t&, uint32_t n_bits);
template OCUDUASN_CODE cbit_ref::unpack<uint32_t>(uint32_t&, uint32_t n_bits);
template OCUDUASN_CODE cbit_ref::unpack<uint64_t>(uint64_t&, uint32_t n_bits);

OCUDUASN_CODE cbit_ref::unpack_bytes(ocudu::span<uint8_t> bytes)
{
  if (bytes.empty()) {
    return OCUDUASN_SUCCESS;
  }
  if (static_cast<std::ptrdiff_t>(bytes.size()) > buffer.end() - it) {
    log_error("unpack_bytes: Buffer size limit was achieved");
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  if (offset == 0) {
    // Aligned case
    std::copy(buffer.begin(), buffer.end(), bytes.begin());
    it += bytes.size();
  } else {
    // Unaligned case
    for (uint32_t i = 0; i < bytes.size(); ++i) {
      HANDLE_CODE(unpack(bytes[i], 8));
    }
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE cbit_ref::align_bytes()
{
  if (offset != 0) {
    if (it == buffer.end()) {
      log_error("{}: Buffer size limit {} was achieved.", __FUNCTION__, buffer.length());
      return OCUDUASN_ERROR_DECODE_FAIL;
    }
    ++it;
    offset = 0;
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE cbit_ref::advance_bits(uint32_t n_bits)
{
  uint32_t extra_bits     = (offset + n_bits) % 8;
  uint32_t bytes_required = ceil_frac(offset + n_bits, 8U);
  uint32_t bytes_offset   = (offset + n_bits) / 8U;

  if (bytes_required > buffer.end() - it) {
    log_error("advance_bytes: Buffer size limit was achieved");
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  it += bytes_offset;
  offset = extra_bits;
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE cbit_ref::advance_bytes(uint32_t bytes)
{
  if (bytes + (offset != 0 ? 1 : 0) > buffer.end() - it) {
    log_error("advance_bytes: Buffer size limit was achieved");
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  it += bytes;
  return OCUDUASN_SUCCESS;
}

/*********************
     ext packing
*********************/

OCUDUASN_CODE pack_unsupported_ext_flag(bit_ref& bref, bool ext)
{
  HANDLE_CODE(bref.pack(ext, 1));
  if (ext) {
    log_error("ASN extensions not currently supported");
    return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE unpack_unsupported_ext_flag(bool& ext, cbit_ref& bref)
{
  OCUDUASN_CODE ret = bref.unpack(ext, 1);
  if (ext) {
    log_error("ASN extensions not currently supported");
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return ret;
}

/************************
      enum packing
************************/

// Pack without extension
OCUDUASN_CODE pack_enum(bit_ref& bref, uint32_t enum_val, uint32_t nbits)
{
  return bref.pack(enum_val, nbits);
}

// Pack with extension
OCUDUASN_CODE pack_enum(bit_ref& bref, uint32_t enum_val, uint32_t nbits, uint32_t nof_noext)
{
  bool ext = enum_val >= nof_noext;
  HANDLE_CODE(bref.pack((bool)ext, 1));
  OCUDUASN_CODE ret;
  if (not ext) {
    ret = bref.pack(enum_val, nbits);
  } else {
    ret = pack_norm_small_non_neg_whole_number(bref, enum_val - nof_noext);
  }
  return ret;
}

OCUDUASN_CODE pack_enum(bit_ref& bref, uint32_t e, uint32_t nof_types, uint32_t nof_exts, bool has_ext)
{
  if (e >= nof_types) {
    log_error(
        "The provided enum is not within the range of possible values ({}>={})", (unsigned)e, (unsigned)nof_types);
    return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  OCUDUASN_CODE ret;
  if (has_ext) {
    uint32_t nof_bits = (uint32_t)std::ceil(std::log2(nof_types - nof_exts));
    ret               = pack_enum(bref, e, nof_bits, nof_types - nof_exts);
  } else {
    uint32_t nof_bits = (uint32_t)std::ceil(std::log2(nof_types));
    ret               = pack_enum(bref, e, nof_bits);
  }
  return ret;
}

ValOrError unpack_enum(uint32_t nof_types, uint32_t nof_exts, bool has_ext, cbit_ref& bref)
{
  ValOrError ret;
  if (has_ext) {
    uint32_t nof_bits = (uint32_t)std::ceil(std::log2(nof_types - nof_exts));
    bool     ext;
    ret.code = bref.unpack(ext, 1);
    if (ret.code != OCUDUASN_SUCCESS) {
      return ret;
    }
    if (not ext) {
      ret.code = bref.unpack(ret.val, nof_bits);
    } else {
      ret.code = unpack_norm_small_non_neg_whole_number(ret.val, bref);
      ret.val += nof_types - nof_exts;
    }
  } else {
    uint32_t nof_bits = (uint32_t)std::ceil(std::log2(nof_types));
    ret.code          = bref.unpack(ret.val, nof_bits);
  }
  if (ret.val >= nof_types) {
    log_error("The provided enum is not within the range of possible values ({}>={})",
              (unsigned)ret.val,
              (unsigned)nof_types);
    ret.code = OCUDUASN_ERROR_DECODE_FAIL;
  }
  return ret;
}

/************************
     PER encoding
************************/

/**
 * X.691 - Section 10.5
 * Encoder function for a constrained whole number
 * @tparam IntType type of integer (e.g. uint8_t, uint32_t, etc.)
 * @param bref bit_ref where to encode integer
 * @param n integer current value
 * @param lb lower bound
 * @param ub upper bound
 * @return success or failure
 */
/// Number of bits needed to represent an unsigned value.
static uint32_t bit_width(uint64_t v)
{
  uint32_t n = 0;
  for (; v != 0; v >>= 1) {
    ++n;
  }
  return n;
}

template <class IntType>
OCUDUASN_CODE pack_constrained_whole_number(bit_ref& bref, IntType n, IntType lb, IntType ub, bool aligned)
{
  if (ub < lb or n < lb or n > ub) {
    log_error("The condition lb <= n <= ub ({} <= {} <= {}) was not met", (long)lb, (long)n, (long)ub);
    return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  // Unsigned arithmetic, since the range of a 64-bit type does not fit in it.
  uint64_t max_offset = (uint64_t)ub - (uint64_t)lb;
  if (max_offset == 0) {
    return OCUDUASN_SUCCESS;
  }
  uint32_t n_bits   = bit_width(max_offset);
  uint64_t toencode = (uint64_t)n - (uint64_t)lb;
  if (not aligned) {
    // UNALIGNED variant
    return bref.pack(toencode, n_bits);
  }
  // ALIGNED variant
  if (max_offset < 255) {
    return bref.pack(toencode, n_bits);
  }
  if (max_offset < ASN_64K) {
    // both one and two octets cases
    uint32_t n_octets = ceil_frac(n_bits, 8u);
    HANDLE_CODE(bref.align_bytes_zero());
    HANDLE_CODE(bref.pack(toencode, n_octets * 8));
    return bref.align_bytes_zero();
  }
  // Octet count as a constrained whole number in 1..max octets, followed by the minimum octets.
  uint32_t max_octets = ceil_frac(n_bits, 8u);
  uint32_t n_octets   = std::max(ceil_frac(bit_width(toencode), 8u), 1u);
  HANDLE_CODE(bref.pack(n_octets - 1, bit_width(max_octets - 1)));
  HANDLE_CODE(bref.align_bytes_zero());
  return bref.pack(toencode, n_octets * 8u);
}
template OCUDUASN_CODE
pack_constrained_whole_number<int8_t>(bit_ref& bref, int8_t n, int8_t lb, int8_t ub, bool aligned);
template OCUDUASN_CODE
pack_constrained_whole_number<int16_t>(bit_ref& bref, int16_t n, int16_t lb, int16_t ub, bool aligned);
template OCUDUASN_CODE
pack_constrained_whole_number<int32_t>(bit_ref& bref, int32_t n, int32_t lb, int32_t ub, bool aligned);
template OCUDUASN_CODE
pack_constrained_whole_number<int64_t>(bit_ref& bref, int64_t n, int64_t lb, int64_t ub, bool aligned);
template OCUDUASN_CODE
pack_constrained_whole_number<uint8_t>(bit_ref& bref, uint8_t n, uint8_t lb, uint8_t ub, bool aligned);
template OCUDUASN_CODE
pack_constrained_whole_number<uint16_t>(bit_ref& bref, uint16_t n, uint16_t lb, uint16_t ub, bool aligned);
template OCUDUASN_CODE
pack_constrained_whole_number<uint32_t>(bit_ref& bref, uint32_t n, uint32_t lb, uint32_t ub, bool aligned);
template OCUDUASN_CODE
pack_constrained_whole_number<uint64_t>(bit_ref& bref, uint64_t n, uint64_t lb, uint64_t ub, bool aligned);

/**
 * X.691 - Section 10.5
 * Decoder function for a constrained whole number
 * @tparam IntType type of integer (e.g. uint8_t, uint32_t, etc.)
 * @param n integer with decoded value
 * @param bref bit_ref where to encode integer
 * @param lb lower bound
 * @param ub upper bound
 * @return success or failure
 */
template <class IntType>
OCUDUASN_CODE unpack_constrained_whole_number(IntType& n, cbit_ref& bref, IntType lb, IntType ub, bool aligned)
{
  if (ub < lb) {
    log_error("The condition lb <= ub ({} <= {}) was not met", (long)lb, (long)ub);
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  // Unsigned arithmetic, since the range of a 64-bit type does not fit in it.
  uint64_t max_offset = (uint64_t)ub - (uint64_t)lb;
  uint64_t offset     = 0;
  uint32_t n_bits     = bit_width(max_offset);
  if (max_offset == 0) {
    // Single value, nothing to decode.
  } else if (not aligned or max_offset < 255) {
    HANDLE_CODE(bref.unpack(offset, n_bits));
  } else if (max_offset < ASN_64K) {
    HANDLE_CODE(bref.align_bytes());
    HANDLE_CODE(bref.unpack(offset, ceil_frac(n_bits, 8u) * 8));
    HANDLE_CODE(bref.align_bytes());
  } else {
    uint32_t max_octets = ceil_frac(n_bits, 8u);
    uint32_t n_octets;
    HANDLE_CODE(bref.unpack(n_octets, bit_width(max_octets - 1)));
    n_octets += 1;
    if (n_octets > max_octets) {
      log_error("Integer of {} octets exceeds the maximum of {} octets", n_octets, max_octets);
      return OCUDUASN_ERROR_DECODE_FAIL;
    }
    HANDLE_CODE(bref.align_bytes());
    HANDLE_CODE(bref.unpack(offset, n_octets * 8));
  }
  if (offset > max_offset) {
    log_error(
        "The condition lb <= n <= ub ({} <= {} <= {}) was not met", (long)lb, (long)((uint64_t)lb + offset), (long)ub);
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  n = (IntType)((uint64_t)lb + offset);
  return OCUDUASN_SUCCESS;
}
template OCUDUASN_CODE
unpack_constrained_whole_number<int8_t>(int8_t& n, cbit_ref& bref, int8_t lb, int8_t ub, bool aligned);
template OCUDUASN_CODE
unpack_constrained_whole_number<int16_t>(int16_t& n, cbit_ref& bref, int16_t lb, int16_t ub, bool aligned);
template OCUDUASN_CODE
unpack_constrained_whole_number<int32_t>(int32_t& n, cbit_ref& bref, int32_t lb, int32_t ub, bool aligned);
template OCUDUASN_CODE
unpack_constrained_whole_number<int64_t>(int64_t& n, cbit_ref& bref, int64_t lb, int64_t ub, bool aligned);
template OCUDUASN_CODE
unpack_constrained_whole_number<uint8_t>(uint8_t& n, cbit_ref& bref, uint8_t lb, uint8_t ub, bool aligned);
template OCUDUASN_CODE
unpack_constrained_whole_number<uint16_t>(uint16_t& n, cbit_ref& bref, uint16_t lb, uint16_t ub, bool aligned);
template OCUDUASN_CODE
unpack_constrained_whole_number<uint32_t>(uint32_t& n, cbit_ref& bref, uint32_t lb, uint32_t ub, bool aligned);
template OCUDUASN_CODE
unpack_constrained_whole_number<uint64_t>(uint64_t& n, cbit_ref& bref, uint64_t lb, uint64_t ub, bool aligned);

/**
 * X.691 - Section 10.6
 * Encoder function for a normally small non-negative whole number
 * @tparam IntType type of integer (e.g. uint8_t, uint32_t, etc.)
 * @param bref bit_ref where to encode integer
 * @param n integer current value
 * @return success or failure
 */
template <typename UintType>
OCUDUASN_CODE pack_norm_small_non_neg_whole_number(bit_ref& bref, UintType n)
{
  if (n <= 63) {
    HANDLE_CODE(bref.pack(n, 7)); // [1 bit: 0 | 6 bit: n]
  } else {
    HANDLE_CODE(bref.pack(1, 1));
    log_error("Long small integers not supported");
    return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
template <typename UintType>
OCUDUASN_CODE unpack_norm_small_non_neg_whole_number(UintType& n, cbit_ref& bref)
{
  bool          ext;
  OCUDUASN_CODE ret = bref.unpack(ext, 1);
  HANDLE_CODE(ret);
  if (not ext) {
    ret = bref.unpack(n, 6);
  } else {
    log_error("Long small integers not supported");
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return ret;
}
template OCUDUASN_CODE pack_norm_small_non_neg_whole_number<uint8_t>(bit_ref& bref, uint8_t n);
template OCUDUASN_CODE pack_norm_small_non_neg_whole_number<uint16_t>(bit_ref& bref, uint16_t n);
template OCUDUASN_CODE pack_norm_small_non_neg_whole_number<uint32_t>(bit_ref& bref, uint32_t n);
template OCUDUASN_CODE pack_norm_small_non_neg_whole_number<uint64_t>(bit_ref& bref, uint64_t n);
template OCUDUASN_CODE unpack_norm_small_non_neg_whole_number<uint8_t>(uint8_t& n, cbit_ref& bref);
template OCUDUASN_CODE unpack_norm_small_non_neg_whole_number<uint16_t>(uint16_t& n, cbit_ref& bref);
template OCUDUASN_CODE unpack_norm_small_non_neg_whole_number<uint32_t>(uint32_t& n, cbit_ref& bref);
template OCUDUASN_CODE unpack_norm_small_non_neg_whole_number<uint64_t>(uint64_t& n, cbit_ref& bref);

/// Number of octets of the minimal two's-complement encoding of an unconstrained whole number (X.691 - Section 10.8,
/// X.690 - Section 8.3). A non-negative value needs a leading zero bit, so e.g. 128 takes two octets, not one.
template <typename IntType>
uint32_t unconstrained_whole_number_length(IntType n)
{
  uint32_t len = 1;
  if constexpr (std::is_signed<IntType>::value) {
    auto val = static_cast<int64_t>(n);
    while (val > 127 or val < -128) {
      val >>= 8;
      ++len;
    }
  } else {
    auto val = static_cast<uint64_t>(n);
    while (val > 127) {
      val >>= 8;
      ++len;
    }
  }
  return len;
}

/**
 * X.691 - Section 10.8
 * Encoder function for an unconstrained whole number
 * @tparam IntType type of integer (e.g. int8_t, uint32_t, etc.)
 * @param bref bit_ref where to encode integer
 * @param n integer current value
 * @return success or failure
 */
template <typename IntType>
OCUDUASN_CODE pack_unconstrained_whole_number(bit_ref& bref, IntType n, bool aligned)
{
  uint32_t len = unconstrained_whole_number_length(n);
  if (aligned) {
    HANDLE_CODE(bref.align_bytes_zero());
  }
  // Two's-complement content octets, most significant first. The conversion to uint64_t yields the 64-bit two's-
  // complement pattern of the value, so shifting out the low octets works for both signed and unsigned types.
  auto bits = static_cast<uint64_t>(n);
  for (uint32_t i = len; i != 0; --i) {
    uint32_t shift = 8U * (i - 1);
    // A non-negative value wider than 63 bits needs a 9th octet, which is the leading zero octet.
    uint64_t octet = shift < 64U ? (bits >> shift) & 0xffU : 0U;
    HANDLE_CODE(bref.pack(octet, 8));
  }

  return OCUDUASN_SUCCESS;
}
template <typename IntType>
OCUDUASN_CODE unpack_unconstrained_whole_number(IntType& n, cbit_ref& bref, uint32_t len, bool aligned)
{
  if (aligned) {
    HANDLE_CODE(bref.align_bytes());
  }
  if (len == 0) {
    log_error("Unconstrained whole number with no content octets");
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  uint64_t bits     = 0;
  bool     negative = false;
  bool     fits     = true;
  for (uint32_t i = 0; i != len; ++i) {
    uint8_t octet = 0;
    HANDLE_CODE(bref.unpack(octet, 8));
    if (i == 0) {
      // The encoding is negative if the top bit of the first content octet is set.
      negative = (octet & 0x80U) != 0;
      fits     = not negative or std::is_signed<IntType>::value;
    }
    if (len - i > sizeof(uint64_t)) {
      // Leading octet that is dropped. It only carries sign extension if it repeats the sign bit.
      fits = fits and octet == (negative ? 0xffU : 0x00U);
      continue;
    }
    bits = (bits << 8U) | octet;
  }
  if (negative and len < sizeof(uint64_t)) {
    // Sign extension of the two's-complement encoding.
    bits |= ~static_cast<uint64_t>(0) << (8U * len);
  } else if (std::is_signed<IntType>::value and len >= sizeof(uint64_t)) {
    // Octets were dropped from the front, so the retained bits must carry the sign of the encoding themselves.
    fits = fits and ((bits >> 63U) != 0) == negative;
  }
  n = static_cast<IntType>(bits);
  if (not fits or static_cast<uint64_t>(n) != bits) {
    log_error("Unconstrained whole number of {} octets does not fit in the destination type", len);
    return OCUDUASN_ERROR_DECODE_FAIL;
  }

  return OCUDUASN_SUCCESS;
}
template OCUDUASN_CODE pack_unconstrained_whole_number<int8_t>(bit_ref& bref, int8_t n, bool aligned);
template OCUDUASN_CODE pack_unconstrained_whole_number<int16_t>(bit_ref& bref, int16_t n, bool aligned);
template OCUDUASN_CODE pack_unconstrained_whole_number<int32_t>(bit_ref& bref, int32_t n, bool aligned);
template OCUDUASN_CODE pack_unconstrained_whole_number<int64_t>(bit_ref& bref, int64_t n, bool aligned);
template OCUDUASN_CODE unpack_unconstrained_whole_number<int8_t>(int8_t& n, cbit_ref& bref, uint32_t len, bool aligned);
template OCUDUASN_CODE
unpack_unconstrained_whole_number<int16_t>(int16_t& n, cbit_ref& bref, uint32_t len, bool aligned);
template OCUDUASN_CODE
unpack_unconstrained_whole_number<int32_t>(int32_t& n, cbit_ref& bref, uint32_t len, bool aligned);
template OCUDUASN_CODE
unpack_unconstrained_whole_number<int64_t>(int64_t& n, cbit_ref& bref, uint32_t len, bool aligned);
template OCUDUASN_CODE pack_unconstrained_whole_number<uint8_t>(bit_ref& bref, uint8_t n, bool aligned);
template OCUDUASN_CODE pack_unconstrained_whole_number<uint16_t>(bit_ref& bref, uint16_t n, bool aligned);
template OCUDUASN_CODE pack_unconstrained_whole_number<uint32_t>(bit_ref& bref, uint32_t n, bool aligned);
template OCUDUASN_CODE pack_unconstrained_whole_number<uint64_t>(bit_ref& bref, uint64_t n, bool aligned);
template OCUDUASN_CODE
unpack_unconstrained_whole_number<uint8_t>(uint8_t& n, cbit_ref& bref, uint32_t len, bool aligned);
template OCUDUASN_CODE
unpack_unconstrained_whole_number<uint16_t>(uint16_t& n, cbit_ref& bref, uint32_t len, bool aligned);
template OCUDUASN_CODE
unpack_unconstrained_whole_number<uint32_t>(uint32_t& n, cbit_ref& bref, uint32_t len, bool aligned);
template OCUDUASN_CODE
unpack_unconstrained_whole_number<uint64_t>(uint64_t& n, cbit_ref& bref, uint32_t len, bool aligned);

/*********************
   varlength_packing
*********************/

template <typename IntType>
OCUDUASN_CODE pack_length(bit_ref& bref, IntType n, IntType lb, IntType ub, bool aligned)
{
  if (ub >= ASN_64K) {
    return pack_length(bref, n, aligned);
  }
  return pack_constrained_whole_number(bref, n, lb, ub, aligned);
}
template OCUDUASN_CODE pack_length<uint8_t>(bit_ref& bref, uint8_t n, uint8_t lb, uint8_t ub, bool aligned);
template OCUDUASN_CODE pack_length<uint16_t>(bit_ref& bref, uint16_t n, uint16_t lb, uint16_t ub, bool aligned);
template OCUDUASN_CODE pack_length<uint32_t>(bit_ref& bref, uint32_t n, uint32_t lb, uint32_t ub, bool aligned);
template OCUDUASN_CODE pack_length<uint64_t>(bit_ref& bref, uint64_t n, uint64_t lb, uint64_t ub, bool aligned);
template OCUDUASN_CODE pack_length<int8_t>(bit_ref& bref, int8_t n, int8_t lb, int8_t ub, bool aligned);
template OCUDUASN_CODE pack_length<int16_t>(bit_ref& bref, int16_t n, int16_t lb, int16_t ub, bool aligned);
template OCUDUASN_CODE pack_length<int32_t>(bit_ref& bref, int32_t n, int32_t lb, int32_t ub, bool aligned);
template OCUDUASN_CODE pack_length<int64_t>(bit_ref& bref, int64_t n, int64_t lb, int64_t ub, bool aligned);

template <typename IntType>
OCUDUASN_CODE unpack_length(IntType& n, cbit_ref& bref, IntType lb, IntType ub, bool aligned)
{
  if (ub >= ASN_64K) {
    uint32_t      len;
    OCUDUASN_CODE ret = unpack_length(len, bref, aligned);
    n                 = len;
    return ret;
  }
  return unpack_constrained_whole_number(n, bref, lb, ub, aligned);
}
template OCUDUASN_CODE unpack_length<uint8_t>(uint8_t& n, cbit_ref& bref, uint8_t lb, uint8_t ub, bool aligned);
template OCUDUASN_CODE unpack_length<uint16_t>(uint16_t& n, cbit_ref& bref, uint16_t lb, uint16_t ub, bool aligned);
template OCUDUASN_CODE unpack_length<uint32_t>(uint32_t& n, cbit_ref& bref, uint32_t lb, uint32_t ub, bool aligned);
template OCUDUASN_CODE unpack_length<uint64_t>(uint64_t& n, cbit_ref& bref, uint64_t lb, uint64_t ub, bool aligned);
template OCUDUASN_CODE unpack_length<int8_t>(int8_t& n, cbit_ref& bref, int8_t lb, int8_t ub, bool aligned);
template OCUDUASN_CODE unpack_length<int16_t>(int16_t& n, cbit_ref& bref, int16_t lb, int16_t ub, bool aligned);
template OCUDUASN_CODE unpack_length<int32_t>(int32_t& n, cbit_ref& bref, int32_t lb, int32_t ub, bool aligned);
template OCUDUASN_CODE unpack_length<int64_t>(int64_t& n, cbit_ref& bref, int64_t lb, int64_t ub, bool aligned);

/**
 * X.691 - Section 10.9
 * Pack unconstrained length determinant
 * @param bref
 * @param val "n" length value to pack
 * @param aligned
 * @return
 */
OCUDUASN_CODE pack_length(bit_ref& bref, uint32_t val, bool aligned)
{
  if (not aligned) {
    if (val < 128) {
      HANDLE_CODE(bref.pack(val, 8)); // first bit is 0
    } else if (val < ASN_16K) {
      HANDLE_CODE(bref.pack(0b10, 2));
      HANDLE_CODE(bref.pack(val, 14));
    } else {
      log_error("Not handling sizes longer than 16383 octets");
      return OCUDUASN_ERROR_ENCODE_FAIL;
    }
  } else {
    if (val > ASN_64K) {
      // TODO: Error message
      return OCUDUASN_ERROR_ENCODE_FAIL;
    }
    HANDLE_CODE(bref.align_bytes_zero());
    if (val < 128) {
      return bref.pack(val, 8); // first bit is 0
    } else if (val < ASN_16K) {
      HANDLE_CODE(bref.pack(0b10, 2));
      return bref.pack(val, 14);
    } else {
      HANDLE_CODE(bref.pack(0b11, 2));
      uint32_t m = val / ASN_16K;
      HANDLE_CODE(bref.pack(m, 6));
      val = val - m * ASN_16K;
      return bref.pack(val, 16); // TODO: TODO
    }
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE unpack_length(uint32_t& val, cbit_ref& bref, bool aligned)
{
  bool ext;
  if (not aligned) {
    OCUDUASN_CODE ret = bref.unpack(ext, 1);
    HANDLE_CODE(ret);
    if (not ext) {
      ret = bref.unpack(val, 7);
    } else {
      ret = bref.unpack(ext, 1);
      HANDLE_CODE(ret);
      if (not ext) {
        ret = bref.unpack(val, 14);
      } else {
        log_error("Not handling octet strings longer than 16383 octets");
        val = 0;
        return OCUDUASN_ERROR_DECODE_FAIL;
      }
    }
    return ret;
  } else {
    HANDLE_CODE(bref.align_bytes());
    HANDLE_CODE(bref.unpack(val, 8));
    if (val < 128) {
      // "n" < 128
      // first bit was set to zero
      return OCUDUASN_SUCCESS;
    }
    val -= 128;
    if (val < 64) {
      // 128 <= "n" < 16K
      // second bit is set to zero as well
      uint32_t val_octet_2;
      HANDLE_CODE(bref.unpack(val_octet_2, 8));
      val = (val << 8u) + val_octet_2;
      return OCUDUASN_SUCCESS;
    }
    val = 0;
    log_error("Not handling octet strings longer than 16383 octets");
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
}

/************************
        Integer
************************/

template <typename IntType>
OCUDUASN_CODE pack_integer(bit_ref& bref, IntType n, IntType lb, IntType ub, bool has_ext, bool aligned)
{
  // pack ext marker
  bool within_bounds = n >= lb and n <= ub;
  if (has_ext) {
    HANDLE_CODE(bref.pack(not within_bounds, 1));
  } else if (not within_bounds) {
    log_error("The condition lb <= n <= ub ({} <= {} <= {}) was not met", (long)lb, (long)n, (long)ub);
    return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  // Type limits of signed integers stand for absent bounds; unsigned integers are always constrained.
  bool lower_bounded = lb != std::numeric_limits<IntType>::min() or std::is_unsigned<IntType>::value;
  bool upper_bounded = ub != std::numeric_limits<IntType>::max() or std::is_unsigned<IntType>::value;

  if (within_bounds and lower_bounded and upper_bounded) {
    HANDLE_CODE(pack_constrained_whole_number(bref, n, (IntType)lb, (IntType)ub, aligned));
  } else {
    // See X.691 - 12.2.6
    if (not within_bounds or (not lower_bounded and not upper_bounded)) {
      HANDLE_CODE(pack_length(bref, unconstrained_whole_number_length(n), aligned));
      HANDLE_CODE(pack_unconstrained_whole_number(bref, n, aligned));
    } else {
      log_error("Semi-constrained integers are not supported");
      return OCUDUASN_ERROR_ENCODE_FAIL;
    }
  }
  return OCUDUASN_SUCCESS;
}
template OCUDUASN_CODE
pack_integer<uint8_t>(bit_ref& bref, uint8_t n, uint8_t lb, uint8_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
pack_integer<uint16_t>(bit_ref& bref, uint16_t n, uint16_t lb, uint16_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
pack_integer<uint32_t>(bit_ref& bref, uint32_t n, uint32_t lb, uint32_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
pack_integer<uint64_t>(bit_ref& bref, uint64_t n, uint64_t lb, uint64_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE pack_integer<int8_t>(bit_ref& bref, int8_t n, int8_t lb, int8_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
pack_integer<int16_t>(bit_ref& bref, int16_t n, int16_t lb, int16_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
pack_integer<int32_t>(bit_ref& bref, int32_t n, int32_t lb, int32_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
pack_integer<int64_t>(bit_ref& bref, int64_t n, int64_t lb, int64_t ub, bool has_ext, bool aligned);

// unconstrained specialization case
template <typename IntType>
OCUDUASN_CODE pack_unconstrained_integer(bit_ref& bref, IntType n, bool has_ext, bool aligned)
{
  return pack_integer(
      bref, n, std::numeric_limits<IntType>::min(), std::numeric_limits<IntType>::max(), has_ext, aligned);
}
template OCUDUASN_CODE pack_unconstrained_integer<int8_t>(bit_ref& bref, int8_t n, bool has_ext, bool aligned);
template OCUDUASN_CODE pack_unconstrained_integer<int16_t>(bit_ref& bref, int16_t n, bool has_ext, bool aligned);
template OCUDUASN_CODE pack_unconstrained_integer<int32_t>(bit_ref& bref, int32_t n, bool has_ext, bool aligned);
template OCUDUASN_CODE pack_unconstrained_integer<int64_t>(bit_ref& bref, int64_t n, bool has_ext, bool aligned);

template <typename IntType>
OCUDUASN_CODE unpack_integer(IntType& n, cbit_ref& bref, IntType lb, IntType ub, bool has_ext, bool aligned)
{
  bool within_bounds = true;
  if (has_ext) {
    HANDLE_CODE(bref.unpack(within_bounds, 1));
    within_bounds = not within_bounds;
  }
  // Type limits of signed integers stand for absent bounds; unsigned integers are always constrained.
  bool lower_bounded = lb != std::numeric_limits<IntType>::min() or std::is_unsigned<IntType>::value;
  bool upper_bounded = ub != std::numeric_limits<IntType>::max() or std::is_unsigned<IntType>::value;

  if (within_bounds and lower_bounded and upper_bounded) {
    HANDLE_CODE(unpack_constrained_whole_number(n, bref, (IntType)lb, (IntType)ub, aligned));
  } else {
    // See X.691 - 12.2.6
    if (not within_bounds or (not lower_bounded and not upper_bounded)) {
      uint32_t len;
      HANDLE_CODE(unpack_length(len, bref, aligned));
      HANDLE_CODE(unpack_unconstrained_whole_number(n, bref, len, aligned));
    } else {
      log_error("Semi-constrained integers are not supported");
      return OCUDUASN_ERROR_DECODE_FAIL;
    }
  }
  return OCUDUASN_SUCCESS;
}
template OCUDUASN_CODE
unpack_integer<uint8_t>(uint8_t& n, cbit_ref& bref, uint8_t lb, uint8_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
unpack_integer<uint16_t>(uint16_t& n, cbit_ref& bref, uint16_t lb, uint16_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
unpack_integer<uint32_t>(uint32_t& n, cbit_ref& bref, uint32_t lb, uint32_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
unpack_integer<uint64_t>(uint64_t& n, cbit_ref& bref, uint64_t lb, uint64_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
unpack_integer<int8_t>(int8_t& n, cbit_ref& bref, int8_t lb, int8_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
unpack_integer<int16_t>(int16_t& n, cbit_ref& bref, int16_t lb, int16_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
unpack_integer<int32_t>(int32_t& n, cbit_ref& bref, int32_t lb, int32_t ub, bool has_ext, bool aligned);
template OCUDUASN_CODE
unpack_integer<int64_t>(int64_t& n, cbit_ref& bref, int64_t lb, int64_t ub, bool has_ext, bool aligned);

// unconstrained specialization case
template <typename IntType>
OCUDUASN_CODE unpack_unconstrained_integer(IntType& n, cbit_ref& bref, bool has_ext, bool aligned)
{
  return unpack_integer(
      n, bref, std::numeric_limits<IntType>::min(), std::numeric_limits<IntType>::max(), has_ext, aligned);
}
template OCUDUASN_CODE unpack_unconstrained_integer<int8_t>(int8_t& n, cbit_ref& bref, bool has_ext, bool aligned);
template OCUDUASN_CODE unpack_unconstrained_integer<int16_t>(int16_t& n, cbit_ref& bref, bool has_ext, bool aligned);
template OCUDUASN_CODE unpack_unconstrained_integer<int32_t>(int32_t& n, cbit_ref& bref, bool has_ext, bool aligned);
template OCUDUASN_CODE unpack_unconstrained_integer<int64_t>(int64_t& n, cbit_ref& bref, bool has_ext, bool aligned);

// standalone packer
template <class IntType>
integer_packer<IntType>::integer_packer(IntType lb_, IntType ub_, bool has_ext_, bool aligned_) :
  lb(lb_), ub(ub_), has_ext(has_ext_), aligned(aligned_)
{
}

template <class IntType>
OCUDUASN_CODE integer_packer<IntType>::pack(bit_ref& bref, IntType n)
{
  return pack_integer(bref, n, lb, ub, has_ext, aligned);
}
template <class IntType>
OCUDUASN_CODE integer_packer<IntType>::unpack(IntType& n, cbit_ref& bref)
{
  return unpack_integer(n, bref, lb, ub, has_ext, aligned);
}
template struct integer_packer<int8_t>;
template struct integer_packer<int16_t>;
template struct integer_packer<int32_t>;
template struct integer_packer<int64_t>;
template struct integer_packer<uint8_t>;
template struct integer_packer<uint16_t>;
template struct integer_packer<uint32_t>;
template struct integer_packer<uint64_t>;

/************************
    common octstring
************************/

uint64_t octet_string_helper::to_uint(ocudu::span<const uint8_t> buf)
{
  size_t nbytes = buf.size();
  if (nbytes > 8U) {
    log_error("OCTET STRING of size={} does not fit in an uint64_t", nbytes);
    return 0;
  }
  uint64_t val = 0;
  for (unsigned i = 0; i < nbytes; ++i) {
    val += ((uint64_t)buf[nbytes - 1 - i]) << (uint64_t)(i * 8U);
  }
  return val;
}

uint64_t octet_string_helper::to_uint(const byte_buffer& buf)
{
  size_t nbytes = buf.length();
  if (nbytes > 8) {
    log_error("OCTET STRING with size={} does not fit in an uint64_t", nbytes);
    return 0;
  }
  uint64_t val = 0;
  auto     it  = buf.begin();
  for (unsigned i = 0; i < nbytes; ++i) {
    val += static_cast<uint64_t>(*it) << static_cast<uint64_t>((nbytes - 1 - i) * 8U);
    ++it;
  }
  return val;
}

void octet_string_helper::to_octet_string(ocudu::span<uint8_t> buf, uint64_t number)
{
  uint64_t nbytes = buf.size();
  if (nbytes < 8 and (static_cast<uint64_t>(1U) << (8U * nbytes)) <= number) {
    log_error("Integer={} does not fit in an OCTET STRING of size={}", number, nbytes);
    return;
  }
  for (uint32_t i = 0; i < nbytes; ++i) {
    buf[nbytes - 1 - i] = (number >> (uint64_t)(i * 8U)) & 0xffu;
  }
}

void octet_string_helper::to_octet_string(ocudu::byte_buffer& buf, uint64_t number)
{
  buf           = byte_buffer{byte_buffer::fallback_allocation_tag{}};
  size_t nbytes = sizeof(number);
  for (uint32_t i = 0; i < nbytes; ++i) {
    if (not buf.append((number >> (uint64_t)((nbytes - 1 - i) * 8U)) & 0xffu)) {
      log_error("Failed to append octet string byte to buffer");
      return;
    }
  }
}

static void to_hex(char* cstr, uint8_t val)
{
  std::sprintf(cstr, "%02x", val);
}

std::string octet_string_helper::to_hex_string(ocudu::span<const uint8_t> buf)
{
  std::string s;
  s.resize(buf.size() * 2);
  char cstr[3];
  for (uint32_t i = 0; i < buf.size(); i++) {
    to_hex(cstr, buf[i]);
    s.replace(i * 2, 2, cstr);
  }
  return s;
}

std::string octet_string_helper::to_hex_string(const byte_buffer& buf)
{
  std::string s;
  s.resize(buf.length() * 2);
  char     cstr[3];
  unsigned i = 0;
  for (uint8_t b : buf) {
    to_hex(cstr, b);
    s.replace(i * 2, 2, cstr);
    ++i;
  }
  return s;
}

unsigned octet_string_helper::hex_string_to_octets(ocudu::span<uint8_t> buf, const std::string& str)
{
  ocudu_assert(buf.size() >= ceil_frac(str.size(), (size_t)2U), "out-of-bounds access");
  if (str.size() % 2 != 0) {
    log_error("The provided hex string size={} is not a multiple of 2.", str.size());
  }
  char cstr[] = "\0\0\0";
  for (unsigned i = 0; i < str.size(); i += 2) {
    std::memcpy(&cstr[0], &str[i], 2);
    buf[i / 2] = strtoul(cstr, nullptr, 16);
  }
  return (str.size() + 1) / 2;
}

void octet_string_helper::append_hex_string(byte_buffer& buf, const std::string& str)
{
  if (str.size() % 2 != 0) {
    log_error("The provided hex string size={} is not a multiple of 2.", str.size());
  }
  char cstr[] = "\0\0\0";
  for (unsigned i = 0; i < str.size(); i += 2) {
    std::memcpy(&cstr[0], &str[i], 2);
    if (not buf.append(strtoul(cstr, nullptr, 16))) {
      log_error("Failed to append octet string byte to buffer");
    }
  }
}

/************************
     dyn_octstring
************************/

template <bool Al>
unbounded_octstring<Al>::unbounded_octstring(const unbounded_octstring& other) noexcept :
  // Use fallback allocator, because operator= should never fail.
  ocudu::byte_buffer(fallback_allocation_tag{}, other)
{
}

template <bool Al>
unbounded_octstring<Al>& unbounded_octstring<Al>::operator=(const unbounded_octstring& other) noexcept
{
  if (this != &other) {
    // Use fallback allocator, because operator= should never fail.
    *this = byte_buffer{fallback_allocation_tag{}, other};
  }
  return *this;
}

template <bool Al>
unbounded_octstring<Al>& unbounded_octstring<Al>::operator=(unbounded_octstring&& other) noexcept
{
  *static_cast<byte_buffer*>(this) = std::move(other);
  return *this;
}

template <bool Al>
OCUDUASN_CODE unbounded_octstring<Al>::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_length(bref, length(), aligned));
  for (uint8_t b : *this) {
    HANDLE_CODE(bref.pack(b, 8));
  }
  return OCUDUASN_SUCCESS;
}

template <bool Al>
OCUDUASN_CODE unbounded_octstring<Al>::unpack(cbit_ref& bref)
{
  unsigned len;
  HANDLE_CODE(unpack_length(len, bref, aligned));
  clear();
  for (unsigned i = 0; i != len; ++i) {
    uint8_t b;
    HANDLE_CODE(bref.unpack(b, 8));
    HANDLE_CODE(append(b) ? OCUDUASN_SUCCESS : OCUDUASN_ERROR_DECODE_FAIL);
  }
  return OCUDUASN_SUCCESS;
}

template <bool Al>
std::string unbounded_octstring<Al>::to_string() const
{
  return octet_string_helper::to_hex_string(*this);
}

template <bool Al>
unbounded_octstring<Al>& unbounded_octstring<Al>::from_string(const std::string& hexstr)
{
  // clears previous buffer.
  *this = byte_buffer{byte_buffer::fallback_allocation_tag{}};

  // appends hex string to buffer.
  octet_string_helper::append_hex_string(*this, hexstr);

  return *this;
}

template <bool Al>
unbounded_octstring<Al>& unbounded_octstring<Al>::from_bytes(span<const uint8_t> bytes)
{
  // clears previous buffer.
  *this = byte_buffer{byte_buffer::fallback_allocation_tag{}};

  // appends bytes to buffer.
  if (not this->append(bytes)) {
    log_error("Failed to append byte to buffer");
  }

  return *this;
}

template <bool Al>
uint64_t unbounded_octstring<Al>::to_number() const
{
  return octet_string_helper::to_uint(*this);
}

template <bool Al>
unbounded_octstring<Al>& unbounded_octstring<Al>::from_number(uint64_t val)
{
  octet_string_helper::to_octet_string(*this, val);
  return *this;
}

template class unbounded_octstring<true>;
template class unbounded_octstring<false>;

/*********************
     bitstring
*********************/

namespace bitstring_utils {

/**
 * Pack ASN1 bitstring length prefix. Accommodates for cases: fixed/unbounded/bounded, aligned/unaligned, with/out ext
 */
static OCUDUASN_CODE pack_length_prefix(bit_ref& bref,
                                        uint32_t len,
                                        uint32_t lb         = 0,
                                        uint32_t ub         = std::numeric_limits<uint32_t>::max(),
                                        bool     has_ext    = false,
                                        bool     is_aligned = false)
{
  if (has_ext and ub == std::numeric_limits<uint32_t>::max()) {
    log_error("has extension marker but it is an unbounded prefix size");
    return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  bool within_bounds = len >= lb and len <= ub;
  if (not within_bounds and not has_ext) {
    log_error("bitstring length={} is not within bounds [{}, {}]", len, lb, ub);
    return OCUDUASN_ERROR_ENCODE_FAIL;
  }

  // encode ext bit
  if (has_ext) {
    HANDLE_CODE(bref.pack(not within_bounds, 1));
  }

  // pack as unbounded if unbounded bitstring or ext is active
  if (ub == std::numeric_limits<uint32_t>::max() or not within_bounds) {
    return pack_length(bref, len, is_aligned);
  }

  // do not encode prefix if fixed size
  if (lb == ub) {
    return OCUDUASN_SUCCESS;
  }

  return pack_length(bref, len, lb, ub, is_aligned);
}

static OCUDUASN_CODE
pack_bitfield(bit_ref& bref, const uint8_t* buf, uint32_t nbits, uint32_t lb, uint32_t ub, bool is_aligned)
{
  if (nbits == 0) {
    // Empty bit string, which only the length prefix encodes.
    return OCUDUASN_SUCCESS;
  }
  if (is_aligned and (lb != ub or ub > 16)) {
    bref.align_bytes_zero();
  }
  uint32_t n_octs = ceil_frac(nbits, 8u);
  uint32_t offset = ((nbits - 1) % 8) + 1;
  HANDLE_CODE(bref.pack(buf[n_octs - 1], offset));
  for (uint32_t i = 1; i < n_octs; ++i) {
    HANDLE_CODE(bref.pack(buf[n_octs - 1 - i], 8));
  }
  return OCUDUASN_SUCCESS;
}

/**
 * Pack ASN1 bitstring. Accommodates for cases: fixed/unbounded/bounded, aligned/unaligned, with/out ext
 */
OCUDUASN_CODE
pack(bit_ref& bref, const uint8_t* data, uint32_t len, uint32_t lb, uint32_t ub, bool has_ext, bool is_aligned)
{
  HANDLE_CODE(bitstring_utils::pack_length_prefix(bref, len, lb, ub, has_ext, is_aligned));
  if (len < lb or len > ub) {
    // Lengths outside the root range are encoded as if unbounded.
    return pack_bitfield(bref, data, len, 0, std::numeric_limits<uint32_t>::max(), is_aligned);
  }
  return pack_bitfield(bref, data, len, lb, ub, is_aligned);
}

// Unpack prefix, excluding ext bit

/**
 * Unpack ASN1 bitstring length prefix. Accommodates for cases: fixed/unbounded/bounded, aligned/unaligned, with/out ext
 */
OCUDUASN_CODE
unpack_length_prefix(uint32_t& len, cbit_ref& bref, uint32_t lb, uint32_t ub, bool has_ext, bool is_aligned)
{
  bool ext = false;
  if (has_ext) {
    HANDLE_CODE(bref.unpack(ext, 1));
  }

  // unbounded or ext is set
  if (ub == std::numeric_limits<uint32_t>::max() or ext) {
    return unpack_length(len, bref, is_aligned);
  }

  // fixed length with no ext
  if (lb == ub) {
    len = lb;
    return OCUDUASN_SUCCESS;
  }

  // constrained, simple case
  HANDLE_CODE(unpack_length(len, bref, lb, ub, is_aligned));
  // Lengths with an upper bound of 64K or more are decoded without range check.
  if (len < lb or len > ub) {
    log_error("bitstring length={} is not within bounds [{}, {}]", len, lb, ub);
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

// for both fixed, constrained and unconstrained scenarios
OCUDUASN_CODE unpack_bitfield(uint8_t* buf, cbit_ref& bref, uint32_t n, uint32_t lb, uint32_t ub, bool is_aligned)
{
  if (n > ASN_64K) {
    log_error("bitstrings longer than 64K not supported");
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  if (n == 0) {
    // empty bit string
    return OCUDUASN_SUCCESS;
  }
  if (is_aligned and (lb != ub or ub > 16)) {
    bref.align_bytes();
  }
  // unpack exactly "n" bits
  uint32_t n_octs = ceil_frac(n, 8u);
  uint32_t offset = ((n - 1) % 8) + 1;
  HANDLE_CODE(bref.unpack(buf[n_octs - 1], offset));
  for (uint32_t i = 1; i < n_octs; ++i) {
    HANDLE_CODE(bref.unpack(buf[n_octs - 1 - i], 8));
  }
  return OCUDUASN_SUCCESS;
}

void from_number(uint8_t* ptr, uint64_t number, uint32_t nbits)
{
  if (nbits > 64u) {
    log_error("bitstring of size={} does not fit in an uint64_t", nbits);
    return;
  }
  uint32_t nof_bytes = ceil_frac(nbits, 8u);
  for (uint32_t i = 0; i < nof_bytes; ++i) {
    ptr[i] = (number >> (i * 8u)) & 0xffu;
  }
  uint32_t offset = nbits % 8; // clean up any extra set bit
  if (offset > 0) {
    ptr[nof_bytes - 1] &= (uint8_t)((1u << offset) - 1u);
  }
}

std::string to_string(const uint8_t* ptr, uint32_t nbits)
{
  std::string str(nbits, '\0');
  for (uint32_t i = 0; i < nbits; ++i) {
    str[i] = bitstring_utils::get(ptr, nbits - 1 - i) ? '1' : '0';
  }
  return str;
}

std::string to_hex_string(const uint8_t* ptr, uint32_t nbits)
{
  std::vector<uint8_t> octets(ceil_frac(nbits, 8u), 0);
  for (uint32_t i = 0; i < nbits; ++i) {
    if (bitstring_utils::get(ptr, nbits - 1 - i)) {
      octets[i / 8] |= 0x80u >> (i % 8);
    }
  }
  return octet_string_helper::to_hex_string(octets);
}

uint64_t to_number(const uint8_t* ptr, uint32_t nbits)
{
  if (nbits > 64u) {
    log_error("bitstring of size={} does not fit in an uint64_t", nbits);
    return 0;
  }
  uint64_t val       = 0;
  uint32_t nof_bytes = ceil_frac(nbits, 8u);
  for (uint32_t i = 0; i < nof_bytes; ++i) {
    val += static_cast<uint64_t>(ptr[i]) << static_cast<uint64_t>(i * 8u);
  }
  return val;
}

} // namespace bitstring_utils

/*********************
     choice utils
*********************/
void log_invalid_access_choice_id(uint32_t val, uint32_t choice_id)
{
  log_error("The access choide id is invalid ({}!={})", (size_t)val, (size_t)choice_id);
}
void log_invalid_choice_id(uint32_t val, const char* choice_type)
{
  log_error("Invalid choice id={} for choice type {}", (size_t)val, choice_type);
}

/*********************
   printable string
*********************/

namespace asn_string_utils {

static size_t get_nof_bits_per_char(size_t lb, size_t ub, bool aligned)
{
  size_t N = ub - lb + 1;
  auto   b = (size_t)std::ceil(std::log2(N)); // B
  if (aligned) {
    b = (size_t)std::pow(2, std::ceil(std::log2(b))); // B2
  }
  return b;
}

static bool is_octet_aligned(size_t bits_per_char, size_t alb, size_t aub, bool aligned)
{
  size_t max_nof_bits = bits_per_char * aub;
  return aligned and (max_nof_bits > 16 or (alb != aub and max_nof_bits == 16));
}

static constexpr bool is_length_encoded(size_t alb, size_t aub, bool aligned)
{
  return alb != aub or aub >= ASN_64K;
}

OCUDUASN_CODE
pack(bit_ref& bref, const std::string& s, size_t lb, size_t ub, size_t alb, size_t aub, bool ext, bool aligned)
{
  bool within_limits = s.size() >= alb and s.size() <= aub;
  if (not within_limits and not ext) {
    log_error("The PrintableString size={} is not within the limits [{}, {}]", s.size(), alb, aub);
    return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  if (ext) {
    HANDLE_CODE(bref.pack(not within_limits, 1));
  }
  if (not within_limits) {
    // Sizes outside the root range are encoded as if unbounded.
    alb = 0;
    aub = std::numeric_limits<uint32_t>::max();
  }
  size_t b              = asn_string_utils::get_nof_bits_per_char(lb, ub, aligned);
  bool   octet_aligned  = asn_string_utils::is_octet_aligned(b, alb, aub, aligned);
  bool   length_encoded = asn_string_utils::is_length_encoded(alb, aub, aligned);

  if (length_encoded) {
    HANDLE_CODE(pack_length(bref, s.size(), alb, aub, octet_aligned));
  }
  if (octet_aligned) {
    HANDLE_CODE(bref.align_bytes_zero());
  }
  for (char c : s) {
    HANDLE_CODE(bref.pack(c, b));
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE
unpack(std::string& s, cbit_ref& bref, size_t lb, size_t ub, size_t alb, size_t aub, bool ext, bool aligned)
{
  bool is_ext = false;
  if (ext) {
    HANDLE_CODE(bref.unpack(is_ext, 1));
  }
  if (is_ext) {
    // Sizes outside the root range are encoded as if unbounded.
    alb = 0;
    aub = std::numeric_limits<uint32_t>::max();
  }
  size_t b              = asn_string_utils::get_nof_bits_per_char(lb, ub, aligned);
  bool   octet_aligned  = asn_string_utils::is_octet_aligned(b, alb, aub, aligned);
  bool   length_encoded = asn_string_utils::is_length_encoded(alb, aub, aligned);
  if (length_encoded) {
    size_t n;
    HANDLE_CODE(unpack_length(n, bref, alb, aub, octet_aligned));
    s.resize(n);
  }
  if (octet_aligned) {
    HANDLE_CODE(bref.align_bytes());
  }
  for (char& c : s) {
    HANDLE_CODE(bref.unpack(c, b));
  }
  return OCUDUASN_SUCCESS;
}

} // namespace asn_string_utils

/*********************
      ext group
*********************/

bool& ext_groups_packer_guard::operator[](uint32_t idx)
{
  if (idx >= groups.size()) {
    uint32_t prev_size = groups.size();
    groups.resize(idx + 1);
    std::fill(&groups[prev_size], &groups[groups.size()], false);
  }
  return groups[idx];
}

bool ext_groups_packer_guard::any() const
{
  for (uint32_t i = 0; i != groups.size(); ++i) {
    if (groups[i]) {
      return true;
    }
  }
  return false;
}

OCUDUASN_CODE ext_groups_packer_guard::pack(asn1::bit_ref& bref) const
{
  // pack number of groups
  int32_t i = groups.size() - 1;
  for (; i >= 0; --i) {
    if (groups[i]) {
      break;
    }
  }
  uint32_t nof_groups = (uint32_t)i + 1u;
  HANDLE_CODE(pack_norm_small_non_neg_whole_number(bref, nof_groups - 1));

  // pack each group presence flag
  for (uint32_t j = 0; j < nof_groups; ++j) {
    HANDLE_CODE(bref.pack(groups[j], 1));
  }
  return OCUDUASN_SUCCESS;
}

ext_groups_unpacker_guard::ext_groups_unpacker_guard(uint32_t nof_supported_groups_) :
  nof_supported_groups(nof_supported_groups_)
{
  resize(nof_supported_groups);
}

bool& ext_groups_unpacker_guard::operator[](uint32_t idx)
{
  if (idx >= groups.size()) {
    // only resizes for unknown extensions
    resize(idx + 1);
  }
  return groups[idx];
}

void ext_groups_unpacker_guard::resize(uint32_t new_size)
{
  // always grows
  uint32_t prev_size = groups.size();
  groups.resize(std::max(new_size, nof_supported_groups));
  std::fill(&groups[prev_size], &groups[groups.size()], false);
}

ext_groups_unpacker_guard::~ext_groups_unpacker_guard()
{
  // consume all the unknown extensions
  for (uint32_t i = nof_supported_groups; i < nof_unpacked_groups; ++i) {
    if (groups[i]) {
      varlength_field_unpack_guard scope(*bref_tracker);
    }
  }
}

OCUDUASN_CODE ext_groups_unpacker_guard::unpack(cbit_ref& bref)
{
  bref_tracker = &bref;
  // unpack nof of ext groups
  uint32_t nof_ext_groups = 0;
  HANDLE_CODE(unpack_norm_small_non_neg_whole_number(nof_ext_groups, bref));
  nof_unpacked_groups += nof_ext_groups + 1;
  resize(nof_unpacked_groups);

  // unpack each group presence flag
  for (uint32_t i = 0; i < nof_unpacked_groups; ++i) {
    HANDLE_CODE(bref.unpack(groups[i], 1));
  }
  return OCUDUASN_SUCCESS;
}

ext_groups_unpacker::ext_groups_unpacker(cbit_ref& bref, bool aligned_) :
  aligned(aligned_), outer_bref(bref), group_bref({})
{
}

OCUDUASN_CODE ext_groups_unpacker::unpack_presence_flags()
{
  // unpack nof of ext groups
  uint32_t nof_ext_groups = 0;
  HANDLE_CODE(unpack_norm_small_non_neg_whole_number(nof_ext_groups, outer_bref));
  nof_unpacked_groups = nof_ext_groups + 1;

  // Resize presence flags vector
  groups.resize(nof_unpacked_groups);
  std::fill(groups.data(), groups.data() + nof_unpacked_groups, false);

  // unpack each group presence flag
  for (uint32_t i = 0; i < nof_unpacked_groups; ++i) {
    HANDLE_CODE(outer_bref.unpack(groups[i], 1));
  }

  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE ext_groups_unpacker::unpack_next_group()
{
  if (state == state_t::unpack_presence_flags) {
    state = state_t::unpack_groups;
    HANDLE_CODE(unpack_presence_flags());
  }

  if (next_group_idx >= nof_unpacked_groups) {
    state = state_t::done;
    return OCUDUASN_SUCCESS;
  }

  if (groups[next_group_idx++]) {
    // Group is present. Decode its length.
    uint32_t group_len;
    HANDLE_CODE(unpack_length(group_len, outer_bref, aligned));
    // Create subview for the group
    group_bref = outer_bref.subview(0, group_len);
    // Consume the padding bits of the group from the outer bitref
    HANDLE_CODE(outer_bref.advance_bytes(group_len));
  }

  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE ext_groups_unpacker::consume_remaining_groups(cbit_ref& bref)
{
  if (state == state_t::unpack_presence_flags) {
    // No group was unpacked yet, so the presence flags are still pending.
    HANDLE_CODE(unpack_next_group());
  }
  while (next_group_idx < nof_unpacked_groups) {
    HANDLE_CODE(unpack_next_group());
  }
  bref = outer_bref;
  return OCUDUASN_SUCCESS;
}

/*********************
     Open Field
*********************/

varlength_field_pack_guard::varlength_field_pack_guard(bit_ref& bref, bool align_) : brefstart(bref)
{
  bref         = bit_ref(varlen_buffer);
  bref_tracker = &bref;
  align        = align_;
}

varlength_field_pack_guard::~varlength_field_pack_guard()
{
  if (bref_tracker->distance() == 0) {
    // X.691 replaces an empty complete encoding, e.g. of NULL, with a single zero octet.
    bref_tracker->pack(0, 8);
  }
  // fill the spare bits
  uint32_t leftover = 7U - ((bref_tracker->distance() - (uint32_t)1U) % (uint32_t)8U);
  bref_tracker->pack(0, leftover);

  // check how many bytes were written in total
  uint32_t nof_bytes = bref_tracker->distance() / (uint32_t)8U;

  // go back in time to pack length
  pack_length(brefstart, nof_bytes, align);

  // pack encoded bytes
  brefstart.pack_bytes(varlen_buffer);
  *bref_tracker = brefstart;
}

varlength_field_unpack_guard::varlength_field_unpack_guard(cbit_ref& bref, bool align) : bref0(bref)
{
  if (unpack_length(len, bref, align) != OCUDUASN_SUCCESS) {
    // The length could not be decoded, e.g. due to a truncated buffer. Leave an empty view, so that unpacking the field
    // contents fails, and do not restore the caller's bit_ref on destruction.
    len  = 0;
    bref = cbit_ref(ocudu::byte_buffer_view{});
    return;
  }
  bref0        = bref;
  bref_tracker = &bref;
  bref         = bref.subview(0, len);
}

varlength_field_unpack_guard::~varlength_field_unpack_guard()
{
  if (bref_tracker == nullptr) {
    return;
  }
  if (len * 8 < (unsigned)bref_tracker->distance(bref0)) {
    log_error("The number of bits unpacked exceeds the variable length field size ({} > {})",
              bref_tracker->distance(bref0),
              len * 8);
    return;
  }

  // Ignore padding bits, and skip to the end of the varlength field.
  bref0.advance_bytes(len);
  *bref_tracker = bref0;
}

/*******************
    JsonWriter
*******************/

json_writer::json_writer() : ident(""), sep(NONE) {}

void json_writer::write_fieldname(const char* fieldname)
{
  static constexpr const char* septable[] = {",\n", "\n", ""};

  fmt::format_to(std::back_inserter(buffer), "{}{}", septable[sep], sep != NONE ? ident : "");
  if (strlen(fieldname) != 0) {
    fmt::format_to(std::back_inserter(buffer), "\"{}\": ", fieldname);
  }
  sep = NONE;
}

void json_writer::write_str(const char* fieldname, const char* value)
{
  write_fieldname(fieldname);
  fmt::format_to(std::back_inserter(buffer), "\"{}\"", value);
  sep = COMMA;
}

void json_writer::write_str(const char* fieldname, const std::string& value)
{
  write_str(fieldname, value.c_str());
}

void json_writer::write_str(const char* value)
{
  write_str("", value);
}

void json_writer::write_str(const std::string& value)
{
  write_str("", value);
}

void json_writer::write_int(const char* fieldname, int64_t value)
{
  write_fieldname(fieldname);
  fmt::format_to(std::back_inserter(buffer), "{}", value);
  sep = COMMA;
}
void json_writer::write_int(int64_t value)
{
  write_int("", value);
}
void json_writer::write_uint(const char* fieldname, uint64_t value)
{
  write_fieldname(fieldname);
  fmt::format_to(std::back_inserter(buffer), "{}", value);
  sep = COMMA;
}
void json_writer::write_uint(uint64_t value)
{
  write_uint("", value);
}

void json_writer::write_bool(const char* fieldname, bool value)
{
  write_fieldname(fieldname);
  fmt::format_to(std::back_inserter(buffer), "{}", value ? "true" : "false");
  sep = COMMA;
}
void json_writer::write_bool(bool value)
{
  write_bool("", value);
}

void json_writer::write_float(const char* fieldname, float value)
{
  write_fieldname(fieldname);
  fmt::format_to(std::back_inserter(buffer), "{}", value);
  sep = COMMA;
}

void json_writer::write_float(float value)
{
  write_float("", value);
}
void json_writer::write_null(const char* fieldname)
{
  write_fieldname(fieldname);
  fmt::format_to(std::back_inserter(buffer), "null");
  sep = COMMA;
}
void json_writer::write_null()
{
  write_null("");
}

void json_writer::start_obj(const char* fieldname)
{
  write_fieldname(fieldname);
  fmt::format_to(std::back_inserter(buffer), "{{");
  ident += "  ";
  sep = NEWLINE;
}
void json_writer::end_obj()
{
  ident.erase(ident.size() - 2, 2);
  fmt::format_to(std::back_inserter(buffer), "\n{}}}", ident);
  sep = COMMA;
}

void json_writer::start_array(const char* fieldname)
{
  write_fieldname(fieldname);
  fmt::format_to(std::back_inserter(buffer), "[");
  ident += "  ";
  sep = NEWLINE;
}
void json_writer::end_array()
{
  ident.erase(ident.size() - 2, 2);
  fmt::format_to(std::back_inserter(buffer), "\n{}]", ident);
  sep = COMMA;
}

std::string json_writer::to_string() const
{
  return std::string(buffer.data(), buffer.size());
}

/************************
   General Layer Types
************************/

OCUDUASN_CODE skip_ignored_ie(cbit_ref& bref, crit_e crit, uint32_t id)
{
  if (crit != crit_e::ignore) {
    log_error("Unpacked object ID={} is not recognized", id);
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  uint32_t len;
  HANDLE_CODE(unpack_length(len, bref, true));
  return bref.advance_bytes(len);
}

OCUDUASN_CODE
detail::pack_ie(bit_ref& bref, uint32_t id, crit_e crit, pack_ie_value_fn pack_fn, const void* pack_value)
{
  HANDLE_CODE(pack_integer(bref, id, (uint32_t)0u, (uint32_t)65535u, false, true));
  HANDLE_CODE(crit.pack(bref));
  varlength_field_pack_guard varlen_scope(bref, true);
  return pack_fn(pack_value);
}

OCUDUASN_CODE detail::unpack_ie_value(cbit_ref& bref, unpack_ie_value_fn unpack_fn, const void* unpack_value)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  return unpack_fn(unpack_value);
}

OCUDUASN_CODE unpack_ie_header(cbit_ref& bref, uint32_t& id, crit_e& crit, crit_e (*expected_crit)(const uint32_t&))
{
  HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
  HANDLE_CODE(crit.unpack(bref));
  if (crit != expected_crit(id)) {
    log_warning("Unpacked crit does not match object set definition");
  }
  return OCUDUASN_SUCCESS;
}

uint32_t detail::base_empty_obj_set::idx_to_id(uint32_t idx)
{
  asn1::log_error("object set is empty\n");
  return 0;
}
bool detail::base_empty_obj_set::is_id_valid(const uint32_t& id)
{
  asn1::log_error("object set is empty\n");
  return false;
}
crit_e detail::base_empty_obj_set::get_crit(const uint32_t& id)
{
  return {};
}
presence_e detail::base_empty_obj_set::get_presence(const uint32_t& id)
{
  return {};
}

void detail::empty_obj_set_item_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.end_obj();
}
OCUDUASN_CODE detail::empty_obj_set_item_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE detail::empty_obj_set_item_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}

const char* detail::empty_obj_set_item_c::types_opts::to_string() const
{
  log_error("The enum value=0 of type protocol_ies_empty_o::value_c::types is not valid.");
  return "";
}

OCUDUASN_CODE pack_unconstrained_real(bit_ref& bref, float n, bool aligned)
{
  if (aligned) {
    HANDLE_CODE(bref.align_bytes_zero());
  }

  // Handle special cases
  if (std::isinf(n)) {
    pack_length(bref, 1, aligned);
    bref.pack(n > 0 ? 0x40 : 0x41, 8);
    return OCUDUASN_SUCCESS;
  } else if (std::isnan(n)) {
    pack_length(bref, 1, aligned);
    bref.pack(0x42, 8);
    return OCUDUASN_SUCCESS;
  } else if (n == 0.0f) {
    pack_length(bref, 0, aligned);
    return OCUDUASN_SUCCESS;
  }

  uint32_t* bits_ptr = (uint32_t*)&n;
  uint32_t  bits     = *bits_ptr;
  bool      sign     = (bits >> 31) & 0x1;
  int8_t    exponent = ((bits >> 23) & 0xff);
  uint32_t  mantissa = (bits & 0x7fffff) | 0x800000;

  uint8_t info_octet = 0x80; // Base-2
  if (sign) {
    info_octet |= 0x1 << 6;
  }

  uint32_t trailing_zeros = ocudu::zero_lsb_count<unsigned>(mantissa);
  mantissa >>= trailing_zeros;
  // the inverse of the trailing zeros gives the number of bits to shift
  // the mantissa to the right to make it a whole number, this number must be added to the exponent
  uint8_t inv_trailing_zeros = 23 - trailing_zeros;
  exponent -= (127 + inv_trailing_zeros);
  uint8_t asn1_exponent = exponent & 0xff;

  uint8_t mantissa_bit_len = 23 + 1 - trailing_zeros;
  uint8_t mantissa_oct_len = (mantissa_bit_len + 7) / 8;

  uint32_t len = mantissa_oct_len + 2;
  pack_length(bref, len, aligned);
  bref.pack(info_octet, 8);
  bref.pack(asn1_exponent, 8);
  for (int8_t i = mantissa_oct_len - 1; i >= 0; --i) {
    uint8_t octet = (mantissa >> (i * 8)) & 0xff;
    bref.pack(octet, 8);
  }
  return OCUDUASN_SUCCESS;
}

OCUDUASN_CODE unpack_unconstrained_real(float& n, cbit_ref& bref, bool aligned)
{
  if (aligned) {
    HANDLE_CODE(bref.align_bytes());
  }
  uint32_t len;
  HANDLE_CODE(unpack_length(len, bref, aligned));

  // Handle special case 0
  if (len == 0) {
    n = 0;
    return OCUDUASN_SUCCESS;
  }

  if (len == 2) {
    return OCUDUASN_ERROR_DECODE_FAIL;
  }

  uint8_t buf[10];
  if (len > sizeof(buf)) {
    asn1::log_error("REAL length {} exceeds maximum supported size {}", len, sizeof(buf));
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  for (uint32_t i = 0; i < len; i++) {
    HANDLE_CODE(bref.unpack(buf[i], 8));
  }

  uint8_t info_octet = buf[0];

  // Handle special cases
  if (info_octet == 0x40) {
    n = std::numeric_limits<float>::infinity(); // Positive infinity
    return OCUDUASN_SUCCESS;
  } else if (info_octet == 0x41) {
    n = -std::numeric_limits<float>::infinity(); // Negative infinity
    return OCUDUASN_SUCCESS;
  } else if (info_octet == 0x42) {
    n = std::numeric_limits<float>::quiet_NaN(); // NaN
    return OCUDUASN_SUCCESS;
  }

  uint8_t exponent_length = (info_octet & 0x3) + 1;
  uint8_t mantissa_length = len - exponent_length - 1;
  if (exponent_length != 1) {
    // Currently we support decoding only float with 1 byte exponent.
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  if (mantissa_length > 3) {
    // Currently we support decoding only float with 3 byte mantissa.
    return OCUDUASN_ERROR_DECODE_FAIL;
  }

  bool     sign          = (info_octet >> 6) & 0x1;
  int8_t   asn1_exponent = static_cast<int8_t>(buf[1]);
  uint32_t mantissa      = 0;

  for (size_t i = 2; i < len; ++i) {
    mantissa = (mantissa << 8) | buf[i];
  }
  uint8_t leading_zeros = ocudu::zero_msb_count<unsigned>(mantissa) - 8;
  mantissa <<= leading_zeros;

  // Build the IEEE 754 float value from the sign, exponent, and mantissa
  uint32_t ieee_exponent = asn1_exponent + 127;
  uint32_t ieee_mantissa = mantissa & 0x7fffff;

  if (ieee_mantissa) {
    ieee_exponent += (23 - leading_zeros);
  }

  uint32_t ieee_bits = (sign << 31) | (ieee_exponent << 23) | ieee_mantissa;
  float*   bits_ptr  = (float*)&ieee_bits;
  n                  = *bits_ptr;
  return OCUDUASN_SUCCESS;
}

} // namespace asn1
