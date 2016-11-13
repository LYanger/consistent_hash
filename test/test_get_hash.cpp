#include <iostream>

#include "virtual_node.h"
#include "entity_node.h"

using namespace std;
using namespace lb;

int main()
{
	entity_node<int> enode1;
	entity_node<int> enode2;
	virtual_node<int> *vn1 = new virtual_node<int>(enode1);
	virtual_node<int> *vn2 = new virtual_node<int>(enode2);
	long hash1 = 99999999;
	long hash2 = 88888888;
	vn1->set_hash(hash1);
	vn2->set_hash(hash2);

	//assert(vn1 > vn2);
	if(vn1 > vn2){
		std::cout<<"yes"<<endl;
	}

	return 0;
}
