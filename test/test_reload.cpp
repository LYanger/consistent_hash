#include <iostream>
#include <string>

void fun(std::string sz)
{
	std::cout<<"fun2"<<std::endl;
}

void fun(const char* str)
{
	std::cout<<"fun1"<<std::endl;
	fun(std::string(str));     //if you reload a funciton, ensure that function is ahead of the call function
}

int main()
{
	const char* str = "hello";

	fun(str);

	const char* str1 = "wocao";

	fun(str1);

	return 0;
}
