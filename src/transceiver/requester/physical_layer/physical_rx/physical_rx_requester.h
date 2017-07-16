/*
 * physical_rx.h
 *
 *  Created on: 15 de jul de 2017
 *      Author: root
 */

#ifndef SRC_REQUESTER_PHYSICAL_LAYER_PHYSICAL_RX_PHYSICAL_RX_H_
#define SRC_REQUESTER_PHYSICAL_LAYER_PHYSICAL_RX_PHYSICAL_RX_H_

#include "deserializer_requester.h"

SC_MODULE(physical_rx_requester){
public:
	sc_in_clk clk;
	sc_in<bool> serialized_flit_available[NUMBER_OF_LINKS], en_pop_deserialize_queue[NUMBER_OF_LINKS];
	sc_in<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit[NUMBER_OF_LINKS];

	sc_out<sc_lv<FLIT_SIZE> > flit_deserialized[NUMBER_OF_LINKS];
	sc_out<bool> flit_deserialized_available[NUMBER_OF_LINKS];

	SC_CTOR(physical_rx_requester){
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_deserializer[i] = new deserializer_requester(sc_gen_unique_name("mod_deserializer"));
			mod_deserializer[i]->clk(clk);
			mod_deserializer[i]->serialized_flit_available(serialized_flit_available[i]);
			mod_deserializer[i]->en_pop_deserialize_queue(en_pop_deserialize_queue[i]);
			mod_deserializer[i]->serialized_flit(serialized_flit[i]);
			mod_deserializer[i]->flit_deserialized(flit_deserialized[i]);
			mod_deserializer[i]->flit_deserialized_available(flit_deserialized_available[i]);
			mod_deserializer[i] -> id = i;
		}
	}
public:
	deserializer_requester *mod_deserializer[NUMBER_OF_LINKS];

};



#endif /* SRC_REQUESTER_PHYSICAL_LAYER_PHYSICAL_RX_PHYSICAL_RX_H_ */
