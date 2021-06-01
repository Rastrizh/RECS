#ifndef RECSTYPES_H
#define RECSTYPES_H

	// Check if using 64-Bit architecture
#if (defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_X64))
#define MALLOC_64 1

// Check if using 32-Bit architecture
#elif (defined(_WIN32) && !defined(_WIN64)) || defined(_M_IX86)
#define MALLOC_32 1
#endif

#include <cstdlib>
#include <cstdint>

namespace RECS {

// signed integer type
	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;

#ifdef MALLOC_64
	using i64 = int64_t;
#else
	using i64 = int32_t;
#endif

	// unsigned integer type
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
#ifdef MALLOC_64
	using u64 = uint64_t;
#else
	using u64 = uint32_t;
#endif

	// floating point
	using f32 = float_t;
	using f64 = double_t;

	// pointer
	using iptr = intptr_t;
	using uptr = uintptr_t;

	using entityID = u32;
	using ComponentTypeID = size_t;
}
#endif // !RECSTYPES_H
