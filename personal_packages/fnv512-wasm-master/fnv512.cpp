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

#include "fnv512.hpp"


constexpr uint512_t fnv_prime = uint512(
	0x0000000000000000ull, 0x0000000000000000ull,
	0x0000000001000000ull, 0x0000000000000000ull,
	0x0000000000000000ull, 0x0000000000000000ull,
	0x0000000000000000ull, 0x0000000000000157ull
);

constexpr uint512_t fnv_offset_basis = uint512(
	0xB86DB0B1171F4416ull, 0xDCA1E50F309990ACull,
	0xAC87D059C9000000ull, 0x0000000000000D21ull,
	0xE948F68A34C192F6ull, 0x2EA79BC942DBE7CEull,
	0x182036415F56E34Bull, 0xAC982AAC4AFE9FD9ull
);


int version(){
	return 20210224;
}


char* create_buffer(size_t size){
	return (char*) malloc(size);
}
void destroy_buffer(void* buffer){
	free(buffer);
}

char nibble2hex(unsigned char nibble) {
  if (nibble < 10)
    return '0' + nibble;
  else
    return 'a' + nibble - 10;
}



fnv_context* fnv512_init(int variant){

   uint512_debug("init fnv_prime", fnv_prime);
   uint512_debug("init fnv_offset_basis", fnv_offset_basis);


	fnv_context* ctx = (fnv_context*) create_buffer(sizeof(fnv_context));
	ctx->digest_bits = 512;
	ctx->digest_bytes = 64;

	if(variant<FNV_VARIANT_MIN || variant>FNV_VARIANT_MAX) variant = FNV_VARIANT_DEFAULT;
	ctx->variant = variant;

	switch(variant){
		case FNV_VARIANT_0:
			ctx->hash = 0;
			break;
		case FNV_VARIANT_1:
		case FNV_VARIANT_1A:
		default:
  			ctx->hash = fnv_offset_basis;
			break;
	}

   uint512_debug("init hash", ctx->hash);

	return ctx;
}

void fnv512_cleanup(fnv_context* ctx){
	destroy_buffer(ctx);
}

void fnv512_update(fnv_context* ctx, const char* data, size_t len){
  if(ctx->variant==FNV_VARIANT_1A){
	  for (size_t i = 0; i < len; i++) {
  	  // xor with a byte of data
    	ctx->hash ^= (uint512_t) data[i];

    	// multiplication by 2^344 + 2^8 + 0x57
    	ctx->hash *= fnv_prime;
  	}
  }else{
  	for (size_t i = 0; i < len; i++) {
    	// multiplication by 2^344 + 2^8 + 0x57
    	ctx->hash *= fnv_prime;

    	// xor with a byte of data
    	ctx->hash ^= (uint512_t) data[i];
  	}
  }
}


//author: Niels Keurentjes
//https://stackoverflow.com/questions/17261798/converting-a-hex-string-to-a-byte-array
int char2int(const char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  throw std::invalid_argument("Invalid input string");
}
void hex2bin(const char* src, char* target)
{
  while(*src && src[1])
  {
    *(target++) = char2int(*src)*16 + char2int(src[1]);
    src += 2;
  }
}

void fnv512_final(fnv_context* ctx, char* digest){
   uint512_u hash_be = uint512_u_be(ctx->hash);
   uint512_debug("final hash be", hash_be.u512);
   for(unsigned int i=0; i< ctx->digest_bytes; i++) digest[i]=hash_be.u8[i];
}
void fnv512_finalHex(fnv_context* ctx, char* hexdigest){
   uint512_u hash_be = uint512_u_be(ctx->hash);
   uint512_debug("final hash be", hash_be.u512);

   for(unsigned int i = 0,h=0; i < ctx->digest_bytes; i++){
	int hi = (hash_be.u8[i]&0xF0) >> 4;
	int lo = hash_be.u8[i]&0x0F;
        hexdigest[h++]=nibble2hex(hi);
	hexdigest[h++]=nibble2hex(lo);
   }
}


