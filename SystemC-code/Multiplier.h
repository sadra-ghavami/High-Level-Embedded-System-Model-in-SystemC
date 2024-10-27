#ifndef MULTIPLIER_HEADER_MACRO
#define MULTIPLIER_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <string>
#include "FIFO_interface.h"


const int USE_CTRL_REG = 0;
const int USE_DATA_REG = 1;

template<int mult_size, int adrbit, int base_adr, int num_clk>
SC_MODULE(Multiplier) {
	sc_port<get_if<sc_lv<2 * mult_size>>> channel_input;
	sc_in<sc_logic> clk, read_IO, write_IO;
	sc_in<sc_lv<2 * mult_size>> bus_input;
	sc_out<sc_lv<2 * mult_size>> bus_output;
	sc_in<sc_lv<adrbit>> address_input;
	sc_out<sc_logic> result_ready;

	sc_lv<2 * mult_size> ctrl_reg, data_reg;
	sc_signal<sc_logic> start, done;
	
	SC_CTOR(Multiplier) {
		SC_THREAD(reg_accessing);
		sensitive << channel_input << read_IO << write_IO << bus_input << address_input << done;

		SC_THREAD(multiplying);
		sensitive << start;
	}
	void multiplying();
	void reg_accessing();
};

template<int mult_size, int adrbit, int base_adr, int num_clk>
void Multiplier<mult_size, adrbit, base_adr, num_clk>::multiplying() {
	while (true) {
		if (start == SC_LOGIC_1) {
			done = SC_LOGIC_0;
			sc_lv<2 * mult_size> data_temp;
			channel_input->get(data_temp);
			data_temp = data_temp.range(2 * mult_size - 1, mult_size).to_uint() * data_temp.range(mult_size - 1, 0).to_uint();
			for (int i = 0; i < num_clk; i++)
				wait(clk->posedge_event());
			data_reg = data_temp;
			done = SC_LOGIC_1;
			//std::cout << "result of multiplication is: " << data_reg << std::endl;
			ctrl_reg[0] = SC_LOGIC_0;
		}

		wait();
	}
}


template<int mult_size, int adrbit, int base_adr, int num_clk>
void Multiplier<mult_size, adrbit, base_adr, num_clk>::reg_accessing() {
	while (true) {
		result_ready = SC_LOGIC_0;
		ctrl_reg[1] = done;
		int address = address_input->read().to_uint() - base_adr;
		if (address == USE_CTRL_REG) {
			if (read_IO->read() == SC_LOGIC_1) {
				bus_output = ctrl_reg;
				result_ready = SC_LOGIC_1;
			}
			else if (write_IO->read() == SC_LOGIC_1) {
				ctrl_reg = bus_input;
				result_ready = SC_LOGIC_1;
				wait(clk->posedge_event());
			}
		}
		else if (address == USE_DATA_REG) {
			if (read_IO->read() == SC_LOGIC_1) {
				bus_output = data_reg;
				result_ready = SC_LOGIC_1;
			}
			else if (write_IO->read() == SC_LOGIC_1) {
				data_reg = bus_input;
				result_ready = SC_LOGIC_1;
				wait(clk->posedge_event());
			}
		}
		start = ctrl_reg[0];
		

		wait();
	}
}

#endif
