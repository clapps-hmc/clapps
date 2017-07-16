/*
 * data_request_buffer.h
 *
 *  Created on: 9 de nov de 2016
 *      Author: root
 */

#ifndef SRC_VAULT_VAULT_CONTROLLER_REQUEST_DATA_REQUEST_BUFFER_H_
#define SRC_VAULT_VAULT_CONTROLLER_REQUEST_DATA_REQUEST_BUFFER_H_

#include "../../../common/defines.h"
#include "../../../common/types.h"

#include "systemc.h"
#include <queue>

/*
 * This module stores data from request
 * to write in the memory.
 * Data in -> 128 bits (16 bytes)
 * Data out -> 256 bits (32 bytes)
 * Push enable policy:
 * 	00 - store 64 LSB bits
 * 	01 - store all 128 bits
 * 	10 - store 64 MSB bits
 * 	11 - disabled
 */

SC_MODULE(data_request_buffer){
public:
	sc_in_clk clk_vault;
	sc_in<bool> en_pop;
	sc_in<sc_lv<2> > en_push;
	sc_in<sc_lv<128> > data_in;

	sc_out<sc_lv<256> > data_out;

	SC_CTOR(data_request_buffer){
		SC_METHOD(prc_data_request_buffer_write);
		sensitive << clk_vault.pos();
		SC_METHOD(prc_data_request_buffer_read);
		sensitive << clk_vault.pos();
		counter = 0;
		vault_id = 0;
	}

	int vault_id;
private:
	void prc_data_request_buffer_write();
	void prc_data_request_buffer_read();
	void prc_reset_data();
	std::queue<sc_lv<256> > buffer;
	sc_lv<256> reconstructed_data;
	unsigned counter;

};

#endif /* SRC_VAULT_VAULT_CONTROLLER_REQUEST_DATA_REQUEST_BUFFER_H_ */
