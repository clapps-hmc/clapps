/*
 * pim.cpp
 *
 *  Created on: 28 de fev de 2017
 *      Author: root
 */

#include "pim_hmc.h"

void pim_hmc::prc_pim(){
	if(operate.read()){
		sc_lv<128> data_16_bytes;
		sc_lv<128> operation_a_alu_0 = 0;
		sc_lv<128> operation_b_alu_0 = 0;
		sc_lv<128> operation_a_alu_1 = 0;
		sc_lv<128> operation_b_alu_1 = 0;
		sc_lv<128> output_alu_0 = 0;
		sc_lv<128> output_alu_1 = 0;

		sc_lv<4> data_select_tmp = data_select.read();
		if(data_select_tmp[0] == 0){ // offset = 0, data in 15:0
			data_16_bytes = data_in.read().range(127,0);
		}
		else{ 					// offset = 1, data in 31:16
			data_16_bytes = data_in.read().range(255,128);
		}

		sc_lv<5> op = alu_operation.read();
		if(op[4] == 0){ // 8 bytes operation

			operation_a_alu_0.range(63,0) = data_16_bytes.range(63,0);
			operation_b_alu_0.range(63,0) = immediate_in.read().range(63,0);
			operation_a_alu_1.range(63,0) = data_16_bytes.range(127,64);
			operation_b_alu_1.range(63,0) = immediate_in.read().range(127,64);
		}
		else{ //16 bytes operation;
			operation_a_alu_0 = data_16_bytes;
			operation_b_alu_0 = immediate_in.read().range(127,0);
		}

		prc_alu(operation_a_alu_0, operation_b_alu_0, operation_a_alu_1, operation_b_alu_1, alu_operation.read().range(3,0),
				output_alu_0, output_alu_1);

		sc_lv<128> tmp;
		if(op[4] == 0){ // 8 bytes operation
			tmp.range(63,0) = output_alu_0.range(63,0);
			tmp.range(127,64) = output_alu_1.range(63,0);
		}
		else{		//16 bytes operation;
			tmp = output_alu_0;
		}

		sc_lv<256> out;
		if(data_select_tmp[0] == 0){ // offset = 0, data in 15:0
			out.range(127,0) = tmp;
			out.range(255,128) = data_in.read().range(255,128);
		}
		else{ // offset = 1, data in 31:16
			out.range(127,0) = data_in.read().range(127,0);
			out.range(255,128) = tmp;
		}

		data_out.write(out);
	}
};

void pim_hmc::prc_alu(sc_lv<128> op0, sc_lv<128> op1, sc_lv<128> op2, sc_lv<128> op3,
	sc_lv<4> control, sc_lv<128> &out0, sc_lv<128> &out1){
	switch(control.to_uint()) {
		case 0:
			out0 = op0.to_uint() + op1.to_uint();
			out1 = op2.to_uint() + op3.to_uint();
		break;
	}
}
