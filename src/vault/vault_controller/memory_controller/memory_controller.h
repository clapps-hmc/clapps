/*
 * memory_controller.h
 *
 *  Created on: 10 de nov de 2016
 *      Author: root
 */

#ifndef SRC_VAULT_VAULT_CONTROLLER_MEMORY_CONTROLLER_MEMORY_CONTROLLER_H_
#define SRC_VAULT_VAULT_CONTROLLER_MEMORY_CONTROLLER_MEMORY_CONTROLLER_H_

#include "systemc.h"
#include "../../../common/defines.h"

SC_MODULE(memory_controller){
enum state {IDLE, CPY, CS, RAS, CAS, OE, WE, EXEC, TSV_CONTROL, DONE};
public:
	sc_in_clk clk;
	sc_in<bool> ack_from_tsv, rsp_will_send;
	sc_in<sc_lv<2> > operation_in;
	sc_in<sc_lv<4> > operation_size_in;
	sc_in<sc_lv<4> > offset_in;
	sc_in<sc_lv<20> > address_in;
	sc_in<sc_lv<5> > pim_operation_in;
	sc_in<sc_lv<LOG_NUMBER_OF_BANKS> > bank_id_in;

	sc_out<bool> en_pop_bank_request_buffer, en_pop_bank_data_request_buffer, en_pop_pim_data_request_buffer;
	sc_out<sc_lv<LOG_NUMBER_OF_BANKS> > bank_id_out;
	sc_out<sc_lv<20> > address_out;
	sc_out<bool> cs, oe, ras, cas, we, exec, done;
	sc_out<sc_lv<4> > offset_out;
	sc_out<sc_lv<4> > pim_offset_out;
	sc_out<sc_lv<5> > pim_operation_out;
	sc_out<sc_lv<2> > operation_out;

	void prc_fsm();

	SC_CTOR(memory_controller){
		SC_METHOD(prc_fsm);
		sensitive << clk.pos();

		counter_request_lng = 0;
		vault_id = 0;
		memory_controller_id = 0;
		number_of_tsv_requests = 0;
		previous = IDLE;
		current = IDLE;
	}

	int vault_id;
	int memory_controller_id;
private:
	sc_lv<4> reg_operation_size;
	sc_lv<2> reg_operation;
	sc_lv<20> reg_address;
	sc_lv<4> reg_offset;
	sc_lv<4> reg_pim_offset;
	sc_lv<5> reg_pim_operation;

	sc_lv<LOG_NUMBER_OF_BANKS> reg_bank_id;

	state current;
	state previous;

	unsigned counter_request_lng;
	float number_of_tsv_requests;

public:
	sc_lv<2> operation_;
};



#endif /* SRC_VAULT_VAULT_CONTROLLER_MEMORY_CONTROLLER_MEMORY_CONTROLLER_H_ */
