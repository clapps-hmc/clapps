/*
 * link_control.h
 *
 *  Created on: 24 de out de 2016
 *      Author: root
 */

#ifndef SRC_RX_LINK_CONTROL_H_
#define SRC_RX_LINK_CONTROL_H_

#include "systemc.h"
#include "../../../../common/defines.h"

SC_MODULE(vault_control){
enum link_controller_fsm {IDLE, SET_SIGNALS, WAIT, DONE};
public:
	sc_in_clk clk;
	sc_in<sc_lv<TAG_SIZE> > request_tag;
	sc_in<bool> response_tag_status[VAULTS_PER_QUADRANT];
	sc_in<sc_lv<FLIT_SIZE> > response_flit[VAULTS_PER_QUADRANT];

	sc_out<sc_lv<DEMUX_SELECT_WIDTH> > vault_select;
	sc_out<bool> en_pop_response[VAULTS_PER_QUADRANT];
	sc_out<bool> en_pop_request;
	sc_out<bool> en_push_serializer;

	SC_CTOR(vault_control){
		SC_METHOD(prc_vault_control);
		sensitive << clk.pos();
		counter = 0;
		current_vault = -1;
		num_of_packets = 0;
		current = IDLE;
		link_id = 0;
	}

	int link_id;
private:
	void prc_vault_control();
	unsigned counter;
	int current_vault;
	unsigned num_of_packets;
	link_controller_fsm current;
};
/*
 * Link control is responsible to set
 * each quadrant mux and also the
 * crossbar that connects each quadrant to
 * the links.
 *
 * It gets as inputs the tag of each quadrant
 * and the tags from the vaults.
 */
SC_MODULE(link_control){
public:
	sc_in_clk clk;
	sc_in<sc_lv<TAG_SIZE> > tag_per_link_from_requester[NUMBER_OF_LINKS];
	sc_in<bool> flit_available_from_vault[NUMBER_OF_VAULTS];
	sc_in<sc_lv<FLIT_SIZE> > flit_from_vault[NUMBER_OF_VAULTS];

	sc_out<sc_lv<DEMUX_SELECT_WIDTH> > vault_select[NUMBER_OF_LINKS];
	sc_out<bool> en_pop_response_from_vault[NUMBER_OF_VAULTS];
	sc_out<bool> en_pop_request_tag_per_link[NUMBER_OF_LINKS];
	sc_out<bool> en_push_serializer[NUMBER_OF_LINKS];

	SC_CTOR(link_control){
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_vault_control[i] = new vault_control(sc_gen_unique_name("mod_vault_control"));
			mod_vault_control[i] -> clk(clk);
			mod_vault_control[i] -> request_tag(tag_per_link_from_requester[i]);
			mod_vault_control[i] -> link_id = i;
			for(int j = 0; j < VAULTS_PER_QUADRANT; j++){
				mod_vault_control[i] -> response_flit[j](flit_from_vault[j + VAULTS_PER_QUADRANT*i]);
				mod_vault_control[i] -> response_tag_status[j](flit_available_from_vault[j + VAULTS_PER_QUADRANT*i]);
				mod_vault_control[i] -> en_pop_response[j](en_pop_response_from_vault[j + VAULTS_PER_QUADRANT*i]);
			}
			mod_vault_control[i] -> vault_select(vault_select[i]);
			mod_vault_control[i] -> en_pop_request(en_pop_request_tag_per_link[i]);
			mod_vault_control[i] -> en_push_serializer(en_push_serializer[i]);
		}
	}
private:
	vault_control *mod_vault_control[NUMBER_OF_LINKS];
};
#endif /* SRC_RX_LINK_CONTROL_H_ */
