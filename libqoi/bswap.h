/* Licensed under the MIT License
 * Copyright (c) 2024 Smgdream */

#ifndef BSWAP_H
#define BSWAP_H


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
		(x & (0x000000FF)) << 24 | \
		(x & (0x0000FF00)) << 8  | \
		(x & (0x00FF0000)) >> 8  | \
		(x & (0xFF000000)) >> 24 \
	)

	#define bswap64(x) ( \
		(x & (0x00000000000000FF)) << 56 | \
		(x & (0x000000000000FF00)) << 40 | \
		(x & (0x0000000000FF0000)) << 24 | \
		(x & (0x00000000FF000000)) << 8  | \
		(x & (0x000000FF00000000)) >> 8  | \
		(x & (0x0000FF0000000000)) >> 24 | \
		(x & (0x00FF000000000000)) >> 40 | \
		(x & (0xFF00000000000000)) >> 56 \
	)
#endif

#endif
