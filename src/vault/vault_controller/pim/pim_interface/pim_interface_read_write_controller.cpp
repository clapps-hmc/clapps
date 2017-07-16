/*
 * pim_interface_read_write_controller.cpp
 *
 *  Created on: 3 de jul de 2017
 *      Author: root
 */

#include "pim_interface_read_write_controller.h"

void pim_interface_read_write_controller::prc_read_request_from_pim(){
	if(en_read_request.read() && !en_write_request.read()){
		sc_lv<64> header = create_header(READ);
		sc_lv<64> tail = create_tail(READ);
		sc_lv<128> flit;
		flit.range(63,0) = header;
		flit.range(127,64) = tail;
		buffer_rqst.push(flit);
	}
	else if(!en_read_request.read() && en_write_request.read()){
		sc_lv<64> header = create_header(WRITE);
		sc_lv<64> tail = create_tail(WRITE);
		sc_lv<128> flit;
		unsigned position = 0;
		for(unsigned i = 0; i < num_flits; i++){
			if(i==0){
				flit.range(63,0) = header;
				flit.range(127,64) = data_in.read().range((position+1)*64 - 1,position);
				position++;
			}
			else if(i == num_flits - 1){
				flit.range(63,0) = data_in.read().range((position+1)*64 -1,position*64);
				flit.range(127,64) = tail;
				position = 0;
			}
			else{
				flit = data_in.read().range((position+2)*64-1,position*64);
				position+=2;
			}

			buffer_rqst.push(flit);
		}
	}
}

void pim_interface_read_write_controller::prc_write_request_to_mem(){
	if(!buffer_rqst.empty()){
		request_out.write(buffer_rqst.front());
		request_available.write(true);

		buffer_rqst.pop();
	}
	else
		request_available.write(false);
}

void pim_interface_read_write_controller::prc_read_response_from_mem(){
	switch (current) {
		case IDLE: //DECODING HEADER
			counter = 0;
			num_packets = 0;
			en_pop_mem_response_buffer.write(false);

			if(en_response.read()){
				num_packets = response_in.read().range(11,7).to_uint();
				if(rsp_is_read())
					current = SENDING;
				else{
					en_pop_mem_response_buffer.write(true);
					buffer_rsp_write.push(response_in);
					current = DONE;
				}
			}
			else{
				current = IDLE;
			}
			break;
		case SENDING:
			if(counter == 0){
				request_tmp.range(63,0) = response_in.read().range(127,64);
				counter++;
				en_pop_mem_response_buffer.write(true);
				current = WAIT;
			}
			else if(counter == num_packets - 1){
				request_tmp.range(127,64) = response_in.read().range(63,0);
				buffer_rsp_read.push(request_tmp);
				en_pop_mem_response_buffer.write(true);
				counter = 0;
				current = DONE;
			}
			else{
				request_tmp.range(127,64) = response_in.read().range(63,0);
				buffer_rsp_read.push(request_tmp);
				request_tmp.range(63,0) = response_in.read().range(127,64);
				en_pop_mem_response_buffer.write(true);
				counter++;
				current = WAIT;

			}
			break;
		case WAIT:
			en_pop_mem_response_buffer.write(false);
			if(en_response.read())
				current = SENDING;
			else
				current = WAIT;
			break;
		case DONE:
			en_pop_mem_response_buffer.write(false);
			current = IDLE;
			break;
		}
}

void pim_interface_read_write_controller::prc_write_write_response_to_pim(){
	if(!buffer_rsp_write.empty()){
		write_response_available.write(true);

		if(en_write_response.read())
			buffer_rsp_write.pop();
	}
	else
		write_response_available.write(false);
}

void pim_interface_read_write_controller::prc_write_read_response_to_pim(){
	if(!buffer_rsp_read.empty()){
		data_out.write(buffer_rsp_read.front());
		read_response_available.write(true);

		if(en_read_response.read())
			buffer_rsp_read.pop();
	}
	else
		read_response_available.write(false);
}




