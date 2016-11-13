#ifndef _MD5_H
#define _MD5_H

#include "lb_util.h"
#include "md5_config.h"

namespace lb
{

typedef unsigned int  u_int;
typedef unsigned char u_char;

static const int BUFFER_SIZE = 1000;
static const int BLOCK_SIZE = 64;      //64           //64 * sizeof(u_char) = 512;
static const int DIGEST_SIZE = 16;  //16                //16 * 8 = 128

class md5 {
public:
	md5(const std::string& sz);
	
	void update(const u_char* input, size_t length);
	void update(const char* input, size_t length);
	
	void digest(char* buf) const;
protected:
	void init();
	void finalize();
	void transform(const u_char block[BLOCK_SIZE]);
	
	void encode(u_char *output,  const u_int  *input, size_t len);
	void decode(u_int  *output, const u_char *input, size_t len);
private:
	bool   finalized_;
	u_char buffer_[BUFFER_SIZE];                 //the max storge capacity is 512
	u_char digest_[DIGEST_SIZE];                //16 * 8 = 128
	u_int  count_[2];                           //which storge the length of the string by bits
	u_int  state_[4];                           //iter state, eventually turned into results
};

inline u_int shift_left(u_int x, int n)   //ROL
{
	return (x << n) | (x >> (32 - n));
}

inline u_int F(u_int x, u_int y, u_int z)    { return (x & y) | ((~x) & z); }  
inline u_int G(u_int x, u_int y, u_int z)    { return (x & z) | (y & (~z)); }
inline u_int H(u_int x, u_int y, u_int z)    { return (x ^ y ^ z);          }
inline u_int I(u_int x, u_int y, u_int z)    { return (y ^ (x | ~z));     }

inline void FF(u_int &a, u_int b, u_int c, u_int d, u_int x, u_int s, u_int ac)
{
	a = shift_left(a + F(b, c, d) + x + ac, s) + b;	
}

inline void GG(u_int &a, u_int b, u_int c, u_int d, u_int x, u_int s, u_int ac)
{
	a = shift_left(a + G(b, c, d) + x + ac, s) + b;
}

inline void HH(u_int &a, u_int b, u_int c, u_int d, u_int x, u_int s, u_int ac)
{
	a = shift_left(a + H(b, c, d) + x + ac, s) + b;
}

inline void II(u_int &a, u_int b, u_int c, u_int d, u_int x, u_int s, u_int ac)
{
	a = shift_left(a + I(b, c, d) + x + ac, s) + b;
}

}
#endif
