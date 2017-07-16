#ifndef SRC_REQUESTER_TRANSACTION_AND_LINK_LAYER_TRANSACTION_AND_LINK_LAYER_H_
#define SRC_REQUESTER_TRANSACTION_AND_LINK_LAYER_TRANSACTION_AND_LINK_LAYER_H_

#include "link_master/link_master_requester.h"
#include "link_slave/link_slave_requester.h"

SC_MODULE(transaction_and_link_layer_requester){
	sc_in_clk  clk;
	sc_in<sc_lv<ADDRESS_WIDTH> > address_in; // UP to 16GB MEMORY
    sc_in<sc_lv<CMD_WIDTH> > cmd_in; //input command
    sc_in<sc_lv<FLIT_SIZE> > data_in[MAX_DATA_FLITS]; //MIN of 1 data FLIT up to 16 data FLIT
    sc_in<sc_lv<NUMBER_OF_CUBES> > cub_in;

	sc_in<sc_lv<FLIT_SIZE> > response_flit[NUMBER_OF_LINKS];
	sc_in<bool> physical_rx_flit_available[NUMBER_OF_LINKS];
    sc_in<bool> en_pop_tag_buffer_per_link[NUMBER_OF_LINKS];


    sc_out<sc_lv<FLIT_SIZE> > flit_out_to_physical_tx[NUMBER_OF_LINKS];
    sc_out<bool> flit_available_to_physical_tx[NUMBER_OF_LINKS];
    sc_out<sc_lv<TAG_SIZE> > request_tag_per_link[NUMBER_OF_LINKS];
    sc_out<bool> en_pop_physical_rx_queue[NUMBER_OF_LINKS];
    sc_out<sc_lv<FLIT_SIZE> > flit_out_to_host;

    SC_CTOR(transaction_and_link_layer_requester){
    	mod_link_master = new link_master_requester(sc_gen_unique_name("mod_link_master"));
    	mod_link_master -> clk(clk);
    	mod_link_master -> address_in(address_in);
		mod_link_master -> cmd_in(cmd_in);
		for(int i = 0; i < MAX_DATA_FLITS; i++)
			mod_link_master -> data_in[i](data_in[i]);
    	mod_link_master -> cub_in(cub_in);
    	for(int i = 0; i < NUMBER_OF_LINKS; i++){
    		mod_link_master -> en_pop_tag_buffer_per_link[i](en_pop_tag_buffer_per_link[i]);
    		mod_link_master -> flit_out[i](flit_out_to_physical_tx[i]);
    		mod_link_master -> flit_available[i](flit_available_to_physical_tx[i]);
    		mod_link_master -> request_tag_per_link[i](request_tag_per_link[i]);
    	}

		mod_link_master -> en_pop_tag_buffer(en_pop_tag_buffer_s);
		mod_link_master -> request_global_tag(request_global_tag_s);

		mod_link_slave = new link_slave_requester(sc_gen_unique_name("mod_link_slave"));
		mod_link_slave -> clk(clk);
		mod_link_slave -> request_tag(request_global_tag_s);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_link_slave -> response_flit[i](response_flit[i]);
			mod_link_slave -> physical_rx_flit_available[i](physical_rx_flit_available[i]);
			mod_link_slave -> en_pop_physical_rx_queue[i](en_pop_physical_rx_queue[i]);
		}
		mod_link_slave -> en_pop_reorder_queue(en_pop_tag_buffer_s);
		mod_link_slave -> flit_out(flit_out_to_host);
    }

public:
	link_master_requester *mod_link_master;
	link_slave_requester *mod_link_slave;
private:
    sc_signal<bool> en_pop_tag_buffer_s;
    sc_signal<sc_lv<TAG_SIZE> > request_global_tag_s;

};



#endif
