#ifndef _ENTITY_NODE_H
#define _ENTITY_NODE_H

#include <string>
#include <string.h>
#include <iostream>
#include <assert.h>

namespace lb
{

const int IDENTITY_SIZE = 100;

template <typename T>
class entity_node {
public:
	entity_node() {}
	entity_node(const char* identity, int num_vnodes, const T& data);	
	~entity_node() {}
public:
	const char* get_identity() const;
	long get_num_vnodes() const;
	
	void set_data(const T& data);
	const T& get_data() const;

protected:
	void set_entity_node(const char* identity, int num_vnodes, const T& data);
private:   
	char identity_[IDENTITY_SIZE];
	int  num_vnodes_;
	T    data_;
};

template <typename T>
entity_node<T>::entity_node(const char* identity, int num_vnodes, const T& data)
	:num_vnodes_(num_vnodes), data_(data)
{
	assert(identity != NULL);
	assert(num_vnodes >= 0);
	strcpy(identity_, const_cast<char *>(identity));
}

template <typename T>
inline const char* entity_node<T>::get_identity() const
{
	return identity_;
}

template <typename T>
inline long entity_node<T>::get_num_vnodes() const
{
	return num_vnodes_;
}

template <typename T>
inline void entity_node<T>::set_data(const T& data)
{
	data_ = data;
}

template <typename T>
inline const T& entity_node<T>::get_data() const
{
	return data_;
}

template <typename T>
inline void entity_node<T>::set_entity_node(const char* identity, int num_vnodes, const T& data)
{
	strcpy(identity_, const_cast<char *>(identity));;
	num_vnodes_ = num_vnodes;
	data_ = data;
}

}
#endif
