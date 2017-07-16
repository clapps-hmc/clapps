#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "systemc.h"
#include <queue>
#include "../../../../common/defines.h"
#include "../../../../traffic_monitor.h"


SC_MODULE(deserializer_responser){
public:
	enum state{IDLE, SENDING, WAIT};
	sc_in_clk clk;
	sc_in<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit;
	sc_in<bool> serialized_flit_available;


	sc_out<sc_lv<FLIT_SIZE> > deserialized_flit;
	sc_out<bool> deserialized_flit_available[VAULTS_PER_QUADRANT];
	sc_out<sc_lv<DEMUX_SELECT_WIDTH> > vault;
	sc_out<sc_lv<MUX_SELECT_WIDTH> > quadrant;

	SC_CTOR(deserializer_responser){
		SC_METHOD(prc_deserialize);
		sensitive << clk.pos();
		SC_METHOD(prc_send_flits);
		sensitive << clk.pos();

		counter = 0;
		num_flits = 0;
		id = 0;
		counter_send = 0;
		current = IDLE;

		traffic = NULL;
	}
	int id;
private:

	void prc_deserialize();
	void prc_send_flits();


	unsigned counter;
	unsigned counter_send;
	unsigned num_flits;
	sc_lv<3> reg_vault;
	sc_lv<2> reg_quadrant;

	state current;
	sc_lv<FLIT_SIZE> flit_being_reconstructed;
	std::queue<sc_lv<FLIT_SIZE> > buffer;

	void prc_decode_address();
    traffic_monitor *traffic;
public:
    void set_traffic_monitor(traffic_monitor *t){
    	traffic = t;
    }

};

#endif
