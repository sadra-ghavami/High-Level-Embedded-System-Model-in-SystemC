#ifndef MEMORY_HEADER_MACRO
#define MEMORY_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <fstream>
#include <string>


template<int adrbit, int databit, char const *init_file_name>
SC_MODULE(Memory) {
	sc_in<sc_logic> clk, memread, memwrite;
	sc_in<sc_lv<adrbit>> address;
	sc_in<sc_lv<databit>> datain;
	sc_out<sc_lv<databit>> dataout;
	sc_out<sc_logic> memready;

	int mem_num;
	sc_lv<databit> *mem;

	SC_CTOR(Memory) {
		mem_num = int(pow(2, adrbit));
		mem = new sc_lv<databit>[mem_num];

		SC_THREAD(initialize);

		SC_THREAD(reading_and_writing);
		sensitive << address << memread << memwrite;
		
	}
	void initialize();
	void reading_and_writing();
	void dump_into_file();
};

template<int adrbit, int databit, char const *init_file_name>
void Memory<adrbit, databit, init_file_name>::initialize() {
	ifstream input_file;
	input_file.open(init_file_name);
	for (int i = 0; i < mem_num; i++) {
		std::string line;
		input_file >> line;
		int number = 0;
		for (int j = 0; j < databit; j++) {
			int binary_num = int(line[j]-'0') * int(pow(2, databit - j - 1));
			number += binary_num;
		}
		mem[i] = number;
		std::cout << i << ") initial value: " << mem[i] << std::endl;
	}
	input_file.close();
}

template<int adrbit, int databit, char const *init_file_name>
void Memory<adrbit, databit, init_file_name>::dump_into_file() {
	ofstream output_file;
	output_file.open("mem_out.txt");
	for (int i = 0; i < mem_num; i++) {
		output_file << i << ": \t" << mem[i] << endl;
	}
	output_file.close();
}


template<int adrbit, int databit, char const *init_file_name>
void Memory<adrbit, databit, init_file_name>::reading_and_writing() {
	while (true) {
		memready->write(SC_LOGIC_0);
		if (memread->read() == SC_LOGIC_1 && address->read().to_uint() < mem_num && memwrite->read() == SC_LOGIC_0) {
			dataout = mem[address->read().to_uint()];
			memready->write(SC_LOGIC_1);
			std::cout << address->read().to_uint() << ") MEM READ:	" << mem[address->read().to_uint()] << std::endl;
		}
		else if (memwrite->read() == SC_LOGIC_1 && memread->read() == SC_LOGIC_0  && address->read().to_uint() < mem_num) {
			mem[address->read().to_uint()] = datain->read();
			wait(clk->posedge_event());
			memready->write(SC_LOGIC_1);
			std::cout << address->read().to_uint() << ") MEM WRITE:	" << datain->read() << std::endl;
		}
		wait();
	}
}

#endif
