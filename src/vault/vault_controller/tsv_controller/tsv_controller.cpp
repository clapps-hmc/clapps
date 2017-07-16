/*
 * tsv_controller.cpp
 *
 *  Created on: 7 de nov de 2016
 *      Author: root
 */

#include "tsv_controller.h"

void tsv_controller::prc_tsv_controller(){
	for(unsigned i = 0; i < FAW; i++){
		if(i == round_robin){
			if(we[i].read()){
				if(fsm_operation[i].read() == "01")
					data_to_bank[bank_id[i].read().to_uint()].write(data_from_data_buffer[i].read());
				else
					data_to_bank[bank_id[i].read().to_uint()].write(data_from_pim_data_buffer[i].read());

				write_available[bank_id[i].read().to_uint()].write(true);
				ack_request[i].write(true);
			}
			else if(oe[i].read()){
				if(data_available_to_read[bank_id[i].read().to_uint()].read()){
					data_to_data_buffer[i].write(data_from_bank[bank_id[i].read().to_uint()].read());
					if(fsm_operation[i].read() == "00")
						en_push_data_buffer[i].write(true);
					else
						en_push_pim_data_buffer[i].write(true);
					ack_request[i].write(true);
				}
			}
		}
		else{
			ack_request[i].write(false);
			en_push_data_buffer[i].write(false);
			en_push_pim_data_buffer[i].write(false);
			write_available[bank_id[i].read().to_uint()].write(false);
		}
	}

	round_robin = (round_robin + 1) % FAW;
}
