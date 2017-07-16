#include "systemc.h"
#include "../../../common/defines.h"

SC_MODULE(tsv_controller){
	/*
	 * This module is responsible to
	 * allocate the FSM commands to the appropriate
	 * bank.
	 * There are 4 FSM sending data to 16 banks atm.
	 * Only one bank can send data at a given time (we use round-robin approach).
	 */

public:
	sc_in_clk clk;

	sc_in<bool> we[FAW], oe[FAW];
	sc_in<sc_lv<LOG_NUMBER_OF_BANKS> > bank_id[FAW];
	sc_in<sc_lv<256> > data_from_data_buffer[FAW];
	sc_in<sc_lv<256> > data_from_pim_data_buffer[FAW];
	sc_in<sc_lv<256> > data_from_bank[NUMBER_OF_BANKS];
	sc_in<bool> data_available_to_read[NUMBER_OF_BANKS];
	sc_in<sc_lv<2> > fsm_operation[FAW];

	sc_out<bool> write_available[NUMBER_OF_BANKS], ack_request[FAW];
	sc_out<sc_lv<256> > data_to_bank[NUMBER_OF_BANKS];
	sc_out<sc_lv<256> > data_to_data_buffer[FAW];
	sc_out<bool> en_push_data_buffer[FAW];
	sc_out<bool> en_push_pim_data_buffer[FAW];

	SC_CTOR(tsv_controller){
		SC_METHOD(prc_tsv_controller);
		sensitive << clk.neg();
		round_robin = 0;
		vault_id = 0;
	}

	int vault_id;
private:
	void prc_tsv_controller();
	unsigned round_robin;
};

