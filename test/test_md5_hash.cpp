#include "md5_hash.h"

#include <string>

using namespace lb;

typedef unsigned long u_long;

int main()
{
	md5_hash mh;

	//std::string sz("admin");

	//u_long hash = mh.calc_md5_hash(sz);

	//std::cout<<hash<<std::endl;

	const char* str = "wocao";
	u_long hash1 = mh.calc_md5_hash(str);

	std::cout<<hash1<<std::endl;

	return 0;
}
