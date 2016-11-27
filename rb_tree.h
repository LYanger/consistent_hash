#ifndef _RB_TREE_H
#define _RB_TREE_H

#include <stdio.h>
#define _DEBUG_

#include "lb_util.h"

namespace lb
{

typedef enum {RED, BLACK}color_type;

template <typename T>
class rb_tree;

template <typename T>
class rb_tree_node{
	friend class rb_tree<T>;	
public:
	explicit rb_tree_node(T data=T(), color_type color=RED, rb_tree_node* left=NULL,
					      rb_tree_node* right=NULL, rb_tree_node* parent=NULL) 
		: data_(data), color_(color), left_(left), right_(right), parent_(parent)
	{}
	~rb_tree_node(){
		left_ = right_ = parent_ = NULL;
	}
public:
	const T& get_data() const; 
private:
	T             data_;
	color_type    color_;
	rb_tree_node* left_; 
	rb_tree_node* right_; 
	rb_tree_node* parent_; 
};

template <typename T>
class rb_tree {
	typedef rb_tree_node<T> node_type;
public:
	rb_tree(){
		nil_ = new(std::nothrow) node_type(T(), BLACK);      //for any empty node 
		assert(nil_ != NULL);
		nil_->left_ = nil_;
		nil_->right_ = nil_;
		nil_->parent_ = nil_;
		root_ = nil_;
	}
	~rb_tree(){
		destroy(root_);	
		delete nil_;
		root_ = NULL;
		nil_ = NULL;
	}
public:
#ifdef _DEBUG_
	void inorder_traverse() const{
		inorder_traverse(root_);
	}
	void preorder_traverse() const {
		preorder_traverse(root_);
	}
	void inorder_traverse(node_type* const &) const;
	void preorder_traverse(node_type* const &) const;
#endif
	bool insert(const T& key);
	bool remove(const T& key);
	
	node_type* lookup(const T& key) const;
	//void set_compare_method(void (*compare)(const void*, const void*));
private:
	void insert_fixup(node_type *x);
	void remove_fixup(node_type*& x);
	void rotate_left(node_type* p);
	void rotate_right(node_type *p);
	void destroy(node_type*& t);

