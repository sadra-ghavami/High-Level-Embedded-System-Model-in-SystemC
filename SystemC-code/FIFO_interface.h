#ifndef FIFO_INTERFACE_HEADER_MACRO
#define FIFO_INTERFACE_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <string>

template <class T>
class put_if : virtual public sc_interface
{
public:
	virtual void put(T) = 0;
};

template <class T>
class get_if : virtual public sc_interface
{
public:
	virtual void get(T &) = 0;
};

template<class T>
class is_empty_if : virtual public sc_interface
{
public:
	virtual bool is_empty() = 0;
};

#endif
