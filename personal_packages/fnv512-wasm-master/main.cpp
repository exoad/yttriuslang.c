#include <iostream>
#include <cstring>
#include "fnv512.hpp"

char _nibble2hex(unsigned char nibble) {
  if (nibble < 10)
    return '0' + nibble;
  else
    return 'a' + nibble - 10;
}


void _hexDigest(char* digest, char* hexdigest){
   for(unsigned int i = 0,h=0; i < 64; i++){
        int hi = (digest[i]&0xF0) >> 4;
        int lo = digest[i]&0x0F;
        hexdigest[h++]=_nibble2hex(hi);
        hexdigest[h++]=_nibble2hex(lo);
//        std::cout<<"[main] "<<_nibble2hex(hi)<<_nibble2hex(lo)<<std::endl;
   }
}


int main(){
	const char* input = "chongo <Landon Curt Noll> /\\../\\";

	fnv_context* ctx = fnv512_init(FNV_VARIANT_0);

	fnv512_update(ctx, input, strlen(input));

	char* output = create_buffer(129);
	output[128]=0;
	fnv512_finalHex(ctx, output);
	printf("fnv0 test output hex: %s\r\n",output);

	char* output2 = create_buffer(64);
	fnv512_final(ctx,output2);
	char* output2h = create_buffer(129);
	output2h[128]=0;
	_hexDigest(output2,output2h);
	printf("fnv0 test output raw: %s\r\n",output2h);




//	fnv0(input, strlen(input), output);
//	printf("output %s",output);

	destroy_buffer(output);
	fnv512_cleanup(ctx);

	return 0;

}
