#ifndef FIFO_TB_HEADER_MACRO
#define FIFO_TB_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <string>
#include "FIFO_channel.h"

SC_MODULE(FIFO_TB) {
	FIFO_channel<sc_lv<4>, 4>* test_FIFO;
	sc_lv<4> get_data;
	sc_lv<4> put_data;
	bool is_empty;
	SC_CTOR(FIFO_TB) {
		test_FIFO = new FIFO_channel<sc_lv<4>, 4>;
		SC_THREAD(testing_fifo);
	}
	void testing_fifo();
};

void FIFO_TB::testing_fifo() {
	is_empty = test_FIFO->is_empty();
	std::cout << "1) is empty: " << is_empty << std::endl;
	std::cout << "2) put data: 0000" << std::endl;
	put_data = "0000";
	test_FIFO->put(put_data);
	std::cout << "3) put data: 0001" << std::endl;
	put_data = "0001";
	test_FIFO->put(put_data);
	is_empty = test_FIFO->is_empty();
	std::cout << "4) is_empty: " << is_empty <<std::endl;
	std::cout << "5) put data: 0010" << std::endl;
	put_data = "0010";
	test_FIFO->put(put_data);
	test_FIFO->get(get_data);
	std::cout << "6) get data: " << get_data << std::endl;
	test_FIFO->get(get_data);
	std::cout << "7) get data: " << get_data << std::endl;
	std::cout << "8) put data: 1000" << std::endl;
	put_data = "1000";
	test_FIFO->put(put_data);
	std::cout << "9) put data: 1100" << std::endl;
	put_data = "1100";
	test_FIFO->put(put_data);
	std::cout << "10) put data: 1110" << std::endl;
	put_data = "1110";
	test_FIFO->put(put_data);
	
	test_FIFO->get(get_data);
	std::cout << "11) get data: " << get_data << std::endl;
	test_FIFO->get(get_data);
	std::cout << "12) get data: " << get_data << std::endl;
	test_FIFO->get(get_data);
	std::cout << "13) get data: " << get_data << std::endl;
	is_empty = test_FIFO->is_empty();
	std::cout << "14) is empty: " << is_empty << std::endl;
	test_FIFO->get(get_data);
	std::cout << "15) get data: " << get_data << std::endl;
	is_empty = test_FIFO->is_empty();
	std::cout << "16) is empty: " << is_empty << std::endl;

}


#endif
