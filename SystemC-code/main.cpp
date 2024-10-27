#include <systemc.h>
#include <iostream>

#include "Memory_TB.h"
#include "Bus_TB.h"
#include "FIFO_TB.h"
#include "IO_TB.h"
#include "Processor_TB.h"
#include "System_TB.h"

int sc_main(int argc, char** argv) {
	
	//Memory_TB* mem_testbench = new Memory_TB("test_mem");
	/*
	sc_trace_file* VCD_MEM;
	VCD_MEM = sc_create_vcd_trace_file("MEM_testbench");
	sc_trace(VCD_MEM, mem_testbench->clk, "clk");
	sc_trace(VCD_MEM, mem_testbench->datain, "data_in");
	sc_trace(VCD_MEM, mem_testbench->dataout, "data_out");
	sc_trace(VCD_MEM, mem_testbench->address, "address");
	sc_trace(VCD_MEM, mem_testbench->memread, "memread");
	sc_trace(VCD_MEM, mem_testbench->memwrite, "memwrite");
	sc_trace(VCD_MEM, mem_testbench->memready, "memready");
	*/
	//Bus_TB* bus_testbench = new Bus_TB("bus_test_bench");
	//FIFO_TB* fifo_test_bench = new FIFO_TB("test_fifo");
	//IO_TB* io_testbench = new IO_TB("test_IOs");
	//Processor_TB* test_processor = new Processor_TB("test_processor");
	System_TB* test_sys = new System_TB("test_system");
	sc_start(100000, SC_NS);

	test_sys->dump_mem();

	return 0;
}