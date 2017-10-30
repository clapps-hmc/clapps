/*
 * rvu.h
 *
 *  Created on: 4 de jul de 2017
 *      Author: root
 */

#ifndef SRC_VAULT_VAULT_CONTROLLER_PIM_RVU_H_
#define SRC_VAULT_VAULT_CONTROLLER_PIM_RVU_H_

#include "systemc.h"
#include "rvu_defines.h"
#include "../../../common/defines.h"
#include "../../../traffic_monitor.h"

SC_MODULE(rvu){
enum state{FETCH, DECODE, EXEC, REQUEST_READ, REQUEST_WRITE, WAIT_READ, WAIT_WRITE, WRITE_BACK};
enum instruction_type{NONE, READ, WRITE, ALU};
public:
	/*
	 * General
	 */
	sc_in_clk clk_rvu;

	/*
	 * PIM Interface
	 */
	sc_in<sc_lv<128> > rvu_instruction;
	sc_in<bool> rvu_instruction_available;
	sc_in<sc_lv<128> > read_response;
	sc_in<bool> read_response_available, write_response_available;

	sc_out<bool> en_pop_instruction_buffer;
	sc_out<bool> en_read_request, en_pop_read_response, en_write_request, en_pop_write_response;
	sc_out<sc_lv<34> > address;
	sc_out<sc_lv<4> > size;
	sc_out<sc_lv<16*128> > write_request;

	SC_CTOR(rvu){
		SC_METHOD(prc_rvu);
		sensitive << clk_rvu.pos();

		current = FETCH;
		type = NONE;
		rs = 0;
		rt = 0;
		rd = 0;
		operation_size = 0;
		counter = 0;
		rvu_id = 0;
		traffic = NULL;
		rvu_stale = true; //true - stale false - running
	}

private:
	sc_lv<SCALAR_WIDTH*NUM_SCALAR_REGS> reg_bank[NUM_VECT_REGS];
	state current;
	instruction_type type;
	sc_lv<128> instruction;
	unsigned rs, rt, rd;
	unsigned operation_size;
	unsigned counter;
	sc_lv<4> request_size;
	sc_lv<34> addr;
	sc_lv<3> alu_op;
	bool rvu_stale;
	void prc_rvu();
	void decode_instruction();
	void exec();

    traffic_monitor *traffic;
public:
    unsigned rvu_id;
    void set_traffic_monitor(traffic_monitor *t){
    	traffic = t;
    }
    bool rvu_is_stale(){
    	return rvu_stale;
    }

};


#endif /* SRC_VAULT_VAULT_CONTROLLER_PIM_RVU_H_ */
