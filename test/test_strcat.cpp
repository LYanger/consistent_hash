#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

int main()
{
	char str[30] = "hello";
	char num[10];
	for(int i=0; i<10; ++i){
		//itoa(i, num, 10);
		sprintf(num, "%d", i);
		strcat(str, num);
		cout<<str<<endl;
	}
	return 0;
}
