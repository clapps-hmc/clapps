#ifndef SRC_RX_REORDER_RESPONSER_H_
#define SRC_RX_REORDER_RESPONSER_H_

#include "systemc.h"
#include "../../../../common/defines.h"

/*
 * Responsible to keep order between response packets coming from the links.
 *
 * From link_master: request_tag
 * From physical_rx: response_flit, physical_rx_flit_available
 *
 * To mux: mux_select, set_mux
 * To physical_rx: en_pop_physical_rx
 * To link_master: en_pop_reorder_queue
 */

SC_MODULE(reorder_response){
enum link_controller_fsm {IDLE, SET_SIGNALS, WAIT_, WAIT, DONE};

public:
	sc_in_clk clk;
	sc_in<sc_lv<TAG_SIZE> > request_tag;
	sc_in<sc_lv<FLIT_SIZE> > response_flit[NUMBER_OF_LINKS];
	sc_in<bool> physical_rx_flit_available[NUMBER_OF_LINKS];

	sc_out<sc_lv<2> > mux_select;
	sc_out<bool> set_mux;
	sc_out<bool> en_pop_reorder_queue;
	sc_out<bool> en_pop_physical_rx_queue[NUMBER_OF_LINKS];

	SC_CTOR(reorder_response){
		SC_METHOD(prc_reorder_response);
		sensitive << clk.pos();

		counter = 0;
		num_flits = 0;
		link_to_send = 0;
		sending = false;
		current = IDLE;
	}
private:
	unsigned counter;
	unsigned num_flits;
	bool sending;
	int link_to_send;
	void prc_reorder_response();
	link_controller_fsm current;

};
#endif
