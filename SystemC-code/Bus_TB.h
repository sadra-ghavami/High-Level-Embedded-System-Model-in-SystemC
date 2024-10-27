#ifndef BUS_TB_MACRO
#define BUS_TB_MACRO

#include <systemc.h>
#include <iostream>
#include <string>
#include "Bus.h"

SC_MODULE(Bus_TB) {
	sc_signal<sc_lv<32>> data_from_processor, data_from_mem, data_from_inter_comp, data_from_mult;
	sc_signal<sc_lv<32>> data_to_processor, data_to_mem, data_to_inter_comp, data_to_mult;
	sc_signal<sc_lv<8>> address_from_processor;
	sc_signal<sc_lv<8>> address_to_mem, address_to_inter_comp, address_to_mult;
	sc_signal<sc_logic> read_mem_from_processor, write_mem_from_processor, memready_from_memory, read_IO_from_processor, write_IO_from_processor,
		IOready_from_inter_comp, IOready_from_mult;
	sc_signal<sc_logic> read_mem_to_memory, write_mem_to_memory, memready_to_processor, IOready,
		read_IO_to_inter_comp, write_IO_to_inter_comp, read_IO_to_mult, write_IO_to_mult;
	Bus<32, 8, 250, 252>* test_bus;

	SC_CTOR(Bus_TB) {
		test_bus = new Bus<32, 8, 250, 252>("test_BUS");
		test_bus->data_from_processor(data_from_processor); test_bus->data_from_mem(data_from_mem); test_bus->data_from_inter_comp(data_from_inter_comp);
		test_bus->data_from_mult(data_from_mult); test_bus->data_to_processor(data_to_processor); test_bus->data_to_mem(data_to_mem);
		test_bus->data_to_inter_comp(data_to_inter_comp); test_bus->data_to_mult(data_to_mult); test_bus->address_from_processor(address_from_processor);
		test_bus->address_to_mem(address_to_mem); test_bus->address_to_inter_comp(address_to_inter_comp); test_bus->address_to_mult(address_to_mult);
		test_bus->read_mem_from_processor(read_mem_from_processor); test_bus->write_mem_from_processor(write_mem_from_processor);
		test_bus->memready_from_memory(memready_from_memory); test_bus->read_IO_from_processor(read_IO_from_processor); test_bus->write_IO_from_processor(write_IO_from_processor);
		test_bus->IOready_from_inter_comp(IOready_from_inter_comp); test_bus->IOready_from_mult(IOready_from_mult); test_bus->read_mem_to_memory(read_mem_to_memory);
		test_bus->write_mem_to_memory(write_mem_to_memory); test_bus->memready_to_processor(memready_to_processor); test_bus->IOready(IOready); 
		test_bus->read_IO_to_inter_comp(read_IO_to_inter_comp); test_bus->write_IO_to_inter_comp(write_IO_to_inter_comp);
		test_bus->read_IO_to_mult(read_IO_to_mult); test_bus->write_IO_to_mult(write_IO_to_mult);
		SC_THREAD(input_generation);
	}

	void input_generation();
};

void Bus_TB::input_generation() {
	address_from_processor = "11110000"; data_from_processor = "11111111111111111111111111111111";
	read_mem_from_processor = SC_LOGIC_0; write_mem_from_processor = SC_LOGIC_0;
	read_IO_from_processor = SC_LOGIC_0; write_IO_from_processor = SC_LOGIC_0;
	wait(20, SC_NS);
	read_mem_from_processor = SC_LOGIC_1; data_from_mem = "10101010101010101010101010101010"; memready_from_memory = SC_LOGIC_1;
	wait(5, SC_NS);
	std::cout << "reading memory: \n memroy address: " << address_to_mem.read() << "\n reading data: " << data_to_processor.read() <<
		"\n memready: " << memready_to_processor.read() <<std::endl;
	read_mem_from_processor = SC_LOGIC_0; write_mem_from_processor = SC_LOGIC_1; address_from_processor = "10101010";
	wait(5, SC_NS);
	std::cout << "writing memory: \n memroy address: " << address_to_mem.read() << "\n writing data: " << data_to_mem.read() <<
		"\n memready: " << memready_to_processor.read() << std::endl;
	write_mem_from_processor = SC_LOGIC_0; address_from_processor = "10000000";
	write_IO_from_processor = SC_LOGIC_1; IOready_from_inter_comp = SC_LOGIC_1;
	wait(5, SC_NS);
	std::cout << "Writing IO(inter_comp): \n IO address: " << address_from_processor.read() << "\n writing data: " << data_from_inter_comp.read() <<
		"\n write_IO: " << write_IO_to_inter_comp.read() << "\n ready: " << IOready << std::endl;
	address_from_processor = "11111010"; // 250
	wait(5, SC_NS);
	std::cout << "Writing IO(inter_comp): \n IO address: " << address_from_processor.read() << "\n writing data: " << data_to_inter_comp.read() <<
		"\n write_IO: " << write_IO_to_inter_comp.read() << "\n ready: " << IOready << std::endl;
	address_from_processor = "11111101"; // 253
	data_from_processor = "11110000111100001111000011110000";  IOready_from_mult = SC_LOGIC_1;
	wait(5, SC_NS);
	std::cout << "Writing IO(mult): \n IO address: " << address_to_mult.read() << "\n writing data: " << data_to_mult.read() <<
		"\n write_IO: " << write_IO_to_mult.read() << "\n ready: " << IOready << std::endl;
}

#endif
