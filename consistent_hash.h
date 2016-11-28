#ifndef _CONSISTENT_HASH_H
#define _CONSISTENT_HASH_H

#include "lb_util.h"
#include "md5_hash.h"
#include "rb_tree.h"
#include "entity_node.h"
#include "virtual_node.h"

namespace lb
{

static const int MESSAGE_SIZE = 1000;
static const int MAX_NUM_SIZE = 10;

template <typename T, typename HASH=md5_hash>   //T is the entity_node data, HASH is the method of hash
class consistent_hash {
public:
	consistent_hash(const HASH& hash_obj);
	~consistent_hash();
public:
	void set_hash_class(const HASH& hash_obj);
	entity_node<T>* lookup_node(const char* object);

	void add_node(const entity_node<T>& node);
	void del_node(const entity_node<T>& node);
	long get_vnodes_amount() const;
private:
	int                         vnodes_amount_;      //the number of virtual number
	HASH                        hash_obj_;
	rb_tree<virtual_node<T> >*  vnode_tree_;
};

template <typename T, typename HASH>
consistent_hash<T,HASH>::consistent_hash(const HASH& hash_obj)
	:vnodes_amount_(0), hash_obj_(hash_obj), vnode_tree_(new rb_tree<virtual_node<T> >())
{
}

template <typename T, typename HASH>
consistent_hash<T, HASH>::~consistent_hash()
{
	vnode_tree_->preorder_traverse();
	std::cout<<std::endl;
	vnode_tree_->inorder_traverse();
	std::cout<<std::endl;
	delete vnode_tree_;	
}

template <typename T, typename HASH>
void consistent_hash<T, HASH>::set_hash_class(const HASH& hash_obj)
{
	hash_obj_ = hash_obj;
}

template <typename T, typename HASH>
entity_node<T>* consistent_hash<T, HASH>::lookup_node(const char* object)
{
	if(object == NULL || vnodes_amount_ <= 0)
		return NULL;
	
	long hash = hash_obj_.calc_md5_hash(object);
	virtual_node<T> virt_node;
	virt_node.set_hash(hash);
#ifdef _DEBUG_
	fprintf(stdout, "client hash:> %ld\n", hash);
#endif

	rb_tree_node<virtual_node<T> >* rb_node_ptr;
	if((rb_node_ptr = vnode_tree_->lookup(virt_node)) == NULL){
		fprintf(stdout, "lookup %s failed.\n", object);
		return NULL;
	}
	else{
#ifdef _DEBUG_
		fprintf(stdout, "machine assign:> %ld\n", (rb_node_ptr->get_data()).get_hash());
#endif
		return (rb_node_ptr->get_data()).get_entity_node();
	}

}


template <typename T, typename HASH>
void consistent_hash<T, HASH>::add_node(const entity_node<T>& node)
{
	int num_vnodes = node.get_num_vnodes();
	char str[MESSAGE_SIZE] = {'\0'};;
	char num[MAX_NUM_SIZE] ={'\0'};
	strcpy(str, node.get_identity());
	
	for(int i=1; i<=num_vnodes; ++i){
		int ret = sprintf(num, "%d", i);
		assert(ret >= 0);
		strcat(str, num);
		std::cout<<str<<std::endl;
		long hash = hash_obj_.calc_md5_hash(str);
	
		virtual_node<T> virt_node(node);    //create n virtual nodes
		virt_node.set_hash(hash);
		virt_node.set_entity_node(node);                
		
		if(!vnode_tree_->insert(virt_node))
			fprintf(stdout, "the num : %d virtual node insert failed\n", i);
		else{
			++vnodes_amount_;
#ifdef _DEBUG_
			fprintf(stdout, "insert virtual node %d success.\n", i);
#endif
		}

	}
}

template <typename T, typename HASH>
void consistent_hash<T, HASH>::del_node(const entity_node<T>& node)
{
	int num_vnodes = node.get_num_vnodes();
	char str[MESSAGE_SIZE] = {'\0'};
	char num[MAX_NUM_SIZE] ={'\0'};
	strcpy(str, node.get_identity());
	
	virtual_node<T>* virt_node = NULL;
	for(int i=1; i<=num_vnodes; ++i){
		int ret = sprintf(num, "%d", i);
		assert(ret >= 0);
		strcat(str, num);
		long hash = hash_obj_.calc_md5_hash(str);
		
		virtual_node<T> virt_node(node);  
		virt_node.set_hash(hash);
		virt_node.set_entity_node(node);
		
		if(!vnode_tree_->remove(virt_node))
			fprintf(stdout, "the virtual node %d did not in the rb_tree.\n", i);
		else{
				--vnodes_amount_;
#ifdef _DEBUG_
			fprintf(stdout, "delete virtual node %d success.\n", i);
#endif
			}
	}
}


template <typename T, typename HASH>
long consistent_hash<T, HASH>::get_vnodes_amount() const
{
	return vnodes_amount_;
}

}
#endif
