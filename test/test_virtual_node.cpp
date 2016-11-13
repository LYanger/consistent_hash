#include "virtual_node.h"

#include <iostream>
using namespace std;

int main()
{	
	entity_node<int> enode("machine a", 50, 1);

	virtual_node<int> vnode(enode);

	vnode.set_hash(1);
	cout<<vnode.get_hash()<<endl;

	entity_node<int> enode("machine b", 30, 2);
	vnode.set_entity_node(enode);

	return 0;
}
