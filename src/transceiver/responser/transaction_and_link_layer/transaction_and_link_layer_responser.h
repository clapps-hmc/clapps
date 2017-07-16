/*
 * transaction_and_link_layer.h
 *
 *  Created on: 16 de jul de 2017
 *      Author: root
 */

#ifndef SRC_TRANSCEIVER_RESPONSER_TRANSACTION_AND_LINK_LAYER_TRANSACTION_AND_LINK_LAYER_RESPONSER_H_
#define SRC_TRANSCEIVER_RESPONSER_TRANSACTION_AND_LINK_LAYER_TRANSACTION_AND_LINK_LAYER_RESPONSER_H_

#include "link_master/link_master_responser.h"
#include "link_slave/link_slave_responser.h"

SC_MODULE(transaction_and_link_layer_responser){
public:
	sc_in_clk clk;
	sc_in<sc_lv<TAG_SIZE> > tag_per_link_from_requester[NUMBER_OF_LINKS];
	sc_in<sc_lv<FLIT_SIZE> > flit_from_vault[NUMBER_OF_VAULTS];
	sc_in<bool> flit_available_from_vault[NUMBER_OF_VAULTS];
	sc_in<sc_lv<FLIT_SIZE> > flit_from_physical_rx[NUMBER_OF_LINKS];
	sc_in<sc_lv<MUX_SELECT_WIDTH> > select_quadrant[NUMBER_OF_LINKS];
	sc_in<sc_lv<DEMUX_SELECT_WIDTH> > select_vault_within_quadrant[NUMBER_OF_LINKS];

	sc_out<sc_lv<FLIT_SIZE> > flit_to_vault[NUMBER_OF_VAULTS];
	sc_out<bool> en_pop_response_from_vault[NUMBER_OF_VAULTS];
	sc_out<bool> en_pop_request_tag_per_link[NUMBER_OF_LINKS];
	sc_out<sc_lv<FLIT_SIZE> > flits_to_physical_tx[NUMBER_OF_LINKS];
	sc_out<bool> flits_to_physical_tx_available[NUMBER_OF_LINKS];

	SC_CTOR(transaction_and_link_layer_responser){
		mod_link_slave = new link_slave_responser(sc_gen_unique_name("mod_link_slave"));
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_link_slave -> flit_from_physical_rx[i](flit_from_physical_rx[i]);
			mod_link_slave -> select_quadrant[i](select_quadrant[i]);
			mod_link_slave -> select_vault_within_quadrant[i](select_vault_within_quadrant[i]);
		}
		for(int i = 0; i < NUMBER_OF_VAULTS; i++)
			mod_link_slave -> flit_to_vault[i](flit_to_vault[i]);

		mod_link_master = new link_master_responser(sc_gen_unique_name("mod_link_slave"));
		mod_link_master -> clk(clk);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_link_master -> tag_per_link_from_requester[i](tag_per_link_from_requester[i]);
			mod_link_master -> en_pop_request_tag_per_link[i](en_pop_request_tag_per_link[i]);
			mod_link_master -> flits_to_physical_tx[i](flits_to_physical_tx[i]);
			mod_link_master -> flits_to_physical_tx_available[i](flits_to_physical_tx_available[i]);
		}
		for(int i = 0; i < NUMBER_OF_VAULTS; i++){
			mod_link_master -> flit_from_vault[i](flit_from_vault[i]);
			mod_link_master -> flit_available_from_vault[i](flit_available_from_vault[i]);
			mod_link_master -> en_pop_response_from_vault[i](en_pop_response_from_vault[i]);
		}

	}
private:
	link_slave_responser *mod_link_slave;
	link_master_responser *mod_link_master;

};



#endif /* SRC_TRANSCEIVER_RESPONSER_TRANSACTION_AND_LINK_LAYER_TRANSACTION_AND_LINK_LAYER_RESPONSER_H_ */
