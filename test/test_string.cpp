#include <iostream>

#include <string>

unsigned long fun(const char* str)
{
	//return fun2(std::string(str));
}

unsigned long fun2(std::string& sz)
{
	//do something
}


int main()
{
	std::string str;	

	fun(str);

	return 0;
}
