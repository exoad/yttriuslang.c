// fnv512, a python library for 512-bit versions of the fnv-0/fnv-1/fnv-1a hashes
// Copyright (C) 2021 tweqx
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once
#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#else
#define EMSCRIPTEN_KEEPALIVE
#endif


//#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/atomic/detail/bitwise_cast.hpp>
#include "uint512_llvm.hpp"
#include <iostream>

//using namespace boost::multiprecision;
//using namespace boost::multiprecision::literals;
//using namespace boost::atomics::detail;

#define FNV_VARIANT_0 0
#define FNV_VARIANT_1 1
#define FNV_VARIANT_1A 2

#define FNV_VARIANT_MIN 0
#define FNV_VARIANT_MAX 2
#define FNV_VARIANT_DEFAULT 2


typedef struct fnv_context{
	int variant;
	int digest_bits;
	int digest_bytes;
	uint512_t hash;
} fnv_context;

extern "C" {
EMSCRIPTEN_KEEPALIVE
int version();

EMSCRIPTEN_KEEPALIVE
char* create_buffer(size_t size);

EMSCRIPTEN_KEEPALIVE
void destroy_buffer(void* buffer);

EMSCRIPTEN_KEEPALIVE
fnv_context* fnv512_init(int variant);

EMSCRIPTEN_KEEPALIVE
void fnv512_cleanup(fnv_context* ctx);

EMSCRIPTEN_KEEPALIVE
void fnv512_update(fnv_context* ctx, const char* data, size_t len);

EMSCRIPTEN_KEEPALIVE
void fnv512_finalHex(fnv_context* ctx, char* hexdigest);

EMSCRIPTEN_KEEPALIVE
void fnv512_final(fnv_context* ctx, char* digest);


};
