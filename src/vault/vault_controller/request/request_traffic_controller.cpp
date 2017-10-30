/*
 * request_traffic_controller.cpp
 *
 *  Created on: 13 de jun de 2017
 *      Author: root
 */

#include "request_traffic_controller.h"


void request_traffic_controller::prc_read_host_requests(){
	if(request_from_host_available.read()){
		host_request_buffer.push(request_from_host.read());
	}
}

void request_traffic_controller::prc_read_pim_requests(){
	if(request_from_pim_available.read()){
		pim_request_buffer.push(request_from_pim.read());
	}
}

void request_traffic_controller::prc_traffic_controller_fsm(){
	switch (current) {
		case IDLE:
			request_available_to_vault.write(false);
			pck_counter = 0;

			if(!host_request_buffer.empty() && pim_request_buffer.empty())
				current = SEND_HOST;
			else if(host_request_buffer.empty() && !pim_request_buffer.empty())
				current = SEND_PIM;
			else if(!host_request_buffer.empty() && !pim_request_buffer.empty()){
				if(priority == 0){ current = SEND_HOST; priority = 1;}
				else if(priority == 1) {current = SEND_PIM; priority = 0;}
			}
			else
				current = IDLE;
			break;
		case SEND_HOST:
			if(pck_counter == 0){
				pck_counter = host_request_buffer.front().range(11,7).to_uint(); //get number of flits from header
			}

			if(!host_request_buffer.empty()){
				request_to_vault.write(host_request_buffer.front());
				request_available_to_vault.write(true);
				host_request_buffer.pop();
				pck_counter--;
			}
			else{
				request_available_to_vault.write(false);
			}

			if(pck_counter == 0) current = IDLE;
			else 				 current = SEND_HOST;

			break;
		case SEND_PIM:
			if(pck_counter == 0){
				pck_counter = pim_request_buffer.front().range(11,7).to_uint(); //get number of flits from header
			}

			if(!pim_request_buffer.empty()){
				//cout << "sending to pim \n";
				//cout << pim_request_buffer.front().range(63,0) << endl << endl;

				request_to_vault.write(pim_request_buffer.front());
				request_available_to_vault.write(true);
				pim_request_buffer.pop();
				pck_counter--;
			}
			else{
				request_available_to_vault.write(false);
			}

			if(pck_counter == 0) current = IDLE;
			else current = SEND_PIM;

			break;
	}
}