sc_lv<64> pim_interface_read_write_controller::create_header(operation op){
	sc_lv<64> header;
	if(op == READ){
		header.range(6,0) = find_command(op);
		header.range(11,7) = "00001";
		header.range(22,12) = generate_tag();
		header[23] = "1"; // reserved
		header.range(57,24) = address.read();
		header.range(60,58) = "111"; //reserved
		header.range(63,61) = "111";
	}
	else if(op == WRITE){
		  header.range(6,0) = find_command(op);
		  header.range(11,7) = (sc_lv<PACKET_LNG>) num_flits;
		  header.range(22,12) = generate_tag();
		  header[23] = "1"; // reserved
		  header.range(57,24) = address.read();
		  header.range(60,58) = "111"; //reserved
		  header.range(63,61) = "111";
	}

	return header;
}

sc_lv<7> pim_interface_read_write_controller::find_command(operation op){
	sc_lv<7> cmd;
	if(op == READ){
		if(size.read() == B16)  cmd = RD16;
		if(size.read() == B32)  cmd = RD32;
		if(size.read() == B48)  cmd = RD48;
		if(size.read() == B64)  cmd = RD64;
		if(size.read() == B80)  cmd = RD80;
		if(size.read() == B96)  cmd = RD96;
		if(size.read() == B112) cmd = RD112;
		if(size.read() == B128) cmd = RD128;
		if(size.read() == B256) cmd = RD256;

	}
	else if (op == WRITE){
		if(size.read() == B16){ cmd = WR16;  num_flits = 2; }
		if(size.read() == B32){ cmd = WR32;  num_flits = 3;}
		if(size.read() == B48){ cmd = WR48;  num_flits = 4;}
		if(size.read() == B64){ cmd = WR64;  num_flits = 5;}
		if(size.read() == B80){ cmd = WR80;  num_flits = 6;}
		if(size.read() == B96){ cmd = WR96;  num_flits = 7;}
		if(size.read() == B112){ cmd = WR112; num_flits = 8;}
		if(size.read() == B128){ cmd = WR128; num_flits = 9;}
		if(size.read() == B256){ cmd = WR256;  num_flits = 17;}
	}
	return cmd;
}

sc_lv<11> pim_interface_read_write_controller::generate_tag(){
	sc_lv<11> tag = (sc_lv<11>) tag_counter;
	tag_counter--;
	if(tag_counter == 0 ) tag_counter = 2047;
	return tag_counter;
}

sc_lv<TAIL_WIDTH> pim_interface_read_write_controller::create_tail(operation op){
	/*request tail format
	 *63                        32|31       29|28       26|25        22|21|20    18|17        9|8        0|
	 *              CRC[31:0]        RTC[2:0]   SLID[2:0]      RES[3:0] Pb SEQ[2:0]  FRP[8:0]     RRP[8:0]
	 */


    sc_lv<64> tail;
    tail.range(8,0) = "000000000";
    tail.range(17,9) = "000000000";
    tail.range(20,18) = "000";
    tail[21] = "0";
    tail.range(25,22) = "1111";
    tail.range(27,26) = "000"; // link to send is the same to return
    tail.range(31,28) = "0000";
    tail.range(63,32) = "00000000000000000000000000000000";
    return tail;
}

bool pim_interface_read_write_controller::rsp_is_read(){
	sc_lv<7> cmd = response_in.read().range(6,0);

	if(cmd == RD16)  return true;
	if(cmd == RD32)  return true;
	if(cmd == RD48)  return true;
	if(cmd == RD64)  return true;
	if(cmd == RD80)  return true;
	if(cmd == RD96)  return true;
    if(cmd == RD112) return true;
	if(cmd == RD128) return true;
	if(cmd == RD256) return true;
	if(cmd == MD_RD) return true;

	return false;
}


