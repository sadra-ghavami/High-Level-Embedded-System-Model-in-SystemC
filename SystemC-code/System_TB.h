#ifndef SYSTEM_TB_HEADER_MACRO
#define SYSTEM_TB_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <string>
#include "system.h"

SC_MODULE(System_TB) {
	sc_signal<sc_logic>clk;
	System* sys;
	SC_CTOR(System_TB) {
		sys = new System("test_system");
		sys->clk(clk);
		SC_THREAD(clk_generation);
	}
	void clk_generation();
	void dump_mem();
};

void System_TB::clk_generation() {
	while (true) {
		clk.write(SC_LOGIC_0);
		wait(10, SC_NS);
		clk.write(SC_LOGIC_1);
		wait(10, SC_NS);
	}
}

void System_TB::dump_mem() {
	sys->mem->dump_into_file();
}

#endif
