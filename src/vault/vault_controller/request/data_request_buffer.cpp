/*
 * data_request_buffer.cpp
 *
 *  Created on: 9 de nov de 2016
 *      Author: root
 */

#include "data_request_buffer.h"

void data_request_buffer::prc_reset_data(){
	for(int i = 0; i < 256; i++)
		reconstructed_data[i] = "0";
}
void data_request_buffer::prc_data_request_buffer_write(){

	if(en_push.read() == "00"){ //data - header
		counter = 0;
		prc_reset_data();
		reconstructed_data.range(63,0) = data_in.read().range(63,0);
		counter++;
	}
	else if(en_push.read() == "01"){
		if(counter + 2 < 4){
			reconstructed_data.range((counter+2)*64 - 1, counter*64) = data_in.read();
			counter+=2;
		}
		else{
			reconstructed_data.range(255,192) = data_in.read().range(63,0);
			buffer.push(reconstructed_data);
			prc_reset_data();
			reconstructed_data.range(63,0) = data_in.read().range(127,64);
			counter = 1;
		}
	}
	else if(en_push.read() == "10"){
		reconstructed_data.range((counter+1)*64 - 1, counter*64) = data_in.read().range(63,0);
		counter = 0;
		buffer.push(reconstructed_data);
	}

}

void data_request_buffer::prc_data_request_buffer_read(){
	if(!buffer.empty()){
			data_out.write(buffer.front());

			if(en_pop.read()){
				buffer.pop();
			}
		}
}

