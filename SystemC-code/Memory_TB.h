#ifndef MEMORY_TB_HEADER_MACRO
#define MEMORY_TB_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <string>
#include "Memory.h"

char initial_name[] = "init_file_test.txt";

SC_MODULE(Memory_TB) {
	sc_signal<sc_logic> clk, memread, memwrite;
	sc_signal<sc_lv<3>> address;
	sc_signal<sc_lv<8>> datain;
	sc_signal<sc_lv<8>> dataout;
	sc_signal<sc_logic> memready;
	
	

	Memory<3, 8, initial_name>* mem;

	SC_CTOR(Memory_TB) {
		mem = new Memory<3, 8, initial_name>("mem_test");
		mem->clk(clk);
		mem->memread(memread);
		mem->memwrite(memwrite);
		mem->address(address);
		mem->datain(datain);
		mem->dataout(dataout);
		mem->memready(memready);

		SC_THREAD(generate_clk);
		SC_THREAD(generate_inputs);
	}
	void generate_clk();
	void generate_inputs();
};

void Memory_TB::generate_clk() {
	while (true) {
		clk.write(SC_LOGIC_0);
		wait(10, SC_NS);
		clk.write(SC_LOGIC_1);
		wait(10, SC_NS);
	}
}

void Memory_TB::generate_inputs() {
	memread.write(SC_LOGIC_0);
	memwrite.write(SC_LOGIC_0);
	address = "000";
	datain = "11111111";
	wait(5, SC_NS);
	memread.write(SC_LOGIC_1);
	std::cout << "read data from address : 010" << std::endl;
	address = "010";
	wait(10, SC_NS);
	std::cout << "read data from address : 011" << std::endl;
	address = "011";
	wait(10, SC_NS);
	std::cout << "read data from address : 100" << std::endl;
	address = "100";
	wait(10, SC_NS);
	std::cout << "write data:11111111 into address : 111" << std::endl;
	memwrite.write(SC_LOGIC_1);
	memread.write(SC_LOGIC_0);
	address = "111";
	wait(60, SC_NS);
	std::cout << "write data:11110000 into address : 110" << std::endl;
	address = "110";
	datain = "11110000";
	wait(20, SC_NS);
	memwrite.write(SC_LOGIC_0);
	mem->dump_into_file();
}

#endif

