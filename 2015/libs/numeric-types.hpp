#ifndef _NUMERIC_TYPES_HPP
#define _NUMERIC_TYPES_HPP

#include <cstdint>

// Special case for unsigned 8-bit values
using UCHAR = unsigned char;

using U8 = std::uint8_t;
using I8 = std::int8_t;

using U16 = std::uint16_t;
using I16 = std::int16_t;

using U32 = std::uint32_t;
using I32 = std::int32_t;

using U64 = std::uint64_t;
using I64 = std::int64_t;

using F32 = float;
using F64 = double;

#endif /* _NUMERIC_TYPES_HPP */