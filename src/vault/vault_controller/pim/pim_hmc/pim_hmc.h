/*
 * pim.h
 *
 *  Created on: 28 de fev de 2017
 *      Author: root
 */

#include "systemc.h"
using namespace std;


SC_MODULE(pim_hmc){
public:
	sc_in<sc_lv<256> > data_in;
	sc_in<bool> operate;
	sc_in<sc_lv<256> > immediate_in;
	sc_in<sc_lv<4> > data_select;
	sc_in<sc_lv<5> > alu_operation;

	sc_out<sc_lv<256> > data_out;

	SC_CTOR(pim_hmc){
		SC_METHOD(prc_pim);
		sensitive << operate;
	}

private:
	void prc_pim();
	void prc_alu(sc_lv<128> op0, sc_lv<128> op1, sc_lv<128> op2, sc_lv<128> op3,
				 sc_lv<4> control, sc_lv<128> &out0, sc_lv<128> &out1);
};
