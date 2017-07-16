/*
 * link_master.h
 *
 *  Created on: 16 de jul de 2017
 *      Author: root
 */

#ifndef SRC_TRANSCEIVER_RESPONSER_TRANSACTION_AND_LINK_LAYER_LINK_MASTER_LINK_MASTER_RESPONSER_H_
#define SRC_TRANSCEIVER_RESPONSER_TRANSACTION_AND_LINK_LAYER_LINK_MASTER_LINK_MASTER_RESPONSER_H_

#include "link_control.h"
#include "mux_8x1.h"

SC_MODULE(link_master_responser){
public:
	sc_in_clk clk;
	sc_in<sc_lv<TAG_SIZE> > tag_per_link_from_requester[NUMBER_OF_LINKS];
	sc_in<sc_lv<FLIT_SIZE> > flit_from_vault[NUMBER_OF_VAULTS];
	sc_in<bool> flit_available_from_vault[NUMBER_OF_VAULTS];

	sc_out<bool> en_pop_response_from_vault[NUMBER_OF_VAULTS];
	sc_out<bool> en_pop_request_tag_per_link[NUMBER_OF_LINKS];
	sc_out<sc_lv<FLIT_SIZE> > flits_to_physical_tx[NUMBER_OF_LINKS];
	sc_out<bool> flits_to_physical_tx_available[NUMBER_OF_LINKS];

	SC_CTOR(link_master_responser){
		mod_link_control = new link_control(sc_gen_unique_name("mod_link_control"));
		mod_link_control -> clk(clk);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_link_control -> tag_per_link_from_requester[i](tag_per_link_from_requester[i]);
			for(int j = 0; j < VAULTS_PER_QUADRANT; j++){
				mod_link_control -> flit_from_vault[j + i*VAULTS_PER_QUADRANT](flit_from_vault[j+i*VAULTS_PER_QUADRANT]);
				mod_link_control -> flit_available_from_vault[j + i*VAULTS_PER_QUADRANT](flit_available_from_vault[j+VAULTS_PER_QUADRANT*i]);
				mod_link_control -> en_pop_response_from_vault[j + i*VAULTS_PER_QUADRANT](en_pop_response_from_vault[j + i*VAULTS_PER_QUADRANT]);

			}
			mod_link_control -> vault_select[i](vault_select_s[i]);
			mod_link_control -> en_pop_request_tag_per_link[i](en_pop_request_tag_per_link[i]);
			mod_link_control -> en_push_serializer[i](flits_to_physical_tx_available[i]);
		}

		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_mux_8x1[i] = new mux_8x1(sc_gen_unique_name("mod_mux_8x1"));
			for(int j = 0; j < VAULTS_PER_QUADRANT; j++){
				mod_mux_8x1[i] -> in[j](flit_from_vault[j + VAULTS_PER_QUADRANT*i]);
			}
			mod_mux_8x1[i] -> sel(vault_select_s[i]);
			mod_mux_8x1[i] -> out(flits_to_physical_tx[i]);
		}
	}

private:
	link_control *mod_link_control;
	mux_8x1 *mod_mux_8x1[NUMBER_OF_LINKS];;
	sc_signal<sc_lv<DEMUX_SELECT_WIDTH> > vault_select_s[NUMBER_OF_LINKS];

};


#endif /* SRC_TRANSCEIVER_RESPONSER_TRANSACTION_AND_LINK_LAYER_LINK_MASTER_LINK_MASTER_RESPONSER_H_ */
