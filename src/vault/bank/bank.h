#ifndef SRC_BANK_BANK_H_
#define SRC_BANK_BANK_H_


#include "systemc.h"
#include "../../common/defines.h"
#include <unordered_map>

SC_MODULE(bank){
public:

	sc_in_clk clk;
	sc_in<bool> cs[FAW], ras[FAW], cas[FAW], oe[FAW], we[FAW], done[FAW];
	sc_in<sc_lv<LOG_NUMBER_OF_BANKS> > request_bank_id[FAW];
	sc_in<sc_lv<20> > address [FAW];
	sc_in<sc_lv<4> > offset_in[FAW];
	sc_in<bool> data_available_to_write;
	sc_in<sc_lv<256> > data_in;

	sc_out<bool> data_available_to_read;
	sc_out<sc_lv<256> > data_out;
	SC_CTOR(bank){
		SC_METHOD(prc_bank);
		sensitive << clk.pos();
		bank_size = BANK_SIZE*1024*1024;
		vault_id = 0;
		bank_id = 0;
		current_fsm = 5;
		bank_available = true;
	}

	int vault_id;
	unsigned bank_id;
	unsigned current_fsm;
	bool bank_available;

private:
	std::unordered_map<unsigned, sc_lv<MAX_BLOCK_SIZE*8> > bank_storage;
	sc_lv<MAX_BLOCK_SIZE*8> row_buffer;
	sc_lv<20> address_buffer;
	sc_lv<MAX_BLOCK_SIZE*8> bank_buffer_read;
	void prc_bank();
	unsigned get_converted_address();
	sc_lv<256> decode_row_buffer();
	void write_row_buffer();
	unsigned bank_size;
};

#endif
