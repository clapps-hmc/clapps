#ifndef SRC_RX_DESERIALIZER_RX_H_
#define SRC_RX_DESERIALIZER_RX_H_

#include "systemc.h"
#include "../../../../common/defines.h"
#include "../../../../traffic_monitor.h"
#include <queue>


SC_MODULE(deserializer_requester){
public:
	sc_in_clk clk;
	sc_in<bool> serialized_flit_available, en_pop_deserialize_queue;
	sc_in<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit;

	sc_out<sc_lv<FLIT_SIZE> > flit_deserialized;
	sc_out<bool> flit_deserialized_available;

	SC_CTOR(deserializer_requester){
		SC_METHOD(prc_deserializer);
		sensitive << clk.pos();
		SC_METHOD(prc_send_deserialized_flit);
		sensitive << clk.pos();

		counter = 0;
		id = -1;
		traffic = NULL;
	}
private:
	int counter;
	sc_lv<FLIT_SIZE> flit_being_reconstructed;
	std::queue<sc_lv<FLIT_SIZE> > buffer;
    traffic_monitor *traffic;


	void prc_deserializer();
	void prc_send_deserialized_flit();
public:
	int id;
    void set_traffic_monitor(traffic_monitor *t){
    	traffic = t;
    }
};
#endif /* SRC_RX_DESERIALIZER_RX_H_ */
