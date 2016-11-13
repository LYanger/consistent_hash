#include <iostream>
#include <stdio.h>
using namespace std;

int main()
{
	char buffer[100];

	int a = 5;
	sprintf(buffer, "%d", a);
	cout<<buffer<<endl;

	int c = 9;
	sprintf(buffer, "%d", c);
	cout<<buffer<<endl;

	return 0;
}
