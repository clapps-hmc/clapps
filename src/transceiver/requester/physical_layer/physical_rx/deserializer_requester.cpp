/*
 * deserializer_rx.cpp
 *
 *  Created on: 24 de out de 2016
 *      Author: root
 */

#include "deserializer_requester.h"

void deserializer_requester::prc_deserializer(){
	if(serialized_flit_available.read()){
		flit_being_reconstructed.range((counter+1)*16 -1, counter*16) = serialized_flit.read();
		if(counter == 7){
			buffer.push(flit_being_reconstructed);
			counter = 0;

		}
		else{
			counter++;
		}
	}
}

void deserializer_requester::prc_send_deserialized_flit(){
	if(!buffer.empty()){
		flit_deserialized.write(buffer.front());
		flit_deserialized_available.write(true);

		traffic -> l_monitor[id].t_rx_final = sc_time_stamp().to_double()/1000 + RAS_LATENCY + CAS_LATENCY;
		if(en_pop_deserialize_queue.read()){
			buffer.pop();
			flit_deserialized_available.write(false);
		}
	}
	else{
		flit_deserialized_available.write(false);

	}
}
