#include "md5_hash.h"

using namespace lb;

long md5_hash::calc_md5_hash(const std::string& sz)
{
	char digest[DIGEST_SIZE];

	md5 md(sz);
	md.digest(digest);
	std::cout<<strlen(digest)<<std::endl;

	long hash = 0;
	for(int i=0; i<16; i+=4){
		hash += ((long)digest[i] & 0xff)           //must to be an u_long type, if you want to convert char to u_long.
			  | ((long)(digest[i+1] & 0xff) << 8)  //we want to convert char to u_long, but char only has 8 bits
			  | ((long)(digest[i+2] & 0xff) << 16) //using (& 0xff) to obtain the 8 bits
			  | ((long)(digest[i+3] & 0xff) << 24);
	}
	return hash;
}

long md5_hash::calc_md5_hash(const char* str)
{
	assert(str != NULL);
	return calc_md5_hash(std::string(str));
}