	node_type* locate(const T& key) const;
	node_type* locate_detail(node_type*&, const T& key) const;
	node_type* min() const;
	node_type* apply_node(const T& val=T(), color_type color = RED);
private:
	//void (*compare_)(const void *, const void*);
	node_type* nil_;
	node_type* root_;
};

/*
void rb_tree::set_compare_method(void (*compare)(const void*, const void*))
{	
	assert(compare != NULL);
	compare_ = compare;
}
*/

template <typename T>
const T& rb_tree_node<T>::get_data() const
{
	return data_;
}

template <typename T>
void rb_tree<T>::destroy(node_type *&t)
{
/*
	update:2016.11.27
	content: delelte the fllowing two sentences which generate memory leak;
*/
	if(t != nil_){
		//if(t->left_ != nil_)    //error
		destroy(t->left_); 
		//else if(t->right_ != nil_)  //error
		destroy(t->right_);
		delete(t);
	}
}

template <typename T>
rb_tree_node<T>* rb_tree<T>::apply_node(const T& val, color_type color)
{
	node_type* tmp = new(std::nothrow) node_type(val, RED, nil_, nil_, nil_);    //no throw
	assert(tmp != NULL);
	return tmp;
}

template <typename T>
bool rb_tree<T>::insert(const T& key)
{
#ifdef _DEBUG_
#ifdef _USE_VIRTUAL_NODE_
	fprintf(stdout, "insert:> data_->hash_=%ld \n", key.get_hash());
#else
	fprintf(stdout, "insert %ld\n", (long)key);
#endif

#endif
	node_type *iter = root_;
	node_type *parent = nil_;
	while(iter != nil_){
		parent = iter;
		if(key < iter->data_)
			iter = iter->left_;
		else if(key > iter->data_)
			iter = iter->right_;
		else
			return false;
	}
	
	iter = apply_node(key);
	iter->parent_ = parent;
	
	if(parent == nil_){              //iter->parent == nil, shows this is the root_ node
		iter->color_ = BLACK;       //it's worth notice that we put parent == nil_ first
		root_ = iter;               //will not affect the efficiency, because the nil_->data_ = 0,
		return true;                //so that we can not compare > or < first;
	}                               //weeyanghuang@gmail.com
	else if(parent->data_ > key)
		parent->left_ = iter;
	else
		parent->right_ = iter;

	if(iter->parent_->color_ == BLACK)
		return true;
	else
		insert_fixup(iter);
	
	return true;
}

template <typename T>
void rb_tree<T>::insert_fixup(node_type* x) //x:insert node, p:x->parent pp:x->parent->parent s:uncle node
{
	assert(x != nil_);

	while(x != root_ && x->parent_->color_ == RED){     //RED-RED
		if(x->parent_ == x->parent_->parent_->left_){     //the father is grandfather's left child
			node_type* s = x->parent_->parent_->right_;
			if(s && s->color_ == RED){   //s->color == RED
				s->color_ = BLACK;
				x->parent_->color_ = BLACK;
				x->parent_->parent_->color_ = RED;   //modify two BLACK one red
				x = x->parent_->parent_;
			}
			else{                     //s->color == BLACK
				if(x == x->parent_->right_){
					x = x->parent_;
					rotate_left(x);
				}
				x->parent_->color_ = BLACK;
				x->parent_->parent_->color_ = RED;
				rotate_right(x->parent_->parent_);	
			}	
		}
		else{                                             //the father is grandfather's right child
			node_type* s = x->parent_->parent_->left_;
			if(s && s->color_ == RED){
				s->color_ = BLACK;
				x->parent_->color_ = BLACK;
				x->parent_->parent_->color_ = RED;
				x = x->parent_->parent_;
			}
			else{
				if(x == x->parent_->left_){
					x = x->parent_;
					rotate_right(x);
				}
				x->parent_->color_ = BLACK;
				x->parent_->parent_->color_ = RED;
				rotate_left(x->parent_->parent_);
			}
		}
	}
	root_->color_ = BLACK;
}

template <typename T>
void rb_tree<T>::rotate_right(node_type* x)  //x is the rotate point
{
	assert(x != nil_);

	node_type *y = x->left_;
	x->left_ = y->right_;
	if(y->right_ != nil_)
		y->right_->parent_ = x;
	y->parent_ = x->parent_;       //make self point to father

	if(x == root_)
		root_ = y;
	else if(x == x->parent_->left_)    //make the father point to self
		x->parent_->left_ = y;
	else
		x->parent_->right_ = y;
	
	y->right_ = x;
	x->parent_ = y;
}

template <typename T>
void rb_tree<T>::rotate_left(node_type* x)   //x is the rotate point
{
	assert(x != nil_);
	
	node_type *y = x->right_;
	x->right_ = y->left_;
	if(y->left_ != nil_)
		y->left_->parent_ = x;
	y->parent_ = x->parent_;   //notice who's father changed

	if(x == root_)
		root_ = y;
	else if(x == x->parent_->left_)
		x->parent_->left_ = y;
	else
		x->parent_->right_ = y;
	
	y->left_ = x;
	x->parent_ = y;
}

template <typename T>
rb_tree_node<T>* rb_tree<T>::locate(const T& key) const
{
	if(root_ == nil_)
		return nil_;
	if(root_->data_ == key)
		return root_;
	
	node_type* t = root_;
	return locate_detail(t, key);
}

template <typename T>
rb_tree_node<T>* rb_tree<T>::locate_detail(node_type*& t, const T& key) const
{
	if(t == nil_)
		return nil_;
	if(t->data_ == key)
		return t;

	node_type *found = nil_;
	if(key < t->data_)
		found = locate_detail(t->left_, key);
	return (found == nil_ ? locate_detail(t->right_, key) : found); 
}


template <typename T>
rb_tree_node<T>* rb_tree<T>::lookup(const T& key) const
{
	if(root_ == nil_)
		return NULL;
	
	node_type* iter = root_;
	node_type* node = nil_;
	while(iter != nil_){
		if(key < iter->data_){
			node = iter;         //update node
			iter = iter->left_;
		}
		else if(key >= iter->data_)
			iter = iter->right_;
	}

	return (node != nil_) ? node : min();
}

template <typename T>
rb_tree_node<T>* rb_tree<T>::min() const
{
	if(root_ == nil_)
		return NULL;
	
	node_type* iter = root_;
	while(iter->left_ != nil_)
		iter = iter->left_;
	return iter;
}

template <typename T>
bool rb_tree<T>::remove(const T& key)   //z:delete   y:delete actually   x:y->child    x_parent:x->parent
{
	node_type* z = locate(key);
	if(z == nil_)
		return false;

	node_type* y = z;
	node_type* x = nil_;

	if(y->left_ == nil_)
		x = y->right_;
	else{
		if(y->right_ == nil_)
			x = y->left_;
		else{
			y = z->right_;
			while(y->left_ != nil_)
				y = y->left_;
			x = y->right_;
			x->parent_ = y;     //weeyanghuang@gmail.com, modify at 2016.11.3, add this statement
		}
	}

	if(y != z){
		y->left_ = z->left_;
		z->left_->parent_ = y;

		if(y != z->right_){
			x->parent_ = y->parent_;
			y->parent_->left_ = x;
			y->right_ = z->right_;
			z->right_->parent_ = y;
		}

		y->parent_ = z->parent_;
		if(root_ == z)
			root_ = y;
		else if(z == z->parent_->left_)
			z->parent_->left_ = y;
		else
			z->parent_->right_ = y;

		std::swap(z->color_, y->color_);     //don't forget swap color, move the un_balance to actually deleted
		y = z;    							 //y now points to node to be actually deleted
	}
	else{
		x->parent_ = z->parent_;

		if(root_ == z)
			root_ = x;
		else if(z == z->parent_->left_)
			z->parent_->left_ = x;
		else
			z->parent_->right_ = x;
	}

	if(y->color_ != RED)
		remove_fixup(x);

	delete(z);
#ifdef _DEBUG_
	fprintf(stdout, "remove:> data_.hash_=%ld \n", key.get_hash());
#endif
	return true;
}

template <typename T>
void rb_tree<T>::remove_fixup(node_type*& x)
{
	node_type* x_parent = x->parent_;
	while(x != root_ && (x == nil_ || x->color_ == BLACK)){   //BLACK-BLACK
		if(x == x->parent_->left_){
			node_type* s = x_parent->right_;
			if(s->color_ == RED){               //case 1 , red uncle
				x_parent->color_ = RED;         //case 1
				s->color_ = BLACK;              //case 1
				rotate_left(x_parent);          //case 1
				s = x_parent->right_;           //case 1
			}
			else{
				if( (s->left_ == nil_ || s->left_->color_ == BLACK)          //case 2
				    && (s->right_ == nil_ || s->right_->color_ == BLACK)){   //case 2
					s->color_ = RED;                                        //case 2   
					x = x_parent;              //case 2 ,if x_parent->color_ == RED, break, make it black after while
					x_parent = x_parent->parent_;                           //case 2
				}
				else{
					if(s->right_ == nil_ || s->right_->color_ == BLACK){    //case 3
						s->left_->color_ = BLACK;   //case 3, s->left_ might be nil_
						s->color_ = RED;            //case 3
						rotate_right(s);            //case 3
						s = x_parent->right_;       //case 3
					}
					s->color_ = x_parent->color_;   //case 4, get father's color
					x_parent->color_ = BLACK;       //case 4
					s->right_->color_ = BLACK;      //case 4, s->right_ might be nil_
					rotate_left(x_parent);          //case 4
					break;
				}
			}
		}
		else{                  //the following is the mirror
			node_type* s = x_parent->left_;
			if(s->color_ == RED){
				x_parent->color_ = RED;
				s->color_ = BLACK;
				rotate_right(x_parent);
				s = x_parent->left_;
			}
			else{
				if( (s->right_ == nil_ || s->right_->color_ == BLACK)
				   && (s->left_ == nil_ || s->left_->color_ == BLACK)){
					s->color_ = RED;
					x = x_parent;
					x_parent = x_parent->parent_;
				}
				else{
					if(s->left_ == nil_ || s->left_->color_ == BLACK){
						s->right_->color_ = BLACK;
						s->color_ = RED;
						rotate_left(s);
						s = x_parent->left_;
					}
					s->color_ = x_parent->color_;     
					x_parent->color_ = BLACK;
					s->left_->color_ = BLACK;
					rotate_right(x_parent);
					break;
				}
			}
		}
	}
	x->color_ = BLACK;
}

#ifdef _DEBUG_
template <typename T>
void rb_tree<T>::inorder_traverse(node_type* const &t) const
{
	if(t != nil_){
		if(t->left_ != nil_)
			inorder_traverse(t->left_);
		std::cout<<t->data_.get_hash()<<' ';
		if(t->right_ != nil_)
			inorder_traverse(t->right_);
	}
}

template <typename T>
void rb_tree<T>::preorder_traverse(node_type* const &t) const
{
	if(t != nil_){
		std::cout<<t->data_.get_hash()<<' ';
		if(t->left_ != nil_)
			inorder_traverse(t->left_);
		if(t->right_ != nil_)
			inorder_traverse(t->right_);
	}
}
#endif

}
#endif
