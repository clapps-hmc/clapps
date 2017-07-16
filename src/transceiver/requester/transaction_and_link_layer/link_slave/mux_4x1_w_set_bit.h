#ifndef MUX_4X1_W_SET_BIT_H_
#define MUX_4X1_W_SET_BIT_H_

#include "../../../../common/defines.h"
#include "systemc.h"

SC_MODULE(mux_4x1_w_set_bit){
public:
	sc_in<sc_lv<FLIT_SIZE> > flit_in[NUMBER_OF_LINKS];
	sc_in<bool> set_bit;
	sc_in<sc_lv<MUX_SELECT_WIDTH> > select;

	sc_out<sc_lv<FLIT_SIZE> > flit_out;

	SC_CTOR(mux_4x1_w_set_bit){
		SC_METHOD(prc_mux_4x1);
		for(int i=0; i < NUMBER_OF_LINKS; i++)
			sensitive << flit_in[i];
		sensitive << select << set_bit;
		traffic = NULL;
	}

private:
	void prc_mux_4x1(){
		if(set_bit.read()){
			switch (select.read().to_uint()) {
				case 0:
					flit_out.write(flit_in[0].read());
					break;
				case 1:
					flit_out.write(flit_in[1].read());
					break;
				case 2:
					flit_out.write(flit_in[2].read());
					break;
				case 3:
					flit_out.write(flit_in[3].read());
					break;
			}

		}

		traffic -> t_hmc_final = sc_time_stamp().to_double()/1000 + RAS_LATENCY + CAS_LATENCY;
	}

    traffic_monitor *traffic;
public:
    void set_traffic_monitor(traffic_monitor *t){
    	traffic = t;
    }
};

#endif
