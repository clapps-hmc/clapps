#include "request_decoder.h"

void request_decoder::prc_decode_cmd(sc_lv<7> &c){
	/*
	 * Operation reflects request cmd (read(0), write(1), atomic(2))
	 * Operation_size reflects request size (from 16B(0) to 256B (9), incrementing
	 * 16 bytes each time, and MODE requests(0).
	 */

	/*WRITE REQUESTS
	*/
	hmc_instruction = false;
	pim_instruction.write(false);
	is_pim_instruction = false;
	if	   (c == WR16) {operation.write("01"); operation_size.write("0000"); response_lng.write("00001");}
	else if(c == WR32) {operation.write("01"); operation_size.write("0001"); response_lng.write("00001");}
	else if(c == WR48){operation.write("01"); operation_size.write("0010"); response_lng.write("00001");}
	else if(c == WR64){operation.write("01"); operation_size.write("0011"); response_lng.write("00001");}
	else if(c == WR80){operation.write("01"); operation_size.write("0100"); response_lng.write("00001");}
	else if(c == WR96){operation.write("01"); operation_size.write("0101"); response_lng.write("00001");}
	else if(c == WR112){operation.write("01"); operation_size.write("0110"); response_lng.write("00001");}
	else if(c == WR128){operation.write("01"); operation_size.write("0111"); response_lng.write("00001");}
	else if(c == WR256){operation.write("01"); operation_size.write("1111"); response_lng.write("00001");}
	else if(c == MD_WR){operation.write("01"); operation_size.write("0000"); response_lng.write("00001");}

	/*WRITE POSTED REQUESTS
	*/
	else if(c == P_WR16){operation.write("01"); operation_size.write("0000"); response_lng.write("00000");}
	else if(c == P_WR32){operation.write("01"); operation_size.write("0001"); response_lng.write("00000");}
	else if(c == P_WR48){operation.write("01"); operation_size.write("0010"); response_lng.write("00000");}
	else if(c == P_WR64){operation.write("01"); operation_size.write("0011"); response_lng.write("00000");}
	else if(c == P_WR80){operation.write("01"); operation_size.write("0100"); response_lng.write("00000");}
	else if(c == P_WR96){operation.write("01"); operation_size.write("0101"); response_lng.write("00000");}
	else if(c == P_WR112){operation.write("01"); operation_size.write("0110"); response_lng.write("00000");}
	else if(c == P_WR128){operation.write("01"); operation_size.write("0111"); response_lng.write("00000");}
	else if(c == P_WR256){operation.write("01"); operation_size.write("1111"); response_lng.write("00000");}



	/*READ REQUESTS
	*/
	else if(c == RD16){operation.write("00"); operation_size.write("0000"); response_lng.write("00010");}
	else if(c == RD32){operation.write("00"); operation_size.write("0001"); response_lng.write("00011");}
	else if(c == RD48){operation.write("00"); operation_size.write("0010"); response_lng.write("00100");}
	else if(c == RD64){operation.write("00"); operation_size.write("0011"); response_lng.write("00101");}
	else if(c == RD80){operation.write("00"); operation_size.write("0100"); response_lng.write("00110");}
	else if(c == RD96){operation.write("00"); operation_size.write("0101"); response_lng.write("00111");}
	else if(c == RD112){operation.write("00"); operation_size.write("0110"); response_lng.write("01000");}
	else if(c == RD128){operation.write("00"); operation_size.write("0111"); response_lng.write("01001");}
	else if(c == RD256){operation.write("00"); operation_size.write("1111");response_lng.write("10001");}
	else if(c == MD_RD){operation.write("00"); operation_size.write("0000"); response_lng.write("00010");}

	else if(c == ADD8){ hmc_instruction = true; operation.write("10"); operation_size.write("0001"); response_lng.write("00001"); alu_operation.write("00000");}
	else if(c == ADD16){hmc_instruction = true; operation.write("10"); operation_size.write("0001"); response_lng.write("00001"); alu_operation.write("10000");}

	else if(c == PIM_INST){is_pim_instruction = true; response_lng.write("00001"); pim_instruction.write(true);};
}
void request_decoder::prc_decode_address(){
	/*
	 * Address depend upon the maximum block size
	 * The 34 bits address is composed by:
	 * Ignored bytes - The first DRAM address - Vault - Bank - The rest of the DRAM address
	 * In this module we remove the ignored bytes and both vault and bank address
	 */
	sc_lv<20> address_temp;
	sc_lv<4> bank_temp;
	sc_lv<4> r_temp = request.read().range(31,28);
	if(MAX_BLOCK_SIZE == 32){
		address_temp[0] = r_temp[28];
		address_temp.range(19,1) = request.read().range(56,38);
		bank_temp = request.read().range(37,34);
	}
	else if(MAX_BLOCK_SIZE == 64){
		address_temp.range(1,0) = request.read().range(29,28);
		address_temp.range(19,2) = request.read().range(56,39);
		bank_temp = request.read().range(38,35);
	}
	else if(MAX_BLOCK_SIZE == 128){
		address_temp.range(2,0) = request.read().range(30,28);
		address_temp.range(19,3) = request.read().range(56,40);
		bank_temp = request.read().range(39,36);
	}
	else if(MAX_BLOCK_SIZE == 256){
		address_temp.range(3,0) = request.read().range(31,28);
		address_temp.range(19,4) = request.read().range(56,41);
		bank_temp = request.read().range(40,37);
	}

	address_offset.write(r_temp);
	current_bank_id = bank_temp.to_uint();
	address.write(address_temp);
	bank_id.write(bank_temp);

}
void request_decoder::prc_header_decoder(){
	/*
	 * Request header format
	 *63   61  |60     58|57                       24|23|22      21|11      7|6        0|
	 *CUB[2:0]   RES[2:0]           ADRS[33:0]        R  TAG[10:0]  LNG[4:0]    CMD[6:0]
	 *
	 *The reserved bit 23 was modified to inform weather the instruction came from the host (0) or for the pim (1)
	 */

	sc_lv<7> cmd_temp = request.read().range(6,0);
	prc_decode_cmd(cmd_temp);

	number_of_packets_to_send = request.read().range(11,7).to_uint();

	cmd.write(cmd_temp);
	tag.write(request.read().range(22,12));
	sc_lv<2> temp = request.read().range(23,22);

	if(temp[1] == 0)
		response_dst.write(false);
	else{
		response_dst.write(true);
	}


	prc_decode_address();
	cub_id.write(request.read().range(63,61));

	cmd_bck = request.read().range(6,0);
	tag_bck = request.read().range(22,12);

	if(traffic -> v_monitor[id].t_initial == 0)
		traffic -> v_monitor[id].t_initial = sc_time_stamp().to_double()/1000;


}
void request_decoder::prc_tail_decoder(){
	/*request tail format
	 *63                        32|31       29|28       26|25        22|21|20    18|17        9|8        0|
	 *              CRC[31:0]        RTC[2:0]   SLID[2:0]      RES[3:0] Pb SEQ[2:0]  FRP[8:0]     RRP[8:0]
	 */
	link.write(request.read().range(92,90));


	for(int i = 0; i < FAW; i++){
		if(i == bank_fms_to_send_request){
			en_bank_operation_buffer[i].write(true);
			en_bank_response_buffer[i].write(true);
		}
		else{
			en_bank_operation_buffer[i].write(false);
			en_bank_response_buffer[i].write(false);
		}
	}

	counter = 0;
	number_of_packets_to_send = 0;
}
void request_decoder::prc_bank_policy(){
	/*
	 * Bank Policy
	 * Is there any buffer already dealing the requested packet?
	 * YES - Send this request to that bank
	 * NO - Send this request to the next bank in a round-robin style
	 */


	/*
	 * Decide if there is an actived bank dealing with
	 * the same bank that the new one.
	 */

	if(hmc_instruction){
		bank_fms_to_send_request = 0;
	}
	else{
		for(int i = 0; i < FAW; i++){
			if(actived_bank_fsm[i] == current_bank_id){
				bank_fms_to_send_request = i;
				return;
			}
		}

	actived_bank_fsm[round_robin] = current_bank_id;
	bank_fms_to_send_request = round_robin;
	round_robin = (round_robin + 1) % FAW;

	}
}

