#include "md5.h"

using namespace lb;

md5::md5(const std::string& sz)
{
	init();
	update(sz.c_str(), sz.length());
	finalize();
}

void md5::init()
{
	finalized_ = false;

	memset(count_, 0, sizeof(count_));
	memset(digest_, '\0', sizeof(digest_));

	state_[0] = 0x67452301;                     //they were saved as 01234567 89abcdef fedcba98 76543210 in memory
	state_[1] = 0xefcdab89;
	state_[2] = 0x98badcfe;
	state_[3] = 0x10325476;
}

void md5::update(const u_char* input, size_t length)
{
	size_t index = (count_[0] >> 3) & 0x3f;    //count the index by mod 64
	
	count_[0] += length << 3;                  //accumulative length, using old_len + length
	if(count_[0] < (length << 3))              //carry
		++count_[1];
	count_[1] += length >> 29;                 //avoid the loss of overflow

	size_t partlen = BLOCK_SIZE - index;            //the number of bytes we need to fill

	size_t i = 0;

	if(length >= partlen){
		memcpy(buffer_+index, input, partlen);
		
		transform(buffer_);

		for(int i=partlen; i+BLOCK_SIZE <= length; i+=BLOCK_SIZE)     //handle the chunks 
			transform(input+i);
		
		index = 0;     //need to be zero
	}

	memcpy(buffer_+index, input+i, length-i);      //maybe i is zero
}

void md5::update(const char* input, size_t length)
{
	update(reinterpret_cast<const u_char*>(input), length);
}

void md5::finalize()
{
	static const u_char padding[64] = {
										0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
										   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
									       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
									  };
	if(!finalized_){
		u_char bits[8];
		encode(bits, count_, 8);       //encode the total length

		size_t index = (count_[0] >> 3) & 0x3f;
		size_t pad_len = index < 56 ? (56 - index) : (120 - index);   //reserve 8 bits for length

		update(padding, pad_len);
		update(bits, 8);

		encode(digest_, state_, DIGEST_SIZE);   //also 16 * 8 = 128

		memset(buffer_, 0, sizeof(buffer_));
		memset(count_, 0, sizeof(count_));

		finalized_ = true;
	}
}

void md5::digest(char *buf) const 
{
	assert(finalized_);
	assert(buf != NULL);
#ifdef _USE_HEX_
	for(int i=0; i<16; ++i)
		sprintf(buf+i*2, "%02x", digest_[i]);   //still 16 * 8 = 128, hex = 4 + 4 = 8bits, such as 7E 03 ...
	buf[32] = '\0';
	std::cout<<buf<<std::endl;
#else
	//strncpy((char *)(buf), (char*)(digest_), strlen(digest_)+1);     //16 * 8 = 128
	std::cout<<sizeof(digest_)<<std::endl;
	for(int i=0; i<16; ++i)
		buf[i] = digest_[i];
	buf[16] = '\0';
#endif
}

void md5::transform(const u_char block[BLOCK_SIZE])
{
	u_int a = state_[0], b = state_[1], c = state_[2], d = state_[3];
	u_int x[16];        //16 * 8 = 128
	decode(x, block, BLOCK_SIZE);

	FF (a, b, c, d, x[ 0], S11, 0xd76aa478);   //round 1, 1
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756);
	FF (c, d, a, b, x[ 2], S13, 0x242070db); 
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); 
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf);
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); 
	FF (c, d, a, b, x[ 6], S13, 0xa8304613);
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); 
	FF (a, b, c, d, x[ 8], S11, 0x698098d8);
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); 
	FF (c, d, a, b, x[10], S13, 0xffff5bb1);
	FF (b, c, d, a, x[11], S14, 0x895cd7be); 
	FF (a, b, c, d, x[12], S11, 0x6b901122);
	FF (d, a, b, c, x[13], S12, 0xfd987193); 
	FF (c, d, a, b, x[14], S13, 0xa679438e); 
	FF (b, c, d, a, x[15], S14, 0x49b40821); 
	
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562);   //round 2, 17
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); 
	GG (c, d, a, b, x[11], S23, 0x265e5a51); 
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); 
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); 
	GG (d, a, b, c, x[10], S22,  0x2441453); 
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); 
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); 
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); 
	GG (d, a, b, c, x[14], S22, 0xc33707d6); 
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); 
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); 
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); 
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); 
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); 
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); 

	HH (a, b, c, d, x[ 5], S31, 0xfffa3942);   //round 3, 33
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); 
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); 
	HH (b, c, d, a, x[14], S34, 0xfde5380c); 
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44);
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); 
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); 
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); 
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); 
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); 
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); 
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); 
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); 
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); 
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); 
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); 

	II (a, b, c, d, x[ 0], S41, 0xf4292244);    //round 4 , 49
	II (d, a, b, c, x[ 7], S42, 0x432aff97); 
	II (c, d, a, b, x[14], S43, 0xab9423a7); 
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); 
	II (a, b, c, d, x[12], S41, 0x655b59c3); 
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); 
	II (c, d, a, b, x[10], S43, 0xffeff47d); 
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); 
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); 
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); 
	II (c, d, a, b, x[ 6], S43, 0xa3014314); 
	II (b, c, d, a, x[13], S44, 0x4e0811a1); 
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); 
	II (d, a, b, c, x[11], S42, 0xbd3af235); 
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); 
	II (b, c, d, a, x[ 9], S44, 0xeb86d391);     //64 

	state_[0] += a;       //finally four state into the result string by encode
	state_[1] += b;               
	state_[2] += c;
	state_[3] += d;

	memset(x, 0, sizeof(x));
}

void md5::encode(u_char *output, const u_int *input, size_t len)  //u_int to u_char, 1 u_int = 4 u_char
{
	size_t i=0, j=0;
	while(j < len){
		output[j] = input[i] & 0xff;
		output[j+1] = (input[i] >> 8) & 0xff;
		output[j+2] = (input[i] >> 16) & 0xff;
		output[j+3] = (input[i] >> 24) & 0xff;
		++i;
		j += 4;
	}
}

void md5::decode(u_int *output, const u_char *input, size_t len)
{
	size_t i=0, j=0;
	while(j < len){
		output[i] = input[j] |
					(input[j+1] << 8) |
					(input[j+2] << 16) |
					(input[j+3] << 24);
		++i;
		j += 4;
	}
}
