/*
 * transceiver.h
 *
 *  Created on: 16 de jul de 2017
 *      Author: root
 */

#ifndef SRC_TRANSCEIVER_TRANSCEIVER_H_
#define SRC_TRANSCEIVER_TRANSCEIVER_H_

#include "requester/requester.h"
#include "responser/responser.h"

SC_MODULE(transceiver){
public:
	sc_in_clk  clk;
	sc_in<sc_lv<ADDRESS_WIDTH> > address_in;
    sc_in<sc_lv<CMD_WIDTH> > cmd_in;
    sc_in<sc_lv<FLIT_SIZE> > data_in[MAX_DATA_FLITS];
    sc_in<sc_lv<NUMBER_OF_CUBES> > cub_in;
	sc_in<sc_lv<FLIT_SIZE> > flit_from_vault[NUMBER_OF_VAULTS];
	sc_in<bool> flit_available_from_vault[NUMBER_OF_VAULTS];

    sc_out<sc_lv<FLIT_SIZE> > flit_out_to_host;
	sc_out<bool> flit_available_to_vault[NUMBER_OF_VAULTS];
	sc_out<sc_lv<FLIT_SIZE> > flit_to_vault[NUMBER_OF_VAULTS];
	sc_out<bool> en_pop_response_from_vault[NUMBER_OF_VAULTS];

    SC_CTOR(transceiver){
    	mod_requester = new requester(sc_gen_unique_name("mod_requester"));
    	mod_requester -> clk(clk);
		mod_requester -> address_in(address_in);
		mod_requester -> cmd_in(cmd_in);
		for(int i = 0; i < MAX_DATA_FLITS; i++)
			mod_requester -> data_in[i](data_in[i]);
		mod_requester -> cub_in(cub_in);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_requester -> en_pop_tag_buffer_per_link[i](en_pop_tag_buffer_per_link_s[i]);
			mod_requester -> serialized_flit_from_physical_tx_available[i](serialized_flit_from_physical_tx_available_s[i]);
			mod_requester -> serialized_flit_from_physical_tx[i](serialized_flit_from_physical_tx_s[i]);
			mod_requester -> request_tag_per_link[i](request_tag_per_link_s[i]);
			mod_requester -> serialized_flit_to_physical_rx[i](serialized_flit_to_physical_rx_s[i]);
			mod_requester -> serialized_flit_available_to_physical_rx[i](serialized_flit_available_to_physical_rx_s[i]);
		}
		mod_requester -> flit_out_to_host(flit_out_to_host);

	  	mod_responser = new responser(sc_gen_unique_name("mod_responser"));
	    	mod_responser -> clk(clk);
	    	for(int i = 0; i < NUMBER_OF_LINKS; i++){
	    		mod_responser -> serialized_flit_from_physical_tx[i](serialized_flit_to_physical_rx_s[i]);
				mod_responser -> serialized_flit_from_physical_tx_available[i](serialized_flit_available_to_physical_rx_s[i]);
				mod_responser -> tag_per_link_from_requester[i](request_tag_per_link_s[i]);
				mod_responser -> serializer_flit_to_physical_rx[i](serialized_flit_from_physical_tx_s[i]);
				mod_responser -> serializer_flit_to_physical_rx_available[i](serialized_flit_from_physical_tx_available_s[i]);
				mod_responser -> en_pop_request_tag_per_link[i](en_pop_tag_buffer_per_link_s[i]);
	    	}
	    	for(int i = 0; i < NUMBER_OF_VAULTS; i++){
	    		mod_responser -> flit_from_vault[i](flit_from_vault[i]);
				mod_responser -> flit_available_from_vault[i](flit_available_from_vault[i]);
				mod_responser -> flit_available_to_vault[i](flit_available_to_vault[i]);
				mod_responser -> flit_to_vault[i](flit_to_vault[i]);
				mod_responser -> en_pop_response_from_vault[i](en_pop_response_from_vault[i]);
	    	}


    }
public:
    requester *mod_requester;
    responser *mod_responser;
private:
    sc_signal<bool> en_pop_tag_buffer_per_link_s[NUMBER_OF_LINKS];
 	sc_signal<bool> serialized_flit_from_physical_tx_available_s[NUMBER_OF_LINKS];
 	sc_signal<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit_from_physical_tx_s[NUMBER_OF_LINKS];
 	sc_signal<sc_lv<TAG_SIZE> > request_tag_per_link_s[NUMBER_OF_LINKS];
    sc_signal<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit_to_physical_rx_s[NUMBER_OF_LINKS];
    sc_signal<bool> serialized_flit_available_to_physical_rx_s[NUMBER_OF_LINKS];
};



#endif /* SRC_TRANSCEIVER_TRANSCEIVER_H_ */