void request_decoder::prc_disable_bank(){
	for(int i = 0; i < FAW; i++){
		en_bank_data_buffer[i].write("11");
		en_bank_operation_buffer[i].write(false);
		en_bank_response_buffer[i].write(false);
	}

	en_response_buffer.write(false);
	en_pim_instruction_buffer.write(false);
}

void request_decoder::prc_data_decoder(int size){
	sc_lv<128> data_tmp;
	if(size == 0){ // data - header
		data_tmp.range(63,0) = request.read().range(127,64);
		en_bank_data_buffer[bank_fms_to_send_request].write("00");

	}
	else if(size == 1){ // data - data
		data_tmp.range(63,0) = request.read().range(63,0);
		data_tmp.range(127,64) = request.read().range(127,64);
		en_bank_data_buffer[bank_fms_to_send_request].write("01");
	}
	else if(size == 2){// tail - data
		data_tmp.range(63,0) = request.read().range(63,0);
		en_bank_data_buffer[bank_fms_to_send_request].write("10");
	}

	data_from_packet.write(data_tmp);

}
void request_decoder::prc_decode(){
	/*
	 * Fist check if decoding the first packet
	 * to get information about its request size (number of packets).
	 * IF only 1 packet -> decode header and tail and enable data and operation bank
	 * IF number of packets == counter - 1 -> decode data and tail and
	 * Else 							   -> decode data and data
	 */

	if(start_decoding.read()){
		en_pim_instruction_buffer.write(false);
		if(counter == 0){ // decoding first packet
			prc_header_decoder();
			prc_bank_policy();
			en_response_buffer.write(false);


			if(number_of_packets_to_send == 1){
				prc_tail_decoder();
				en_response_buffer.write(true);
			}
			else if(!is_pim_instruction){
				prc_data_decoder(0);
				counter++;
			}
			else{ //pim_instruction
				packet_to_pim_interface.write(request.read());
				en_pim_instruction_buffer.write(true);
				counter++;
			}
		}
		else if(number_of_packets_to_send == counter + 1){ // sending last packet
			if(!is_pim_instruction){
				en_response_buffer.write(true);
				prc_data_decoder(2); //decode data and tail
				prc_tail_decoder();
			}
			else{
				en_response_buffer.write(true);
				link.write(request.read().range(92,90));

				packet_to_pim_interface.write(request.read());
				en_pim_instruction_buffer.write(true);
				counter = 0;
				number_of_packets_to_send = 0;
				is_pim_instruction = false;
			}
		}
		else if((counter!=0) && (counter < number_of_packets_to_send )){
			en_response_buffer.write(false);
			prc_data_decoder(1);
			counter++;
		}
	}
	else{
		en_response_buffer.write(false);
		en_pim_instruction_buffer.write(false);
		prc_disable_bank();
	}
}


void request_decoder::prc_decode_pim_instruction(sc_lv<7> &cmd){

}


