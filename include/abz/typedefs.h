#ifndef ABZ_TYPEDEFS_H
#define ABZ_TYPEDEFS_H

/*
 * Copyright (c) Abraham vd Merwe <abz@blio.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of other contributors
 *	  may be used to endorse or promote products derived from this software
 *	  without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <inttypes.h>
#include <sys/cdefs.h>
#include <endian.h>

#define ARRAYSIZE(x) (sizeof(x) / sizeof((x)[0]))

#define NIPQUAD(addr)				\
	((uint8_t *) &addr)[0],			\
	((uint8_t *) &addr)[1],			\
	((uint8_t *) &addr)[2],			\
	((uint8_t *) &addr)[3]

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define HIPQUAD(addr)				\
	((uint8_t *) &addr)[3],			\
	((uint8_t *) &addr)[2],			\
	((uint8_t *) &addr)[1],			\
	((uint8_t *) &addr)[0]
#else	/* #if __BYTE_ORDER == __LITTLE_ENDIAN */
#define HIPQUAD NIPQUAD
#endif	/* #if __BYTE_ORDER == __LITTLE_ENDIAN */

/*
 * Workaround for MIN(), MAX() macros defined in sys/param.h
 */
#undef MIN
#undef MAX

#define MIN(x,y) ({					\
		const typeof(x) _x = (x);	\
		const typeof(y) _y = (y);	\
		(void) (&_x == &_y);		\
		_x < _y ? _x : _y;			\
	})

#define MAX(x,y) ({					\
		const typeof(x) _x = (x);	\
		const typeof(y) _y = (y);	\
		(void) (&_x == &_y);		\
		_x > _y ? _x : _y;			\
	})

#define barrier() __asm__ __volatile__("": : :"memory")

#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)

/*
 * Unfortunately, FreeBSD inttypes does not include the standard
 * printf and scanf macros, so we have to try our best to guess
 * what they should be (easy for everything but the 64-bit
 * macros).
 */

#ifndef PRId8
#define PRId8 "d"
#endif	/* #ifndef PRId8 */

#ifndef PRId16
#define PRId16 "d"
#endif	/* #ifndef PRId16 */

#ifndef PRId32
#define PRId32 "d"
#endif	/* #ifndef PRId32 */

#ifndef PRId64
#define PRId64 "lld"
#endif	/* #ifndef PRId64 */

#ifndef PRIu8
#define PRIu8 "u"
#endif	/* #ifndef PRIu8 */

#ifndef PRIu16
#define PRIu16 "u"
#endif	/* #ifndef PRIu16 */

#ifndef PRIu32
#define PRIu32 "u"
#endif	/* #ifndef PRIu32 */

#ifndef PRIu64
#define PRIu64 "llu"
#endif	/* #ifndef PRIu64 */

#endif	/* #ifndef ABZ_TYPEDEFS_H */
