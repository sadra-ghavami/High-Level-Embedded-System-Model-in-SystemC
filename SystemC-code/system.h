#ifndef SYSTEM_HEADER_MACRO
#define SYSTEM_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <string>

#include "Memory.h"
#include "Bus.h"
#include "Processor.h"
#include "Intermediate_Comp.h"
#include "Multiplier.h"
#include "FIFO_channel.h"

char initial_file_name[] = "initial_file.txt";


SC_MODULE(System) {
	sc_in<sc_logic>clk;
	sc_signal<sc_lv<32>> databus_to_processor, databus_from_processor, databus_to_memory, databus_from_memory,
		databus_to_inter_comp, databus_from_inter_comp, databus_to_mult, databus_from_mult;
	sc_signal<sc_lv<10>> adrbus_from_processor, adrbus_to_mem, adrbus_to_inter_comp, adrbus_to_mult;
	sc_signal<sc_logic> memready_to_processor, memready_from_mem, inter_comp_ready, mult_ready, IOready_to_processor,
		readmem_from_processor, readmem_to_mem, writemem_from_processor, writemem_to_mem, readIO_from_processor, writeIO_from_processor,
		readIO_to_inter_comp, writeIO_to_inter_comp, readIO_to_mult, writeIO_to_mult;

	Memory<10, 32, initial_file_name>* mem;
	Bus<32, 10, 1020, 1022>* bus;
	Processor<32, 10, 1020, 1022, 8, 128, 32>* processor;
	Intermediate_Comp<32, 10, 1020>* inter_comp;
	Multiplier<16, 10, 1022, 16>* mult;
	FIFO_channel<sc_lv<32>, 32>* channel;

	SC_CTOR(System) {
		channel = new FIFO_channel<sc_lv<32>, 32>;
		
		mem = new Memory<10, 32, initial_file_name>("inst_mem");
		mem->clk(clk);
		mem->memread(readmem_to_mem); mem->memwrite(writemem_to_mem);
		mem->address(adrbus_to_mem); mem->datain(databus_to_memory);
		mem->dataout(databus_from_memory); mem->memready(memready_from_mem);

		bus = new Bus<32, 10, 1020, 1022>("inst_bus");
		bus->data_from_processor(databus_from_processor); bus->data_from_mem(databus_from_memory); bus->data_from_inter_comp(databus_from_inter_comp);
		bus->data_from_mult(databus_from_mult); bus->data_to_processor(databus_to_processor); bus->data_to_mem(databus_to_memory);
		bus->data_to_inter_comp(databus_to_inter_comp); bus->data_to_mult(databus_to_mult); bus->address_from_processor(adrbus_from_processor);
		bus->address_to_mem(adrbus_to_mem); bus->address_to_inter_comp(adrbus_to_inter_comp); bus->address_to_mult(adrbus_to_mult);
		bus->read_mem_from_processor(readmem_from_processor); bus->write_mem_from_processor(writemem_from_processor);
		bus->memready_from_memory(memready_from_mem); bus->read_IO_from_processor(readIO_from_processor); bus->write_IO_from_processor(writeIO_from_processor);
		bus->IOready_from_inter_comp(inter_comp_ready); bus->IOready_from_mult(mult_ready); bus->read_mem_to_memory(readmem_to_mem);
		bus->write_mem_to_memory(writemem_to_mem); bus->memready_to_processor(memready_to_processor); bus->IOready(IOready_to_processor);
		bus->read_IO_to_inter_comp(readIO_to_inter_comp); bus->write_IO_to_inter_comp(writeIO_to_inter_comp);
		bus->read_IO_to_mult(readIO_to_mult); bus->write_IO_to_mult(writeIO_to_mult);

		processor = new Processor<32, 10, 1020, 1022, 8, 128, 32>("inst_processor");
		processor->clk(clk); processor->memready(memready_to_processor); processor->IOready(IOready_to_processor);
		processor->input_bus(databus_to_processor); processor->output_bus(databus_from_processor);
		processor->address(adrbus_from_processor); processor->read_mem(readmem_from_processor);
		processor->write_mem(writemem_from_processor); processor->readIO(readIO_from_processor);
		processor->writeIO(writeIO_from_processor);

		inter_comp = new Intermediate_Comp<32, 10, 1020>("inst_inter_comp");
		inter_comp->channel_out(*channel); inter_comp->checking_empty(*channel);
		inter_comp->clk(clk); inter_comp->read_IO(readIO_to_inter_comp); inter_comp->write_IO(writeIO_to_inter_comp); inter_comp->bus_input(databus_to_inter_comp);
		inter_comp->input_address(adrbus_to_inter_comp); inter_comp->bus_output(databus_from_inter_comp); inter_comp->readyIO(inter_comp_ready);

		mult = new Multiplier<16, 10, 1022, 16>("inst_mult");
		mult->clk(clk); mult->read_IO(readIO_to_mult); mult->write_IO(writeIO_to_mult); mult->bus_input(databus_to_mult); mult->bus_output(databus_from_mult);
		mult->address_input(adrbus_to_mult); mult->result_ready(mult_ready); mult->channel_input(*channel);

	}

};


#endif
