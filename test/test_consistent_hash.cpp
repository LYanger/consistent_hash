#include "consistent_hash.h"

#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace lb;

void get_ip(char * ip)
{
	int a=0, b=0 , c=0 , d=0;
	a = rand() % 256;
	b = rand() % 256;
	c = rand() % 256;
	d = rand() % 256;
	
	char aa[4],bb[4],cc[4],dd[4];
	sprintf(aa, "%d", a);
	sprintf(bb, "%d", b);
	sprintf(cc, "%d", c);
	sprintf(dd, "%d", d);
	
	strcpy(ip, aa);
	strcat(ip, ".");
	strcat(ip, bb);
	strcat(ip, ".");
	strcat(ip, cc);
	strcat(ip, ".");
	strcat(ip, dd);
}


int main()
{
	md5_hash mhash;
	consistent_hash<int> ch(mhash);
	
	entity_node<int> node1("192.168.0.1", 4, 1);
	//entity_node<int> node2("192.168.0.2", 80, 2);
	//entity_node<int> node3("192.168.0.3", 80, 3);
	//entity_node<int> node4("192.168.0.4", 80, 4);

	ch.add_node(node1);
	//ch.add_node(node2);
	//ch.add_node(node3);
	//ch.add_node(node4);
	
	//ch.vnode_tree_->inorder_traverse();;
	//cout<<endl;
/*
	int ans[4] = {0};
	char ip[100];
	for(int i=0; i<10; ++i){
		get_ip(ip);
		entity_node<int>* node = ch.lookup_node(ip);
		if(node != NULL){
			cout<<"client "<<ip<<" assign on "<<node->get_identity()<<" , the data is:"<<node->get_data()<<endl;
			if(strcmp(node->get_identity(),"192.168.0.1")==0) ans[0]++;
			if(strcmp(node->get_identity(),"192.168.0.2")==0) ans[1]++;
			if(strcmp(node->get_identity(),"192.168.0.3")==0) ans[2]++;
			if(strcmp(node->get_identity(),"192.168.0.4")==0) ans[3]++;
		}
		//ch.del_node(node2);
	}
	

	//cout<<ch.get_vnodes_amount()<<endl;
	//ch.del_node(node1);
	//ch.del_node(node2);
//	ch.vnode_tree_->inorder_traverse();;

	cout<<"total test cases :" <<ans[0]+ans[1]+ans[2]+ans[3]<<endl;
	cout<<"map to machine a :" <<ans[0]<<endl;
	cout<<"map to machine b :" <<ans[1]<<endl;
	cout<<"map to machine c :" <<ans[2]<<endl;
	cout<<"map to machine d :" <<ans[3]<<endl;
*/

	return 0;
}
