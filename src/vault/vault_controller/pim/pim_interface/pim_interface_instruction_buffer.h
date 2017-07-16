/*
 * pim_interface_instruction_buffe.h
 *
 *  Created on: 26 de jun de 2017
 *      Author: root
 */

#ifndef SRC_VAULT_VAULT_CONTROLLER_PIM_PIM_INTERFACE_PIM_INTERFACE_INSTRUCTION_BUFFER_H_
#define SRC_VAULT_VAULT_CONTROLLER_PIM_PIM_INTERFACE_PIM_INTERFACE_INSTRUCTION_BUFFER_H_


#include "systemc.h"
#include <queue>

SC_MODULE(pim_instruction_buffer){
public:
	sc_in_clk clk_vault,clk_pim;
	sc_in<bool> en_push, en_pop;
	sc_in<sc_lv<128> > flit_in;

	sc_out<sc_lv<128> > instruction_out;
	sc_out<bool> instruction_available;

	SC_CTOR(pim_instruction_buffer){
		SC_METHOD(prc_pim_data_buffer_read);
		sensitive << clk_pim.pos();
		SC_METHOD(prc_pim_data_buffer_write);
		sensitive << clk_vault.pos();

		counter = 0;
	}


private:
	std::queue<sc_lv<128> > instruction_queue; // each PIM instruction is 1 FLITS long
	sc_lv<128> instruction_tmp;
	unsigned counter;

	void prc_pim_data_buffer_read(){
		if(!instruction_queue.empty()){

			instruction_out.write(instruction_queue.front());
			instruction_available.write(true);
			if(en_pop.read()){
				instruction_queue.pop();
			}
		}
		else{
			instruction_available.write(false);
		}
	}

	void prc_pim_data_buffer_write(){
		if(en_push.read()){
			if(counter == 0){
				instruction_tmp.range(63,0) = flit_in.read().range(127,64);
				counter++;
			}
			else if(counter == 1){

				instruction_tmp.range(127,64) = flit_in.read().range(63,0);
				counter = 0;
				instruction_queue.push(instruction_tmp);
			}
		}
	}
};



#endif /* SRC_VAULT_VAULT_CONTROLLER_PIM_PIM_INTERFACE_PIM_INTERFACE_INSTRUCTION_BUFFER_H_ */
