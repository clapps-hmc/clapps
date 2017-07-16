
#ifndef SRC_REQUESTER_TRANSACTION_AND_LINK_LAYER_LINK_SLAVE_LINK_SLAVE_H_
#define SRC_REQUESTER_TRANSACTION_AND_LINK_LAYER_LINK_SLAVE_LINK_SLAVE_H_

#include "reorder_responser.h"
#include "mux_4x1_w_set_bit.h"

/*
 * From
 */
SC_MODULE(link_slave_requester){
public:
	sc_in_clk clk;
	sc_in<sc_lv<TAG_SIZE> > request_tag;
	sc_in<sc_lv<FLIT_SIZE> > response_flit[NUMBER_OF_LINKS];
	sc_in<bool> physical_rx_flit_available[NUMBER_OF_LINKS];


	sc_out<bool> en_pop_reorder_queue;
	sc_out<bool> en_pop_physical_rx_queue[NUMBER_OF_LINKS];
	sc_out<sc_lv<FLIT_SIZE> > flit_out;

	SC_CTOR(link_slave_requester){
		mod_reorder_response = new reorder_response(sc_gen_unique_name("mod_reorder_response"));
		mod_reorder_response -> clk(clk);
		mod_reorder_response -> request_tag(request_tag);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_reorder_response -> response_flit[i](response_flit[i]);
			mod_reorder_response -> physical_rx_flit_available[i](physical_rx_flit_available[i]);
			mod_reorder_response -> en_pop_physical_rx_queue[i](en_pop_physical_rx_queue[i]);

		}
		mod_reorder_response -> en_pop_reorder_queue(en_pop_reorder_queue);
		mod_reorder_response -> mux_select(mux_select_s);
		mod_reorder_response -> set_mux(set_mux_s);

		mod_mux_4x1_w_set_bit = new mux_4x1_w_set_bit(sc_gen_unique_name("mod_mux_4x1_w_set_bit"));
		for(int i = 0; i < NUMBER_OF_LINKS; i++)
			mod_mux_4x1_w_set_bit -> flit_in[i](response_flit[i]);
		mod_mux_4x1_w_set_bit -> set_bit(set_mux_s);
		mod_mux_4x1_w_set_bit -> select(mux_select_s);
		mod_mux_4x1_w_set_bit -> flit_out(flit_out);
	}
public:
	reorder_response *mod_reorder_response;
	mux_4x1_w_set_bit *mod_mux_4x1_w_set_bit;

	sc_signal<sc_lv<2> > mux_select_s;
	sc_signal<bool> set_mux_s;
};



#endif
