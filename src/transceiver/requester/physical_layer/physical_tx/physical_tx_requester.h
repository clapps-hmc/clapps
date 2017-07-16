#ifndef SRC_REQUESTER_PHYSICAL_LAYER_PHYSICAL_TX_PHYSICAL_TX_H_
#define SRC_REQUESTER_PHYSICAL_LAYER_PHYSICAL_TX_PHYSICAL_TX_H_

#include "../../../../common/defines.h"
#include "serializer_requester.h"

SC_MODULE(physical_tx_requester){
public:
	sc_in_clk clk;
	sc_in<sc_lv<FLIT_SIZE> > flit_from_link_master[NUMBER_OF_LINKS];
	sc_in<bool> flit_available_from_link_master[NUMBER_OF_LINKS];

    sc_out<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit_to_physical_rx[NUMBER_OF_LINKS];
    sc_out<bool> serialized_flit_available_to_physical_rx[NUMBER_OF_LINKS];

    SC_CTOR(physical_tx_requester){
    	for(int i = 0; i < NUMBER_OF_LINKS; i++){
    		mod_serializer[i] = new serializer_requester(sc_gen_unique_name("mod_serializer"));
    		mod_serializer[i] -> clk (clk);
    		mod_serializer[i] -> flit(flit_from_link_master[i]);
    		mod_serializer[i] -> flit_available(flit_available_from_link_master[i]);
    		mod_serializer[i] -> serialized_flit(serialized_flit_to_physical_rx[i]);
    		mod_serializer[i] -> serialized_flit_available(serialized_flit_available_to_physical_rx[i]);
    	}
    }

private:
    serializer_requester *mod_serializer[NUMBER_OF_LINKS];
};

#endif
