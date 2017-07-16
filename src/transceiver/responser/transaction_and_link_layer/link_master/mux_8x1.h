/*
 * mux_8x1.h
 *
 *  Created on: 24 de out de 2016
 *      Author: root
 */

#ifndef SRC_TEST_FILE_MUX_8X1_H_
#define SRC_TEST_FILE_MUX_8X1_H_

#include "systemc.h"
#include "../../../../common/defines.h"

SC_MODULE(mux_8x1){
	sc_in<sc_lv<FLIT_SIZE> > in[VAULTS_PER_QUADRANT];
	sc_in<sc_lv<DEMUX_SELECT_WIDTH> > sel;
	sc_out<sc_lv<FLIT_SIZE> > out;


	void prc_mux_8x1();
	SC_CTOR(mux_8x1){
		SC_METHOD(prc_mux_8x1);
		for(int i=0; i < VAULTS_PER_QUADRANT; i++)
			sensitive << in[i];
		sensitive << sel;
	}
};

#endif /* SRC_TEST_FILE_MUX_8X1_H_ */
