#include <iostream>
using namespace std;

class myclass {
//	friend class tem_class<T>;
public:
	myclass() : data(0), hash(0)
	{}
public:
	void show() const {
		cout<<data<<endl;
		cout<<hash<<endl;
	}
	void get_value() const {
		cout<<"get value"<<endl;
		cout<<data<<' '<<hash<<endl;
	}
public:
	int  data;
	long hash;
};

template <typename T>
class tem_class {
public:
	tem_class()
		: obj(T())
	{}
	void print() const{
		cout<<"print"<<endl;
		//cout<<T::data<<endl;
		T tmp;
		tmp.show();
		tmp.get_value();
	}
private:
	T obj;
};

int main()
{
	tem_class<myclass> tc;
	tc.print();

	return 0;
}
