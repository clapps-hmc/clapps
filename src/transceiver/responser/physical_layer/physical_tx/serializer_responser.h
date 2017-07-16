/*
 * serializer_rx.h
 *
 *  Created on: 24 de out de 2016
 *      Author: root
 */

#ifndef SRC_SERIALIZER_H_
#define SRC_SERIALIZER_H_

#include <queue>
#include "systemc.h"
#include "../../../../common/defines.h"
#include "../../../../traffic_monitor.h"

SC_MODULE(serializer_responser){
public:
	sc_in_clk clk;
	sc_in<bool> flit_from_link_master_available;
	sc_in<sc_lv<FLIT_SIZE> > flit_from_link_master;

	sc_out<sc_lv<SERIALIZED_PACKET_WIDTH> > serializer_flit;
	sc_out<bool> serializer_flit_available;

	SC_CTOR(serializer_responser){
		SC_METHOD(prc_serializer);
		sensitive << clk.pos();
		SC_METHOD(prc_store_flits);
		sensitive << clk.pos();
		counter = 0;
		traffic = NULL;
	}
private:
	void prc_serializer();
	void prc_store_flits();
	std::queue<sc_lv<FLIT_SIZE> > buffer;

	int counter;
    traffic_monitor *traffic;
public:
    void set_traffic_monitor(traffic_monitor *t){
    	traffic = t;
    }
};


#endif /* SRC_RX_SERIALIZER_RX_H_ */
