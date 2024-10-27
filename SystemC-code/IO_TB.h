#ifndef IO_TB_HEADER_TB
#define IO_TB_HEADER_TB

#include <systemc.h>
#include <iostream>
#include <string>

#include "FIFO_channel.h"
#include "Multiplier.h"
#include "Intermediate_Comp.h"

SC_MODULE(IO_TB) {
	
	sc_signal<sc_logic> clk, read_mult, write_mult;
	sc_signal<sc_lv<8>> mult_input;
	sc_signal<sc_lv<8>> mult_output;
	sc_signal<sc_lv<8>> mult_address_input;
	sc_signal<sc_logic> mult_ready;

	sc_signal<sc_logic> read_inter_comp, write_inter_comp;
	sc_signal<sc_lv<8>> inter_comp_input;
	sc_signal<sc_lv<8>> inter_comp_input_address;
	sc_signal<sc_lv<8>> inter_comp_output;
	sc_signal<sc_logic> inter_comp_ready;
	
	FIFO_channel<sc_lv<8>, 4>* channel;
	Intermediate_Comp<8, 8, 250>* int_comp;
	Multiplier<4, 8, 252, 12>* mult;

	SC_CTOR(IO_TB) {
		channel = new FIFO_channel<sc_lv<8>, 4>;

		int_comp = new Intermediate_Comp<8, 8, 250>("test_comp");
		int_comp->channel_out(*channel); int_comp->checking_empty(*channel);
		int_comp->clk(clk); int_comp->read_IO(read_inter_comp); int_comp->write_IO(write_inter_comp); int_comp->bus_input(inter_comp_input);
		int_comp->input_address(inter_comp_input_address); int_comp->bus_output(inter_comp_output); int_comp->readyIO(inter_comp_ready);

		mult = new Multiplier<4, 8, 252, 12>("test_mult");
		mult->clk(clk); mult->read_IO(read_mult); mult->write_IO(write_mult); mult->bus_input(mult_input); mult->bus_output(mult_output);
		mult->address_input(mult_address_input); mult->result_ready(mult_ready); mult->channel_input(*channel);

		SC_THREAD(clk_generation);

		SC_THREAD(input_generation);

	}
	void clk_generation();
	void input_generation();
};

void IO_TB::clk_generation() {
	while (true) {
		clk = SC_LOGIC_0;
		wait(10, SC_NS);
		clk = SC_LOGIC_1;
		wait(10, SC_NS);
	}
}

void IO_TB::input_generation() {
	wait(5, SC_NS);
	read_inter_comp = SC_LOGIC_1; write_inter_comp = SC_LOGIC_0;
	inter_comp_input_address = 250; 
	std::cout << "1) reading ctrl register of inter_comp: " << std::endl;
	wait(10, SC_NS);
	std::cout << "output: " << inter_comp_output.read() << std::endl;
	std::cout << "2) writing data 00010001 into fifo: " << std::endl;
	read_inter_comp = SC_LOGIC_0; write_inter_comp = SC_LOGIC_1;
	inter_comp_input_address = 251; inter_comp_input = "00010001";
	wait(20, SC_NS);
	std::cout << "ready: " << inter_comp_ready.read() << std::endl;
	inter_comp_input = "00100010";
	std::cout << "3) writing data 00100010 into fifo: " <<  std::endl;
	wait(20, SC_NS);
	std::cout << "ready: " << inter_comp_ready.read() << std::endl;
	std::cout << "4) writing data 00110011 into fifo: "  << std::endl;
	inter_comp_input = "00110011";
	wait(20, SC_NS);
	std::cout << "ready: " << inter_comp_ready.read() << std::endl;
	std::cout << "5) writing data 01000100 into fifo: " << std::endl;
	inter_comp_input = "01000100";
	wait(20, SC_NS);
	std::cout << "ready: " << inter_comp_ready.read() << std::endl;
	std::cout << "6) reading ctrl register of inter_comp: " << std::endl;
	write_inter_comp = SC_LOGIC_0; read_inter_comp = SC_LOGIC_1;
	inter_comp_input_address = 250;
	wait(20, SC_NS);
	std::cout << "output: " << inter_comp_output.read() << std::endl;
	std::cout << "7) writing data 00000001 into ctrl register of mult(start multiplication): "  << std::endl;
	write_inter_comp = SC_LOGIC_0; read_inter_comp = SC_LOGIC_0;
	write_mult = SC_LOGIC_1; read_mult = SC_LOGIC_0;
	mult_address_input = 252; mult_input = "00000001";
	wait(100, SC_NS);
	std::cout << "ready: " << mult_ready.read() << std::endl;
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_1;
	std::cout << "8) reading ctrl register of mult: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_0;
	wait(120, SC_NS);
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_1;
	std::cout << "9) reading ctrl register of mult: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	mult_address_input = 253;
	std::cout << "10) reading first result: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	std::cout << "11) writing data 00000001 into ctrl register of mult(start multiplication): " << std::endl;
	write_mult = SC_LOGIC_1; read_mult = SC_LOGIC_0;
	mult_address_input = 252; mult_input = "00000001";
	wait(100, SC_NS);
	std::cout << "ready: " << mult_ready.read() << std::endl;
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_1;
	std::cout << "12) reading ctrl register of mult: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_0;
	wait(120, SC_NS);
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_1;
	std::cout << "13) reading ctrl register of mult: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	mult_address_input = 253;
	std::cout << "14) reading first result: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	std::cout << "15) writing data 00000001 into ctrl register of mult(start multiplication): " << std::endl;
	write_mult = SC_LOGIC_1; read_mult = SC_LOGIC_0;
	mult_address_input = 252; mult_input = "00000001";
	wait(100, SC_NS);
	std::cout << "ready: " << mult_ready.read() << std::endl;
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_1;
	std::cout << "16) reading ctrl register of mult: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_0;
	wait(120, SC_NS);
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_1;
	std::cout << "17) reading ctrl register of mult: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	mult_address_input = 253;
	std::cout << "18) reading first result: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	std::cout << "19) writing data 00000001 into ctrl register of mult(start multiplication): " << std::endl;
	write_mult = SC_LOGIC_1; read_mult = SC_LOGIC_0;
	mult_address_input = 252; mult_input = "00000001";
	wait(100, SC_NS);
	std::cout << "ready: " << mult_ready.read() << std::endl;
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_1;
	std::cout << "20) reading ctrl register of mult: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_0;
	wait(120, SC_NS);
	write_mult = SC_LOGIC_0; read_mult = SC_LOGIC_1;
	std::cout << "21) reading ctrl register of mult: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	mult_address_input = 253;
	std::cout << "22) reading first result: " << std::endl;
	wait(20, SC_NS);
	std::cout << "output: " << mult_output.read() << std::endl;
	
}

#endif
