/*
 * link_control.cpp
 *
 *  Created on: 24 de out de 2016
 *      Author: root
 */

#include "link_control.h"

void vault_control::prc_vault_control(){
	switch(current){
	case IDLE:
		vault_select.write("xxx");
		for(int i=0; i < VAULTS_PER_QUADRANT; i++)
			en_pop_response[i].write(false);
		en_pop_request.write(false);
		en_push_serializer.write(false);
		counter = 0;
		num_of_packets = 0;

		for(int i=0; i < VAULTS_PER_QUADRANT; i++){
			if(request_tag.read() == response_flit[i].read().range(22,12) && response_tag_status[i].read()){
				current_vault = i;
				num_of_packets = response_flit[current_vault].read().range(11,7).to_uint();
				current = SET_SIGNALS;
				break;
			}
		}
		break;
	case SET_SIGNALS:
		en_pop_response[current_vault].write(true);
		en_push_serializer.write(true);
		vault_select.write(current_vault);

		current = WAIT;
		break;
	case WAIT:
		en_pop_response[current_vault].write(false);
		en_push_serializer.write(false);
		counter++;
		if(counter == num_of_packets) current = DONE;
		else current = SET_SIGNALS;

		break;
	case DONE:
		en_push_serializer.write(false);
		en_pop_response[current_vault].write(false);
		en_pop_request.write(true);

		current = IDLE;
		break;
	}
}
