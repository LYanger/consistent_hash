#include "md5.h"

#include <iostream>
using namespace std;

int main()
{
	//const char* str = "admin";
	md5 m("123456");
	char buf[33];
	m.digest(buf);

	cout<<buf<<endl;

	return 0;
}
