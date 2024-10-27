#ifndef BUS_HEADER_MACRO
#define BUS_HEADER_MACRO

#include <systemc.h>
#include <iostream>
#include <string>

template<int databit, int adrbit, int base_adr_inter_comp, int base_adr_mult>
SC_MODULE(Bus) {
	sc_in<sc_lv<databit>> data_from_processor, data_from_mem, data_from_inter_comp, data_from_mult;
	sc_out<sc_lv<databit>> data_to_processor, data_to_mem, data_to_inter_comp, data_to_mult;
	sc_in<sc_lv<adrbit>> address_from_processor;
	sc_out<sc_lv<adrbit>> address_to_mem, address_to_inter_comp, address_to_mult;
	sc_in<sc_logic> read_mem_from_processor, write_mem_from_processor, memready_from_memory, read_IO_from_processor, write_IO_from_processor,
		IOready_from_inter_comp, IOready_from_mult;
	sc_out<sc_logic> read_mem_to_memory, write_mem_to_memory, memready_to_processor, IOready,
					 read_IO_to_inter_comp, write_IO_to_inter_comp, read_IO_to_mult, write_IO_to_mult;

	SC_CTOR(Bus) {
		SC_THREAD(bind_buses);
		sensitive << data_from_processor << data_from_mem << data_from_inter_comp << data_from_mult << address_from_processor << IOready_from_inter_comp <<
			read_mem_from_processor << write_mem_from_processor << memready_from_memory << read_IO_from_processor << write_IO_from_processor << IOready_from_mult;

	}

	void bind_buses();


};

template<int databit, int adrbit, int base_adr_inter_comp, int base_adr_mult>
void Bus<databit, adrbit, base_adr_inter_comp, base_adr_mult>::bind_buses() {
	while (true) {
		if (read_mem_from_processor == SC_LOGIC_1 || write_mem_from_processor == SC_LOGIC_1) {
			read_mem_to_memory = read_mem_from_processor;
			write_mem_to_memory = write_mem_from_processor;
			address_to_mem = address_from_processor;
			data_to_mem = data_from_processor;
			data_to_processor = data_from_mem;
			memready_to_processor = memready_from_memory;
			std::cout << "access to mem" << std::endl;
		}
		else if (read_IO_from_processor == SC_LOGIC_1 || write_IO_from_processor == SC_LOGIC_1) {
			if (address_from_processor->read().to_uint() >= base_adr_inter_comp && address_from_processor->read().to_uint() < base_adr_mult) {
				read_IO_to_inter_comp = read_IO_from_processor;
				write_IO_to_inter_comp = write_IO_from_processor;
				data_to_inter_comp = data_from_processor;
				address_to_inter_comp = address_from_processor;
				data_to_processor = data_from_inter_comp;
				IOready = IOready_from_inter_comp;
				std::cout << "access to inter comp" << std::endl;
			}
			else if (address_from_processor->read().to_uint() >= base_adr_mult) {
				read_IO_to_mult = read_IO_from_processor;
				write_IO_to_mult = write_IO_from_processor;
				data_to_mult = data_from_processor;
				address_to_mult = address_from_processor;
				data_to_processor = data_from_mult;
				IOready = IOready_from_mult;
				std::cout << "access to mult" << std::endl;
			}
		}
		wait();
	}
}



#endif
