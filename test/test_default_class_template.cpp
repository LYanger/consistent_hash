#include <iostream>

using namespace std;

class A {
public:
	void print() {cout<<"hello A"<<endl;}
};

class C {
public:
	void print() {cout<<"hello C"<<endl;}
};

template <typename CLASS=A>
class B{
public:
	void call(){
		ass.print();
	}
private:
	CLASS ass;
};

int main()
{
	B<C> b;
	b.call();

	return 0;
}
