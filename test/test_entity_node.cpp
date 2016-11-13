#include "entity_node.h"

#include <iostream>
using namespace std;

int main()
{
	int i = 1;
	entity_node<int> enode("machine a", 40, i);

	cout<<enode.get_identity()<<endl;
	cout<<enode.get_num_vnodes()<<endl;

	cout<<enode.get_data()<<endl;
	
	int j = 2;
	enode.set_data(2);
	cout<<enode.get_data()<<endl;

	return 0;
}
