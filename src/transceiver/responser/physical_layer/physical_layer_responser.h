/*
 * physical_layer.h
 *
 *  Created on: 16 de jul de 2017
 *      Author: root
 */

#ifndef SRC_TRANSCEIVER_RESPONSER_PHYSICAL_LAYER_PHYSICAL_LAYER_RESPONSER_H_
#define SRC_TRANSCEIVER_RESPONSER_PHYSICAL_LAYER_PHYSICAL_LAYER_RESPONSER_H_

#include "physical_rx/physical_rx_responser.h"
#include "physical_tx/physical_tx_responser.h"

SC_MODULE(physical_layer_responser){
public:
	sc_in_clk clk;
	sc_in<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit_from_physical_tx[NUMBER_OF_LINKS];
	sc_in<bool> serialized_flit_from_physical_tx_available[NUMBER_OF_LINKS];
	sc_in<bool> flit_from_link_master_available[NUMBER_OF_LINKS];
	sc_in<sc_lv<FLIT_SIZE> > flit_from_link_master[NUMBER_OF_LINKS];


	sc_out<sc_lv<FLIT_SIZE> > deserialized_flit_to_link_slave[NUMBER_OF_LINKS];
	sc_out<bool> deserialized_flit_to_link_slave_available[NUMBER_OF_VAULTS];

	sc_out<sc_lv<DEMUX_SELECT_WIDTH> > vault[NUMBER_OF_LINKS];
	sc_out<sc_lv<MUX_SELECT_WIDTH> > quadrant[NUMBER_OF_LINKS];

	sc_out<sc_lv<SERIALIZED_PACKET_WIDTH> > serializer_flit_to_physical_rx[NUMBER_OF_LINKS];
	sc_out<bool> serializer_flit_to_physical_rx_available[NUMBER_OF_LINKS];

	SC_CTOR(physical_layer_responser){
		mod_physical_rx = new physical_rx_responser(sc_gen_unique_name("mod_physical_rx"));
		mod_physical_rx -> clk(clk);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_physical_rx -> serialized_flit_from_physical_tx[i](serialized_flit_from_physical_tx[i]);
			mod_physical_rx -> serialized_flit_from_physical_tx_available[i](serialized_flit_from_physical_tx_available[i]);
			mod_physical_rx -> deserialized_flit_to_link_slave[i](deserialized_flit_to_link_slave[i]);
			mod_physical_rx -> vault[i](vault[i]);
			mod_physical_rx -> quadrant[i](quadrant[i]);
		}
		for(int i = 0; i < NUMBER_OF_VAULTS; i++)
			mod_physical_rx -> deserialized_flit_to_link_slave_available[i](deserialized_flit_to_link_slave_available[i]);

		mod_physical_tx = new physical_tx_responser(sc_gen_unique_name("mod_physical_tx"));
		mod_physical_tx -> clk(clk);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_physical_tx -> flit_from_link_master_available[i](flit_from_link_master_available[i]);
			mod_physical_tx -> flit_from_link_master[i](flit_from_link_master[i]);
			mod_physical_tx -> serializer_flit[i](serializer_flit_to_physical_rx[i]);
			mod_physical_tx -> serializer_flit_available[i](serializer_flit_to_physical_rx_available[i]);
		}
	}
public:
	physical_tx_responser *mod_physical_tx;
	physical_rx_responser *mod_physical_rx;
};


#endif /* SRC_TRANSCEIVER_RESPONSER_PHYSICAL_LAYER_PHYSICAL_LAYER_RESPONSER_H_ */
