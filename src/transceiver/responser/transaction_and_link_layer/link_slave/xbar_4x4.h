#ifndef XBAR_H
#define XBAR_H

#include "../../../../common/mux_4x1.h"
#include "../../../../common/defines.h"
#include "systemc.h"

SC_MODULE(xbar_4x4){
public:
	sc_in<sc_lv<FLIT_SIZE> > flit_from_physical_rx[NUMBER_OF_LINKS];
	sc_in<sc_lv<MUX_SELECT_WIDTH> > select[NUMBER_OF_LINKS];
	sc_out<sc_lv<FLIT_SIZE> > flit_to_quadrant[NUMBER_OF_LINKS];

	SC_CTOR(xbar_4x4){
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_mux_4x1[i] = new mux_4x1(sc_gen_unique_name("mod_mux_4x1"));
			for(int j = 0; j < NUMBER_OF_LINKS; j++){
				mod_mux_4x1[i] -> data_in[j](flit_from_physical_rx[j]);
			}
			mod_mux_4x1[i]->select(select[i]);
			mod_mux_4x1[i]->data_out(flit_to_quadrant[i]);
		}
	}
private:
	mux_4x1 *mod_mux_4x1[NUMBER_OF_LINKS];
};

#endif
