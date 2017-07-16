/*
 * responser.h
 *
 *  Created on: 16 de jul de 2017
 *      Author: root
 */

#ifndef SRC_TRANSCEIVER_RESPONSER_RESPONSER_H_
#define SRC_TRANSCEIVER_RESPONSER_RESPONSER_H_

#include "physical_layer/physical_layer_responser.h"
#include "transaction_and_link_layer/transaction_and_link_layer_responser.h"

SC_MODULE(responser){
public:
	sc_in_clk clk;
	sc_in<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit_from_physical_tx[NUMBER_OF_LINKS];
	sc_in<bool> serialized_flit_from_physical_tx_available[NUMBER_OF_LINKS];
	sc_in<sc_lv<TAG_SIZE> > tag_per_link_from_requester[NUMBER_OF_LINKS];
	sc_in<sc_lv<FLIT_SIZE> > flit_from_vault[NUMBER_OF_VAULTS];
	sc_in<bool> flit_available_from_vault[NUMBER_OF_VAULTS];

	sc_out<bool> flit_available_to_vault[NUMBER_OF_VAULTS];
	sc_out<sc_lv<SERIALIZED_PACKET_WIDTH> > serializer_flit_to_physical_rx[NUMBER_OF_LINKS];
	sc_out<bool> serializer_flit_to_physical_rx_available[NUMBER_OF_LINKS];
	sc_out<sc_lv<FLIT_SIZE> > flit_to_vault[NUMBER_OF_VAULTS];
	sc_out<bool> en_pop_response_from_vault[NUMBER_OF_VAULTS];
	sc_out<bool> en_pop_request_tag_per_link[NUMBER_OF_LINKS];


	SC_CTOR(responser){
		mod_physical_layer = new physical_layer_responser(sc_gen_unique_name("mod_physical_layer"));
		mod_physical_layer -> clk(clk);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_physical_layer -> serialized_flit_from_physical_tx[i](serialized_flit_from_physical_tx[i]);
			mod_physical_layer -> serialized_flit_from_physical_tx_available[i](serialized_flit_from_physical_tx_available[i]);
			mod_physical_layer -> flit_from_link_master_available[i](flit_from_link_master_available_s[i]);
			mod_physical_layer -> flit_from_link_master[i](flit_from_link_master_s[i]);
			mod_physical_layer -> deserialized_flit_to_link_slave[i](deserialized_flit_to_link_slave_s[i]);
			mod_physical_layer -> vault[i](vault_whitin_quadrant_s[i]);
			mod_physical_layer -> quadrant[i](quadrant_s[i]);
			mod_physical_layer -> serializer_flit_to_physical_rx[i](serializer_flit_to_physical_rx[i]);
			mod_physical_layer -> serializer_flit_to_physical_rx_available[i](serializer_flit_to_physical_rx_available[i]);
		}
		for(int i = 0; i < NUMBER_OF_VAULTS; i++)
			mod_physical_layer -> deserialized_flit_to_link_slave_available[i](flit_available_to_vault[i]);

		mod_transaction_and_link_layer = new transaction_and_link_layer_responser(sc_gen_unique_name("mod_transaction_and_link_layer"));
		mod_transaction_and_link_layer -> clk(clk);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
				mod_transaction_and_link_layer -> tag_per_link_from_requester[i](tag_per_link_from_requester[i]);
				mod_transaction_and_link_layer -> flit_from_physical_rx[i](deserialized_flit_to_link_slave_s[i]);
				mod_transaction_and_link_layer -> select_quadrant[i](quadrant_s[i]);
				mod_transaction_and_link_layer -> select_vault_within_quadrant[i](vault_whitin_quadrant_s[i]);
				mod_transaction_and_link_layer -> en_pop_request_tag_per_link[i](en_pop_request_tag_per_link[i]);
				mod_transaction_and_link_layer -> flits_to_physical_tx[i](flit_from_link_master_s[i]);
				mod_transaction_and_link_layer -> flits_to_physical_tx_available[i](flit_from_link_master_available_s[i]);
			}
			for(int i = 0; i< NUMBER_OF_VAULTS; i++){
				mod_transaction_and_link_layer -> flit_from_vault[i](flit_from_vault[i]);
				mod_transaction_and_link_layer -> flit_available_from_vault[i](flit_available_from_vault[i]);
				mod_transaction_and_link_layer -> flit_to_vault[i](flit_to_vault[i]);
				mod_transaction_and_link_layer -> en_pop_response_from_vault[i](en_pop_response_from_vault[i]);
			}

	}

public:
	physical_layer_responser *mod_physical_layer;
	transaction_and_link_layer_responser *mod_transaction_and_link_layer;
private:
	sc_signal<bool> flit_from_link_master_available_s[NUMBER_OF_LINKS];
	sc_signal<sc_lv<FLIT_SIZE> > flit_from_link_master_s[NUMBER_OF_LINKS];
	sc_signal<sc_lv<FLIT_SIZE> > deserialized_flit_to_link_slave_s[NUMBER_OF_LINKS];
	sc_signal<sc_lv<DEMUX_SELECT_WIDTH> > vault_whitin_quadrant_s[NUMBER_OF_LINKS];
	sc_signal<sc_lv<MUX_SELECT_WIDTH> > quadrant_s[NUMBER_OF_LINKS];

};




#endif /* SRC_TRANSCEIVER_RESPONSER_RESPONSER_H_ */
