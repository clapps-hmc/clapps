/*
 * physical_tx.h
 *
 *  Created on: 16 de jul de 2017
 *      Author: root
 */

#ifndef SRC_TRANSCEIVER_RESPONSER_PHYSICAL_LAYER_PHYSICAL_TX_PHYSICAL_TX_RESPONSER_H_
#define SRC_TRANSCEIVER_RESPONSER_PHYSICAL_LAYER_PHYSICAL_TX_PHYSICAL_TX_RESPONSER_H_

#include "serializer_responser.h"

SC_MODULE(physical_tx_responser){
public:
	sc_in_clk clk;
	sc_in<bool> flit_from_link_master_available[NUMBER_OF_LINKS];
	sc_in<sc_lv<FLIT_SIZE> > flit_from_link_master[NUMBER_OF_LINKS];

	sc_out<sc_lv<SERIALIZED_PACKET_WIDTH> > serializer_flit[NUMBER_OF_LINKS];
	sc_out<bool> serializer_flit_available[NUMBER_OF_LINKS];

	SC_CTOR(physical_tx_responser){
		for(int i=0; i < NUMBER_OF_LINKS; i++){
			mod_serializer[i] = new serializer_responser(sc_gen_unique_name("mod_serializer"));
			mod_serializer[i]->clk(clk);
			mod_serializer[i]->flit_from_link_master_available(flit_from_link_master_available[i]);
			mod_serializer[i]->flit_from_link_master(flit_from_link_master[i]);
			mod_serializer[i]->serializer_flit(serializer_flit[i]);
			mod_serializer[i]->serializer_flit_available(serializer_flit_available[i]);
		}
	}
public:
	serializer_responser *mod_serializer[NUMBER_OF_LINKS];
};



#endif
