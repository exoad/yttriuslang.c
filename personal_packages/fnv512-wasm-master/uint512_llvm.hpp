#pragma once
#include <cstdint>
#include <iostream>

/*

[                  64 bytes                                    ]        (1x uint512_t)
[              32              ][              32              ]        (2x uint256_t)
[     16       ][     16       ][     16       ][     16       ]        (4x uint128_t)
[   8  ][   8  ][   8  ][   8  ][   8  ][   8  ][   8  ][   8  ]        (8x uint64_t)
8888888888888888888888888888888888888888888888888888888888888888        (64 uint8_t / char)
1234567890123456789012345678901234567890123456789012345678901234 64

*/

extern "C"{

typedef unsigned _ExtInt(512) uint512_t;

#define _PROMOTE512(a) ((uint512_t)a)
#define _LSHIFT512(a,s) _PROMOTE512(_PROMOTE512(a)<<s)
#define _RSHIFT512(a,s) _PROMOTE512(_PROMOTE512(a)>>s)
#define _UINT64_TO_512(a,b,c,d, e,f,g,h) ( \
_LSHIFT512(a, 64*7) | \
_LSHIFT512(b, 64*6) | \
_LSHIFT512(c, 64*5) | \
_LSHIFT512(d, 64*4) | \
_LSHIFT512(e, 64*3) | \
_LSHIFT512(f, 64*2) | \
_LSHIFT512(g, 64*1) | \
_LSHIFT512(h, 64*0) )

#if defined(_MSC_VER)
#define U512_INLINE __inline
#elif defined(__GNUC__) && !defined(__STRICT_ANSI__)
#define U512_INLINE inline
#elif defined(__GNUC__)
#define U512_INLINE __inline__
#else
#define U512_INLINE
#endif


static constexpr uint512_t uint512(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g, uint64_t h){
	return _UINT64_TO_512(a,b,c,d,e,f,g,h);
}

static constexpr uint512_t uint512(uint64_t n){
	return _PROMOTE512(n);
}

typedef union uint512_u{
	uint512_t u512;
	uint64_t u64[8];
	uint8_t u8[64];
} uint512_u;

bool uint512_littleendian();

U512_INLINE uint512_u uint512_u_be(uint512_t u512){
	uint512_u value_be;
	if(uint512_littleendian()){
		uint512_u value_le;
		value_le.u512 = u512;
		for(int i=0;i<64;i++) value_be.u8[i] = value_le.u8[63-i];
	}else{
		value_be.u512 = u512;
	}
	return value_be;
}

void uint512_debug_(const char* prefix, const uint512_t& u512);

#ifdef NDEBUG
#define uint512_debug(a,b) ;
#else
#define uint512_debug(a,b) uint512_debug_(a,b);
#endif


}
