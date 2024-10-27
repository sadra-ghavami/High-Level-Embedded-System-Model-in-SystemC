#ifndef INTERMEDIATE_COMP_HEADER_MACRO
#define INTERMEDIATE_COMP_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <string>
#include "FIFO_interface.h"

const int USE_CTRL_REGISTER = 0;
const int USE_DATA_REGISTER = 1;

template<int size, int adrbit, int base_adr>
SC_MODULE(Intermediate_Comp) {
	sc_port<put_if<sc_lv<size>>> channel_out;
	sc_port<is_empty_if<sc_lv<size>>> checking_empty;
	sc_in<sc_logic> clk, read_IO, write_IO;
	sc_in<sc_lv<size>> bus_input;
	sc_in<sc_lv<adrbit>> input_address;
	sc_out<sc_lv<size>> bus_output;
	sc_out<sc_logic> readyIO;

	sc_lv<size> ctrl_reg;
	sc_lv<size> data_reg;
	sc_logic channel_empty;

	SC_CTOR(Intermediate_Comp) {
		SC_THREAD(reg_accessing);
		sensitive << channel_out << read_IO << write_IO << bus_input << input_address << checking_empty;
	}

	void reg_accessing();
};

template<int size, int adrbit, int base_adr>
void Intermediate_Comp<size, adrbit, base_adr>::reg_accessing() {
	while (true) {
		channel_empty = checking_empty->is_empty();
		ctrl_reg[0] = channel_empty;

		int address = input_address->read().to_uint() - base_adr;
		if (address == USE_CTRL_REGISTER) {
			if (read_IO->read() == SC_LOGIC_1) {
				bus_output = ctrl_reg;
				readyIO = SC_LOGIC_1;
			}
			else if (write_IO->read() == SC_LOGIC_1) {
				ctrl_reg = bus_input;
				readyIO = SC_LOGIC_1;
				wait(clk->posedge_event());
			}
		}
		else if (address == USE_DATA_REGISTER) {
			if (read_IO->read() == SC_LOGIC_1) {
				bus_output = data_reg;
				readyIO = SC_LOGIC_1;
			}
			else if (write_IO->read() == SC_LOGIC_1) {
				data_reg = bus_input;
				readyIO = SC_LOGIC_1;
				wait(clk->posedge_event());
				channel_out->put(data_reg);
			}
		}
		

		wait();
	}
}

#endif
