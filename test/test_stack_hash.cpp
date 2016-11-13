#include "md5.h"

#include <iostream>

using namespace std;
using namespace lb;

int main()
{
	const char* msg = "192.168.0.112345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364656667";

	const char* txt = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	//md5_hash mhash;
	//mhash.calc_md5_hash(msg);

	char digest[17];
	md5 md(msg);
	md.digest(digest);
	cout<<digest<<endl;

	return 0;
}
