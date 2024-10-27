#ifndef PROCESSOR_HEADER_MACRO
#define PROCESSOR_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <string>

template<int databit, int adrbit, int inter_comp_base_adr, int mult_base_adr, int mem_start_adr, int num_of_mem_elems, int channel_size>
SC_MODULE(Processor) {
	sc_in<sc_logic> clk, memready, IOready;
	sc_in <sc_lv<databit>> input_bus;
	sc_out <sc_lv<databit>> output_bus;
	sc_out <sc_lv<adrbit>> address;
	sc_out<sc_logic> read_mem, write_mem, readIO, writeIO;

	SC_CTOR(Processor) {
		SC_THREAD(control_flow);
		sensitive << clk.pos();
	}
	void control_flow();
};

template<int databit, int adrbit, int inter_comp_base_adr, int mult_base_adr, int mem_start_adr, int num_of_mem_elems, int channel_size>
void Processor<databit, adrbit, inter_comp_base_adr, mult_base_adr, mem_start_adr, num_of_mem_elems, channel_size>::control_flow() {
	int address_num = mem_start_adr;
	int result_address_in_mem = mem_start_adr + num_of_mem_elems + 5;
	read_mem = SC_LOGIC_0; write_mem = SC_LOGIC_0; readIO = SC_LOGIC_0; writeIO = SC_LOGIC_0;
	for (int i = 0; i < (num_of_mem_elems/channel_size); i++) {
		sc_logic channel_empty = SC_LOGIC_0;

		while (channel_empty == SC_LOGIC_0) {
			address = inter_comp_base_adr; readIO = SC_LOGIC_1;
			wait();
			while (IOready == SC_LOGIC_0) wait();
			readIO = SC_LOGIC_0;
			channel_empty = input_bus[0];
		}
		
		for (int j = 0; j < channel_size; j++) {
			address = address_num; read_mem = SC_LOGIC_1;
			wait();
			while (memready == SC_LOGIC_0) {
				wait();
			}
			//std::cout << "memready =  " << memready << std::endl;
			read_mem = SC_LOGIC_0; address = inter_comp_base_adr + 1; 
			writeIO = SC_LOGIC_1; output_bus = input_bus;
			wait();
			while (IOready == SC_LOGIC_0) wait();
			writeIO = SC_LOGIC_0;
			address_num++;
		}
		wait();
		for (int j = 0; j < channel_size; j++) {
			address = mult_base_adr; writeIO = SC_LOGIC_1;
			output_bus = "01";
			wait();
			while (IOready == SC_LOGIC_0) wait();
			sc_logic done = SC_LOGIC_0;
			while (done == SC_LOGIC_0) {
				writeIO = SC_LOGIC_0; readIO = SC_LOGIC_1;
				address = mult_base_adr;
				wait();
				while (IOready == SC_LOGIC_0) wait();
				//std::cout << "ctrl register mult: " <<input_bus << std::endl;
				if (input_bus->read().to_uint() == 2 ) done = SC_LOGIC_1;
				readIO = SC_LOGIC_0;
			}
			readIO = SC_LOGIC_1; address = mult_base_adr + 1;
			wait();
			while (IOready == SC_LOGIC_0) wait();
			readIO = SC_LOGIC_0; write_mem = SC_LOGIC_1;
			address = result_address_in_mem; output_bus = input_bus;
			wait();
			//std::cout << "memready : " << memready << std::endl;
			while (memready == SC_LOGIC_0) {
				wait();
			}
			write_mem = SC_LOGIC_0; read_mem = SC_LOGIC_0;
			result_address_in_mem++;
			wait();
			//std::cout << "memready : " << memready << std::endl;
		}
	}
	std::cout << "process is completed!" << std::endl;
}

#endif
