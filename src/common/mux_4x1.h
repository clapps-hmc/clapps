#ifndef MUX_H
#define MUX_H

#include "systemc.h"
#include "defines.h"

SC_MODULE(mux_4x1){
public:
	sc_in<sc_lv<FLIT_SIZE> > data_in[NUMBER_OF_LINKS];
	sc_in<sc_lv<MUX_SELECT_WIDTH> > select;
	sc_out<sc_lv<FLIT_SIZE> > data_out;

	SC_CTOR(mux_4x1){
		SC_METHOD(prc_mux_4x1);
		for(int i=0; i < NUMBER_OF_LINKS; i++)
			sensitive << data_in[i];
		sensitive << select;
	}
private:
	void prc_mux_4x1(){
		switch (select.read().to_uint()) {
			case 0:
				data_out.write(data_in[0].read());
				break;
			case 1:
				data_out.write(data_in[1].read());
				break;
			case 2:
				data_out.write(data_in[2].read());
				break;
			case 3:
				data_out.write(data_in[3].read());
				break;
			default:
				data_out.write("X");
				break;
		}
	}
};

#endif
