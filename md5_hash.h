#ifndef _MD5_HASH_H
#define _MD5_HASH_H

#include "md5.h"

#include <string>

namespace lb
{

struct md5_hash {
	long calc_md5_hash(const std::string& sz);
	long calc_md5_hash(const char* str);
};

}
#endif
