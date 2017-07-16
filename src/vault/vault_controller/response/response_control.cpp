#include "../response/response_control.h"

void response_control::prc_response_control_write(){
	switch(current_state){
	case IDLE:
		for(int i = 0; i < FAW; i++){
			en_pop_bank_response_buffer[i].write(false);
			rsp_will_send[i].write(false);
			en_pop_data_buffer[i].write(false);
		}
		en_pop_response_buffer.write(false);
		faw_source = 0;
		counter = 0;
		data_counter = 0;
		reg_is_pim_instruction = false;

		if(is_pim_instruction.read()){
			num_of_pck = 1;
			pck.range(127,64) = creat_tail(-1);
			en_pop_response_buffer.write(true);			reg_is_pim_instruction = true;
			reg_tag_from_rsp_buffer =  tag_from_rsp_buffer.read();
			reg_link_from_pim_rqst =  link_from_pim_rqst.read();
			current_state = SENDING;
		}
		else{
			for(int i = 0; i < FAW; i++){
				if((tag_from_bank_rsp_buffer[i].read() == tag_from_rsp_buffer.read()) && (bank_status[i].read())){
					faw_source = i;
					num_of_pck = response_lng[faw_source].read().to_uint();
					rsp_will_send[faw_source].write(true);
					current_state = SENDING;
					break;
				}
			}
		}

		break;
	case SENDING:
		if(reg_is_pim_instruction){
			pck.range(63,0)   = creat_header(-1);
			en_pop_response_buffer.write(false);
			buffer_r.push(pck);
			current_state = FINISHED;
		}
		else{
			rsp_will_send[faw_source].write(false);
			if(num_of_pck == 1){ // only header and tail
				pck.range(63,0)   = creat_header(faw_source);
				pck.range(127,64) = creat_tail(faw_source);
				decoded_tail = true;
				if(response_dst[faw_source].read()){
					buffer_r_pim.push(pck);
				}
				else{
					buffer_r.push(pck);
				}
				en_pop_bank_response_buffer[faw_source].write(true);
				en_pop_response_buffer.write(true);
				current_state = FINISHED;

			}
			else if(counter == 0){ //decoding header + data
				pck.range(63,0)    = creat_header(faw_source);
				pck.range(127, 64) = creat_data(faw_source);

				buffer_w.push(pck);
				counter++;
				current_state = SENDING;

			}
			else if(counter == num_of_pck - 1){ //decoding data + tail
				pck.range(63,0)    = creat_data(faw_source);
				pck.range(127,64)  = creat_tail(faw_source);
				decoded_tail = true;
				buffer_w.push(pck);


				while(!buffer_w.empty()){
					if(response_dst[faw_source].read()){
						buffer_r_pim.push(buffer_w.front());
					}else
						buffer_r.push(buffer_w.front());

					buffer_w.pop();
				}

				en_pop_bank_response_buffer[faw_source].write(true);
				en_pop_response_buffer.write(true);
				current_state = FINISHED;
			}
			else{ // decoding data + data
				pck.range(63,0)    = creat_data(faw_source);
				pck.range(127,64)  = creat_data(faw_source);
				counter++;

				buffer_w.push(pck);

				current_state = SENDING;
			}
		}
		break;
	case FINISHED:
		en_pop_response_buffer.write(false);
		en_pop_bank_response_buffer[faw_source].write(false);

		counter = 0;
		data_counter = 0;
		traffic -> v_monitor[id].t_final =  sc_time_stamp().to_double()/1000 + RAS_LATENCY + CAS_LATENCY;
		current_state = IDLE;
		break;
	}
}

void response_control::prc_response_control_read(){
	if(!buffer_r.empty()){
		packet_out.write(buffer_r.front());
		packet_available_host.write(true);

		if(en_pop.read()){
			buffer_r.pop();
		}
	}
	else{
		packet_available_host.write(false);
	}
}

void response_control::prc_response_control_read_pim(){
	int counter = 0;
	if(!buffer_r_pim.empty()){
		packet_out_pim.write(buffer_r_pim.front());
		packet_available_pim.write(true);

		if(counter == 0){
			counter++;
		}
		if(en_pop_from_pim.read()){
			buffer_r_pim.pop();
			counter = 0;
		}
	}
	else{
		packet_available_pim.write(false);
	}
}

sc_lv<64> response_control::creat_header(int bank){
	/*
	 * RESPONSE PACKET HEADER LAYOUT
	 *
	 * 63  61|60      42|41     39|38 34|33|32         23|22       12|11  7|6   0
	 *   CUB      RES       SLID    RES  AF       RES          TAG      LNG   CMD
	 *
	 */

	sc_lv<64> header;

	if(bank == -1){ //no bank at all, it is a pim instruction
		header.range(6,0) = PIM_INST;
		header.range(11,7) = "00010";
		header.range(22,12) =  reg_tag_from_rsp_buffer;
		header.range(32,23) = "1111111111";
		header[33] = "1";
		header.range(38,34) = "11111";
		header.range(41,39) = reg_link_from_pim_rqst;
		header.range(60,42) = "1111111111111111111";
		header.range(63,61) = "000";
	}
	else{
		header.range(6,0) = cmd[bank].read();
		header.range(11,7) = response_lng[bank].read();
		header.range(22,12) = tag_from_bank_rsp_buffer[bank].read();
		header.range(32,23) = "1111111111";
		header[33] = "1";
		header.range(38,34) = "11111";
		header.range(41,39) = link[bank].read();
		header.range(60,42) = "1111111111111111111";
		header.range(63,61) = cub[bank].read();
	}


	tag_bck = header.range(22,12);
	cmd_bck = header.range(6,0);

	return header;
}

sc_lv<64> response_control::creat_data(int bank){
	sc_lv<64> data_tmp_;
		switch(data_counter){
		case 0:

			data_tmp = data[bank].read();
			en_pop_data_buffer[bank].write(true);
			data_tmp_ = data_tmp.range(63,0);
			break;
		case 1:
			data_tmp_ = data_tmp.range(127,64);
			en_pop_data_buffer[bank].write(false);
			break;
		case 2:
			data_tmp_ = data_tmp.range(191,128);
			en_pop_data_buffer[bank].write(false);
			break;
		case 3:
			data_tmp_ = data_tmp.range(255,192);
			en_pop_data_buffer[bank].write(false);
			break;
		}

	data_counter = (data_counter + 1) % 4;
	return data_tmp_;
}

sc_lv<64> response_control::creat_tail(int bank){
	/*
	 * RESPONSE PACKET TAIL LAYOUT
	 *
	 * 63                     32|31   29|28      22| 21 |20    18|17    9|8     0
	 * 			CRC				   RTC	   ERRSTAT  DINV    SEQ    FRP      RRP
	 *
	 */

	sc_lv<64> tail;
	tail.range(8,0) = "000000000";
	tail.range(17,9) = "000000000";
	tail.range(21,18) = "0000";

	tail.range(28,22) = "0000000";
	tail.range(31,29) = "000";
	tail.range(63,32) = "00000000000000000000000000000000";

	return tail;

}
