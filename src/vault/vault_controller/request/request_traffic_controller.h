/*
 * request_traffic_controller.h
 *
 *  Created on: 13 de jun de 2017
 *      Author: root
 */

#ifndef SRC_VAULT_VAULT_CONTROLLER_REQUEST_REQUEST_TRAFFIC_CONTROLLER_H_
#define SRC_VAULT_VAULT_CONTROLLER_REQUEST_REQUEST_TRAFFIC_CONTROLLER_H_


#include "systemc.h"
#include <queue>
#include "../../../common/defines.h"

/*
 * This module is responsible for managing the two
 * possible flows of requests 1) from host 2)from PIM.
 * To do so, it has two request buffers, and one arbiter.
 * It checks if there is a request available from the two
 * sources. If both sources want to send data, the arbiter
 * will decide which one will send.
 *
 */

SC_MODULE(request_traffic_controller){
enum state{IDLE, SEND_HOST, SEND_PIM};
public:
	sc_in_clk clk_transiever, clk_vault, clk_pim;
	sc_in<sc_lv<FLIT_SIZE> > request_from_host, request_from_pim;
	sc_in<bool> request_from_host_available, request_from_pim_available;

	sc_out<sc_lv<FLIT_SIZE> > request_to_vault;
	sc_out<bool> request_available_to_vault;

	SC_CTOR(request_traffic_controller){
		SC_METHOD(prc_read_host_requests);
		sensitive << clk_transiever.pos();
		SC_METHOD(prc_read_pim_requests);
		sensitive << clk_pim.pos();
		SC_METHOD(prc_traffic_controller_fsm);
		sensitive << clk_vault.pos();

		priority = 0;
		pck_counter = 0;
		current = IDLE;
	}
private:
	std::queue<sc_lv<FLIT_SIZE> > host_request_buffer, pim_request_buffer;
	unsigned priority;
	unsigned pck_counter;
	state current;

	void prc_read_host_requests();
	void prc_read_pim_requests();
	void prc_traffic_controller_fsm();
};


#endif /* SRC_VAULT_VAULT_CONTROLLER_REQUEST_REQUEST_TRAFFIC_CONTROLLER_H_ */
