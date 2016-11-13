#include <iostream>
using namespace std;

void fun(const string& sz)
{
	cout<<sz.length()<<endl;
}

void fun(const char* str)
{
	fun(string(str));
}

int main()
{
	const char* str = "hello";

	fun(str);

	return 0;
}
