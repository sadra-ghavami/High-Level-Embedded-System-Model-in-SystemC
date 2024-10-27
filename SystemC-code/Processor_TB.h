#ifndef PROCESSOR_TB_HEADER_MACRO
#define PROCESSOR_TB_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <string>
#include "Processor.h"

SC_MODULE(Processor_TB) {
	sc_signal<sc_logic> clk, memready, IOready;
	sc_signal <sc_lv<32>> input_bus;
	sc_signal <sc_lv<32>> output_bus;
	sc_signal <sc_lv<10>> address;
	sc_signal<sc_logic> read_mem, write_mem, readIO, writeIO;

	Processor<32, 10, 1020, 1022, 8, 128, 32>* processor;

	SC_CTOR(Processor_TB) {
		processor = new Processor<32, 10, 1020, 1022, 8, 128, 32>("process_test");
		processor->clk(clk); processor->memready(memready); processor->IOready(IOready);
		processor->input_bus(input_bus); processor->output_bus(output_bus); processor->address(address);
		processor->read_mem(read_mem); processor->write_mem(write_mem); processor->readIO(readIO); processor->writeIO(writeIO);

		SC_THREAD(clk_generation);

		SC_THREAD(input_generation);
	}

	void clk_generation();
	void input_generation();

};

void Processor_TB::clk_generation() {
	while (true) {
		clk = SC_LOGIC_0;
		wait(10, SC_NS);
		clk = SC_LOGIC_1;
		wait(10, SC_NS);
	}
}

void Processor_TB::input_generation() {
	for (int j = 0; j < 4; j++) {
		IOready = SC_LOGIC_0; memready = SC_LOGIC_0;
		wait(105, SC_NS);
		IOready = SC_LOGIC_1; input_bus = "00000000000000000000000000000000";
		wait(100, SC_NS);
		input_bus = "00000000000000000000000000000001";
		wait(120, SC_NS);
		for (int i = 0; i < 32; i++) {
			IOready = SC_LOGIC_0; memready = SC_LOGIC_1;
			input_bus = i;
			wait(40, SC_NS);
			IOready = SC_LOGIC_1; memready = SC_LOGIC_0;
			wait(40, SC_NS);
			IOready = SC_LOGIC_0;
		}

		for (int i = 0; i < 32; i++) {
			IOready = SC_LOGIC_1; input_bus = 0;
			std::cout << "ctrl signal sending to inter_comp: " << output_bus << std::endl;
			wait(240, SC_NS);
			input_bus = 2;
			wait(20, SC_NS);
			input_bus = 4*i;
			wait(20, SC_NS);
			memready = SC_LOGIC_1; IOready = SC_LOGIC_0;
			wait(20, SC_NS);
			memready = SC_LOGIC_0;
			std::cout << "mult result : " << output_bus << std::endl;
			wait(20, SC_NS);
		}
	}
}

#endif
