/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#ifndef BSWAP_H
#define BSWAP_H

#include <stdint.h>

#if __GNUC__ > 5 || __clang_major__ > 3
	static inline uint16_t bswap16(uint16_t x)
	{ return __builtin_bswap16(x); }

	static inline uint32_t bswap32(uint32_t x)
	{ return __builtin_bswap32(x); }

	static inline uint64_t bswap64(uint64_t x)
	{ return __builtin_bswap64(x); }
#else
	#define bswap16(x) ( \
		(x & (0x00FF)) << 8 | \
		(x & (0xFF00)) >> 8 \
	)

	#define bswap32(x) ( \
		(x & (0x000000FF)) << 8*3 | \
		(x & (0x0000FF00)) << 8*1 | \
		(x & (0x00FF0000)) >> 8*1 | \
		(x & (0xFF000000)) >> 8*3 \
	)

	#define bswap64(x) ( \
		(x & (0x00000000000000FF)) << 8*7 | \
		(x & (0x000000000000FF00)) << 8*5 | \
		(x & (0x0000000000FF0000)) << 8*3 | \
		(x & (0x00000000FF000000)) << 8*1 | \
		(x & (0x000000FF00000000)) >> 8*1 | \
		(x & (0x0000FF0000000000)) >> 8*3 | \
		(x & (0x00FF000000000000)) >> 8*5 | \
		(x & (0xFF00000000000000)) >> 8*7 \
	)
#endif

#endif
