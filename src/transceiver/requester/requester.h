/*
 * requester.h
 *
 *  Created on: 15 de jul de 2017
 *      Author: root
 */

#ifndef SRC_REQUESTER_REQUESTER_H_
#define SRC_REQUESTER_REQUESTER_H_

#include "physical_layer/physical_layer_requester.h"
#include "transaction_and_link_layer/transaction_and_link_layer_requester.h"

SC_MODULE(requester){
public:
	sc_in_clk  clk;
	sc_in<sc_lv<ADDRESS_WIDTH> > address_in;
    sc_in<sc_lv<CMD_WIDTH> > cmd_in;
    sc_in<sc_lv<FLIT_SIZE> > data_in[MAX_DATA_FLITS];
    sc_in<sc_lv<NUMBER_OF_CUBES> > cub_in;
    sc_in<bool> en_pop_tag_buffer_per_link[NUMBER_OF_LINKS];
	sc_in<bool> serialized_flit_from_physical_tx_available[NUMBER_OF_LINKS];
	sc_in<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit_from_physical_tx[NUMBER_OF_LINKS];

	sc_out<sc_lv<TAG_SIZE> > request_tag_per_link[NUMBER_OF_LINKS];
    sc_out<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit_to_physical_rx[NUMBER_OF_LINKS];
    sc_out<bool> serialized_flit_available_to_physical_rx[NUMBER_OF_LINKS];
    sc_out<sc_lv<FLIT_SIZE> > flit_out_to_host;

	SC_CTOR(requester){
		mod_transaction_and_link_layer =  new transaction_and_link_layer_requester(sc_gen_unique_name("mod_transaction_and_link_layer"));
		mod_transaction_and_link_layer -> clk (clk);
		mod_transaction_and_link_layer -> address_in(address_in);
		mod_transaction_and_link_layer -> cmd_in(cmd_in);
		for(int i = 0; i < MAX_DATA_FLITS; i++)
			mod_transaction_and_link_layer -> data_in[i](data_in[i]);
		mod_transaction_and_link_layer -> cub_in(cub_in);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_transaction_and_link_layer -> en_pop_tag_buffer_per_link[i](en_pop_tag_buffer_per_link[i]);
			mod_transaction_and_link_layer -> request_tag_per_link[i](request_tag_per_link[i]);
			mod_transaction_and_link_layer -> response_flit[i](response_flit_s[i]);
			mod_transaction_and_link_layer -> physical_rx_flit_available[i](physical_rx_flit_available_s[i]);
			mod_transaction_and_link_layer -> flit_out_to_physical_tx[i](flit_out_to_physical_tx_s[i]);
			mod_transaction_and_link_layer -> flit_available_to_physical_tx[i](flit_available_to_physical_tx_s[i]);
			mod_transaction_and_link_layer -> en_pop_physical_rx_queue[i](en_pop_physical_rx_queue_s[i]);
		}
		mod_transaction_and_link_layer -> flit_out_to_host(flit_out_to_host);

		mod_physical_layer = new physical_layer_requester(sc_gen_unique_name("mod_physical_layer"));
			mod_physical_layer -> clk(clk);
			for(int i = 0; i < NUMBER_OF_LINKS; i++){
				mod_physical_layer -> serialized_flit_from_physical_tx_available[i](serialized_flit_from_physical_tx_available[i]);
				mod_physical_layer -> serialized_flit_from_physical_tx[i](serialized_flit_from_physical_tx[i]);
				mod_physical_layer -> serialized_flit_to_physical_rx[i](serialized_flit_to_physical_rx[i]);
				mod_physical_layer -> serialized_flit_available_to_physical_rx[i](serialized_flit_available_to_physical_rx[i]);
				mod_physical_layer -> flit_from_link_master[i](flit_out_to_physical_tx_s[i]);
				mod_physical_layer -> flit_available_from_link_master[i](flit_available_to_physical_tx_s[i]);
				mod_physical_layer -> en_pop_deserialize_queue[i](en_pop_physical_rx_queue_s[i]);
				mod_physical_layer -> flit_deserialized[i](response_flit_s[i]);
				mod_physical_layer -> flit_deserialized_available[i](physical_rx_flit_available_s[i]);

			}
	}

public:
	transaction_and_link_layer_requester *mod_transaction_and_link_layer;
	physical_layer_requester *mod_physical_layer;
private:
	sc_signal<sc_lv<FLIT_SIZE> > response_flit_s[NUMBER_OF_LINKS];
	sc_signal<bool> physical_rx_flit_available_s[NUMBER_OF_LINKS];
	sc_signal<sc_lv<FLIT_SIZE> > flit_out_to_physical_tx_s[NUMBER_OF_LINKS];
    sc_signal<bool> flit_available_to_physical_tx_s[NUMBER_OF_LINKS];
    sc_signal<bool> en_pop_physical_rx_queue_s[NUMBER_OF_LINKS];

};


#endif /* SRC_REQUESTER_REQUESTER_H_ */
