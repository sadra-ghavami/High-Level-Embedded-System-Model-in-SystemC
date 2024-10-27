#ifndef FIFO_HEADER_MACRO
#define FIFO_HEADER_MACRO
 
#include <systemc.h>
#include <iostream>
#include <string>
#include "FIFO_interface.h"

template<class T, int Max>
class FIFO_channel : public get_if<T>, public put_if<T>, public is_empty_if<T> {
	int size;
	int elems, head;
	T queueContents[Max];
	sc_event put_event, get_event;
public:
	FIFO_channel() : size(Max), elems(0), head(0) {};
	~FIFO_channel() {};
	void put(T data) {
		if (elems == size) wait(get_event);
		queueContents[(head + elems) % size] = data;
		std::cout << "new data pushed: " << queueContents[(head + elems) % size] << std::endl;
		elems = elems + 1;
		put_event.notify();
	}
	void get(T &data) {
		if (elems == 0) wait(put_event);
		data = queueContents[head];
		elems = elems - 1;
		head = (head + 1) % size;
		std::cout << "one element poped: "<< data << std::endl;
		get_event.notify();
	}
	bool is_empty() {
		return elems == 0;
	}
};



#endif
