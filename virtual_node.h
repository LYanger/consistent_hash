#ifndef _VIRTUAL_H
#define _VIRTUAL_H

#include "lb_util.h"
#include "entity_node.h"

namespace lb 
{

template <typename T>
class virtual_node {
public:
	virtual_node();
	virtual_node(const entity_node<T>& node);
	~virtual_node() {}
public:
	void set_entity_node(const entity_node<T>& node);
	entity_node<T>* get_entity_node() const;

	void set_hash(long hash);
	long get_hash() const;
public:
	bool operator<(const virtual_node<T>& other) const;
	bool operator>(const virtual_node<T>& other) const;
	bool operator==(const virtual_node<T>& other) const;
	bool operator>=(const virtual_node<T>& other) const;
private:
	long                   hash_;
	mutable entity_node<T> node_;
};

template <typename T>
virtual_node<T>::virtual_node()
{
	hash_ = -1;
}

template <typename T>
inline virtual_node<T>::virtual_node(const entity_node<T>& node)
	: node_(node), hash_(-1)
{
}

template <typename T>
inline void virtual_node<T>::set_entity_node(const entity_node<T>& node)
{
	node_ = node;
}

template <typename T>
inline entity_node<T>* virtual_node<T>::get_entity_node() const
{
	return (entity_node<T>*)&node_;
}

template <typename T>
inline void virtual_node<T>::set_hash(long hash)
{
	hash_ = hash;
}

template <typename T>
inline long virtual_node<T>::get_hash() const
{
	return hash_;
}

template <typename T>
inline bool virtual_node<T>::operator<(const virtual_node<T> other) const
{
	return hash_ < other.hash_;
}

template <typename T>
inline bool virtual_node<T>::operator>(const virtual_node<T> other) const
{
	return hash_ > other.hash_;
}

template <typename T>
inline bool virtual_node<T>::operator==(const virtual_node<T> other) const
{
	return hash_ == other.hash_;
}

template <typename T>
inline bool virtual_node<T>::operator>=(const virtual_node<T> other) const
{
	return hash_ >= other.hash_;
}

}
#endif
