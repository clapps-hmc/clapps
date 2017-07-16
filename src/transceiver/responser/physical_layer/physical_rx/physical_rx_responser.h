#ifndef PHYSICAL_RX_H
#define PHYSICAL_RX_H

#include "../../../../common/defines.h"
#include "deserializer_responser.h"

SC_MODULE(physical_rx_responser){
public:
	sc_in_clk clk;
	sc_in<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit_from_physical_tx[NUMBER_OF_LINKS];
	sc_in<bool> serialized_flit_from_physical_tx_available[NUMBER_OF_LINKS];


	sc_out<sc_lv<FLIT_SIZE> > deserialized_flit_to_link_slave[NUMBER_OF_LINKS];
	sc_out<bool> deserialized_flit_to_link_slave_available[NUMBER_OF_VAULTS];

	sc_out<sc_lv<DEMUX_SELECT_WIDTH> > vault[NUMBER_OF_LINKS];
	sc_out<sc_lv<MUX_SELECT_WIDTH> > quadrant[NUMBER_OF_LINKS];

	SC_CTOR(physical_rx_responser){
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_deserializer[i] = new deserializer_responser(sc_gen_unique_name("mod_deserializer"));
			mod_deserializer[i]->clk(clk);
			mod_deserializer[i]->serialized_flit(serialized_flit_from_physical_tx[i]);
			mod_deserializer[i]->serialized_flit_available(serialized_flit_from_physical_tx_available[i]);
			mod_deserializer[i]->deserialized_flit(deserialized_flit_to_link_slave[i]);
			for(int j = 0; j < VAULTS_PER_QUADRANT; j++)
				mod_deserializer[i]->deserialized_flit_available[j](deserialized_flit_to_link_slave_available[i*8+j]);
			mod_deserializer[i]->quadrant(quadrant[i]);
			mod_deserializer[i]->vault(vault[i]);

			mod_deserializer[i]->id = i;
		}
	}
public:
	deserializer_responser *mod_deserializer[NUMBER_OF_LINKS];
};

#endif
