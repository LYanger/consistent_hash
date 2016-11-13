#include "rb_tree.h"

#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace lb;

typedef unsigned long u_long;

//void compare(

int main()
{
	rb_tree<long> rbt;

///////////////////////////////////////////////////////////////////
// s color is BLACK
/*
	//test for rotate right
	rbt.insert(10);
	rbt.insert(8);
	rbt.insert(5);
*/

/*
	//test for rotate left_right
	rbt.insert(10);
	rbt.insert(7);
	rbt.insert(9);
*/
////////////////////////////////////////////////////////////////////
//s color is RED     //no rotate !!!!
/*
	//test for left_right
	rbt.insert(10);
	rbt.insert(11);
	rbt.insert(6);
	rbt.insert(9);
*/
/*
	//test for right
	rbt.insert(10);
	rbt.insert(11);
	rbt.insert(9);
	rbt.insert(7);
*/

//random test
	const int N = 11;
	//long array[N] = {383, 886, 777, 915, 793, 335, 386, 492, 649, 421, 362};
	u_long array[N];


	for(int i=0; i<N; ++i)
		*(array+i) = random() % 100000;
		

	for(int i=0; i<N; ++i)
		rbt.insert(*(array+i));

//	rbt.inorder_traverse();
	std::cout<<std::endl;

	//cout<<rbt.lookup(23)<<endl;

	//test for locate

	for(int i=0; i<N; ++i){
		//sleep(1);
		rbt.remove(*(array+i));
		//rbt.inorder_traverse();
	}
	cout<<endl;

//	rbt.inorder_traverse();
	cout<<"over"<<endl;

/*
	rbt.remove(3);
	rbt.inorder_traverse();
*/
	return 0;
}
