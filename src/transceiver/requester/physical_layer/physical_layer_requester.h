/*
 * physical_layer.h
 *
 *  Created on: 15 de jul de 2017
 *      Author: root
 */

#ifndef SRC_REQUESTER_PHYSICAL_LAYER_PHYSICAL_LAYER_H_
#define SRC_REQUESTER_PHYSICAL_LAYER_PHYSICAL_LAYER_H_

#include "physical_rx/physical_rx_requester.h"
#include "physical_tx/physical_tx_requester.h"

SC_MODULE(physical_layer_requester){
public:
	sc_in_clk clk;
	sc_in<sc_lv<FLIT_SIZE> > flit_from_link_master[NUMBER_OF_LINKS];
	sc_in<bool> flit_available_from_link_master[NUMBER_OF_LINKS];
	sc_in<bool> serialized_flit_from_physical_tx_available[NUMBER_OF_LINKS], en_pop_deserialize_queue[NUMBER_OF_LINKS];
	sc_in<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit_from_physical_tx[NUMBER_OF_LINKS];

    sc_out<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit_to_physical_rx[NUMBER_OF_LINKS];
    sc_out<bool> serialized_flit_available_to_physical_rx[NUMBER_OF_LINKS];
	sc_out<sc_lv<FLIT_SIZE> > flit_deserialized[NUMBER_OF_LINKS];
	sc_out<bool> flit_deserialized_available[NUMBER_OF_LINKS];

    SC_CTOR(physical_layer_requester){
    	mod_physical_tx = new physical_tx_requester(sc_gen_unique_name("mod_physical_tx"));
    	mod_physical_tx -> clk(clk);
    	for(int i = 0; i < NUMBER_OF_LINKS; i++){
    		mod_physical_tx -> flit_from_link_master[i](flit_from_link_master[i]);
			mod_physical_tx -> flit_available_from_link_master[i](flit_available_from_link_master[i]);
			mod_physical_tx -> serialized_flit_to_physical_rx[i](serialized_flit_to_physical_rx[i]);
			mod_physical_tx -> serialized_flit_available_to_physical_rx[i](serialized_flit_available_to_physical_rx[i]);
    	}

    	mod_physical_rx = new physical_rx_requester(sc_gen_unique_name("mod_physical_rx"));
    	mod_physical_rx -> clk(clk);
    	for(int i = 0; i < NUMBER_OF_LINKS; i++){
    		mod_physical_rx -> serialized_flit_available[i](serialized_flit_from_physical_tx_available[i]);
			mod_physical_rx -> en_pop_deserialize_queue[i](en_pop_deserialize_queue[i]);
			mod_physical_rx -> serialized_flit[i](serialized_flit_from_physical_tx[i]);
			mod_physical_rx -> flit_deserialized[i](flit_deserialized[i]);
			mod_physical_rx -> flit_deserialized_available[i](flit_deserialized_available[i]);
    	}
    }
public:
	physical_tx_requester *mod_physical_tx;
	physical_rx_requester *mod_physical_rx;
};


#endif /* SRC_REQUESTER_PHYSICAL_LAYER_PHYSICAL_LAYER_H_ */
