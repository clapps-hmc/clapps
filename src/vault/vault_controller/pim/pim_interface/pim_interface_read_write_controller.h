/*
 * pim_interface_read_write_controller.h
 *
 *  Created on: 3 de jul de 2017
 *      Author: root
 */

#ifndef SRC_VAULT_VAULT_CONTROLLER_PIM_PIM_INTERFACE_PIM_INTERFACE_READ_WRITE_CONTROLLER_H_
#define SRC_VAULT_VAULT_CONTROLLER_PIM_PIM_INTERFACE_PIM_INTERFACE_READ_WRITE_CONTROLLER_H_

#include "systemc.h"
#include <queue>
#include "../../../../common/defines.h"

SC_MODULE(pim_interface_read_write_controller){
	enum operation {READ, WRITE};
	enum state {IDLE, SENDING, WAIT, DONE};
public:
	sc_in_clk clk_pim, clk_vault;
	sc_in<bool> en_read_request, en_write_request, en_response,
				 en_write_response, en_read_response;
	sc_in<sc_lv<34> >address;
	sc_in<sc_lv<4> > size;
	sc_in<sc_lv<16*FLIT_SIZE> > data_in;
	sc_in<sc_lv<FLIT_SIZE> > response_in;

	sc_out<sc_lv<FLIT_SIZE> > request_out, data_out;
	sc_out<bool> request_available,
			     en_pop_mem_response_buffer, write_response_available, read_response_available;

	SC_CTOR(pim_interface_read_write_controller){
		SC_METHOD(prc_read_request_from_pim);
		sensitive << clk_pim.pos();
		SC_METHOD(prc_read_response_from_mem);
		sensitive << clk_vault.neg();
		SC_METHOD(prc_write_request_to_mem);
		sensitive << clk_vault.pos();
		SC_METHOD(prc_write_write_response_to_pim);
		sensitive << clk_pim.pos();
		SC_METHOD(prc_write_read_response_to_pim);
		sensitive<< clk_pim.pos();
		tag_counter = 2047;
		current = IDLE;
		counter = 0;
		num_packets = 0;
		num_flits = 0;
	}
private:
	void prc_read_request_from_pim();
	void prc_write_request_to_mem();
	void prc_read_response_from_mem();
	void prc_write_read_response_to_pim();
	void prc_write_write_response_to_pim();
	sc_lv<64> create_header(operation );
	sc_lv<64> create_tail(operation );
	sc_lv<7> find_command(operation );
	sc_lv<11> generate_tag();
	sc_lv<128> request_tmp;
	bool rsp_is_read();
	std::queue<sc_lv<FLIT_SIZE> > buffer_rqst;
	std::queue<sc_lv<FLIT_SIZE> > buffer_rsp_read;
	std::queue<sc_lv<FLIT_SIZE> > buffer_rsp_write;

	unsigned tag_counter;
	unsigned num_packets;
	unsigned num_flits;
	unsigned counter;
	state current;
};

#endif /* SRC_VAULT_VAULT_CONTROLLER_PIM_PIM_INTERFACE_PIM_INTERFACE_READ_WRITE_CONTROLLER_H_ */
