/*
 * link_slave.h
 *
 *  Created on: 16 de jul de 2017
 *      Author: root
 */

#ifndef SRC_TRANSCEIVER_RESPONSER_TRANSACTION_AND_LINK_LAYER_LINK_SLAVE_LINK_SLAVE_RESPONSER_H_
#define SRC_TRANSCEIVER_RESPONSER_TRANSACTION_AND_LINK_LAYER_LINK_SLAVE_LINK_SLAVE_RESPONSER_H_


#include "xbar_4x4.h"
#include "demux_1x8.h"

SC_MODULE(link_slave_responser){
public:
	sc_in<sc_lv<FLIT_SIZE> > flit_from_physical_rx[NUMBER_OF_LINKS];
	sc_in<sc_lv<MUX_SELECT_WIDTH> > select_quadrant[NUMBER_OF_LINKS];
	sc_in<sc_lv<DEMUX_SELECT_WIDTH> > select_vault_within_quadrant[NUMBER_OF_LINKS];

	sc_out<sc_lv<FLIT_SIZE> > flit_to_vault[NUMBER_OF_VAULTS];
	SC_CTOR(link_slave_responser){
		mod_xbar_4x4 = new xbar_4x4("mod_xbar_4x4");
		for(int i=0; i < NUMBER_OF_LINKS; i++){
			 mod_xbar_4x4 -> flit_from_physical_rx[i](flit_from_physical_rx[i]);
			 mod_xbar_4x4 -> select[i](select_quadrant[i]);
			 mod_xbar_4x4 -> flit_to_quadrant[i](flit_to_quadrant_s[i]);
		}

		for(int i=0; i < NUMBER_OF_LINKS; i++){
			mod_demux_1x8[i] = new demux_1x8(sc_gen_unique_name("mod_demux_1x8"));
			mod_demux_1x8[i] -> data_in(flit_to_quadrant_s[i]);
			mod_demux_1x8[i] -> select(select_vault_within_quadrant[i]);
		    for(int j = 0; j < 8; j++){
		    	mod_demux_1x8[i] -> data_out[j](flit_to_vault[j + i*8]);
		    }
		}
	}
private:
	xbar_4x4 *mod_xbar_4x4;
    demux_1x8 *mod_demux_1x8[NUMBER_OF_LINKS];
	sc_signal<sc_lv<FLIT_SIZE> > flit_to_quadrant_s[NUMBER_OF_LINKS];
};



#endif /* SRC_TRANSCEIVER_RESPONSER_TRANSACTION_AND_LINK_LAYER_LINK_SLAVE_LINK_SLAVE_RESPONSER_H_ */
