#include "link_master_requester.h"

sc_lv<CMD_WIDTH> link_master_requester::decode_cmd(){
    /*WRITE REQUESTS
    */
    if(cmd_in.read() == WR16) {writing_bytes = 16;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == WR32) {writing_bytes = 32;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 3;}
    if(cmd_in.read() == WR48) {writing_bytes = 48;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 4;}
    if(cmd_in.read() == WR64) {writing_bytes = 64;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 5;}
    if(cmd_in.read() == WR80) {writing_bytes = 80;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 6;}
    if(cmd_in.read() == WR96) {writing_bytes = 96;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 7;}
    if(cmd_in.read() == WR112){writing_bytes = 112; reading_bytes = 0; return (sc_lv<PACKET_LNG>) 8;}
    if(cmd_in.read() == WR128){writing_bytes = 128; reading_bytes = 0; return (sc_lv<PACKET_LNG>) 9;}
    if(cmd_in.read() == WR256){writing_bytes = 256; reading_bytes = 0; return (sc_lv<PACKET_LNG>) 17;}
    if(cmd_in.read() == MD_WR){writing_bytes = 0;   reading_bytes = 0; return (sc_lv<PACKET_LNG>) 2;}

    /*WRITE POSTED REQUESTS
    */
    if(cmd_in.read() == P_WR16) {writing_bytes = 16;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == P_WR32) {writing_bytes = 32;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 3;}
    if(cmd_in.read() == P_WR48) {writing_bytes = 48;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 4;}
    if(cmd_in.read() == P_WR64) {writing_bytes = 64;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 5;}
    if(cmd_in.read() == P_WR80) {writing_bytes = 80;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 6;}
    if(cmd_in.read() == P_WR96) {writing_bytes = 96;  reading_bytes = 0; return (sc_lv<PACKET_LNG>) 7;}
    if(cmd_in.read() == P_WR112){writing_bytes = 112; reading_bytes = 0; return (sc_lv<PACKET_LNG>) 8;}
    if(cmd_in.read() == P_WR128){writing_bytes = 128; reading_bytes = 0; return (sc_lv<PACKET_LNG>) 9;}
    if(cmd_in.read() == P_WR256){writing_bytes = 256; reading_bytes = 0; return (sc_lv<PACKET_LNG>) 17;}

    /*READ REQUESTS
    */
    if(cmd_in.read() == RD16) {writing_bytes = 0; reading_bytes = 16;  return (sc_lv<PACKET_LNG>) 1;}
    if(cmd_in.read() == RD32) {writing_bytes = 0; reading_bytes = 32;  return (sc_lv<PACKET_LNG>) 1;}
    if(cmd_in.read() == RD48) {writing_bytes = 0; reading_bytes = 48;  return (sc_lv<PACKET_LNG>) 1;}
    if(cmd_in.read() == RD64) {writing_bytes = 0; reading_bytes = 64;  return (sc_lv<PACKET_LNG>) 1;}
    if(cmd_in.read() == RD80) {writing_bytes = 0; reading_bytes = 80;  return (sc_lv<PACKET_LNG>) 1;}
    if(cmd_in.read() == RD96) {writing_bytes = 0; reading_bytes = 96;  return (sc_lv<PACKET_LNG>) 1;}
    if(cmd_in.read() == RD112){writing_bytes = 0; reading_bytes = 112; return (sc_lv<PACKET_LNG>) 1;}
    if(cmd_in.read() == RD128){writing_bytes = 0; reading_bytes = 128; return (sc_lv<PACKET_LNG>) 1;}
    if(cmd_in.read() == RD256){writing_bytes = 0; reading_bytes = 256; return (sc_lv<PACKET_LNG>) 1;}
    if(cmd_in.read() == MD_RD){writing_bytes = 0; reading_bytes = 256; return (sc_lv<PACKET_LNG>) 1;}

    /*ATOMIC REQUESTS
    */
    if(cmd_in.read() == ADD8)    {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == ADD16)   {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == P_2ADD8) {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == P_ADD16) {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == ADDS8R)  {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == ADDS16R) {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == INC8)    {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 1;}
    if(cmd_in.read() == P_INC8)  {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 1;}

    /*BOOLEAN REQUESTS
    */
    if(cmd_in.read() == XOR16)  {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == OR16)   {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == NOR16)  {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == AND16)  {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == NAND16) {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}


    /*COMPARASION
    */
    if(cmd_in.read() == CASGT8)    {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == CASGT16)   {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == CASLT8)    {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == CASLT16)   {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == CASEQ8)    {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == CASZERO16) {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == EQ8)       {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == EQ16) 	   {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}

    /*BITWISE REQUESTS
    */
    if(cmd_in.read() == BWR)    {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == P_BWR)  {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == BWR8R)  {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}
    if(cmd_in.read() == SWAP16) {writing_bytes = 16; reading_bytes = 16; return (sc_lv<PACKET_LNG>) 2;}

    /*
     * PIM REQUESTS
     *
     */
    if(cmd_in.read() == PIM_INST) {writing_bytes = 0; reading_bytes = 0; return (sc_lv<PACKET_LNG>) 2;}
    return 0;
}

sc_lv<MUX_SELECT_WIDTH> link_master_requester::link_to_send(){
	sc_lv<MUX_SELECT_WIDTH> link_temp;
	sc_lv<5> vault_temp;
	switch(MAX_BLOCK_SIZE){
	case 32:
		link_temp = address_in.read().range(9,8);
		vault_temp = address_in.read().range(9,5);
		break;
	case 64:
		link_temp = address_in.read().range(10,9);
		vault_temp = address_in.read().range(10,6);
		break;
	case 128:
		link_temp = address_in.read().range(11,10);
		vault_temp = address_in.read().range(11,7);
		break;
	case 256:
		link_temp = address_in.read().range(12,11);
		vault_temp = address_in.read().range(12,8);
		break;
	}

	vault_to_send = vault_temp.to_uint();
	return link_temp;
}

sc_lv<TAG_SIZE> link_master_requester::generate_tag(){
	unsigned link = link_to_send().to_uint();
	sc_lv<TAG_SIZE> temp;
	temp = tag;
	tag_buffer.push(temp);
	//if(!running) running = true;
	first_flit_already_received = true;
	tag_buffer_per_link[link].push(temp);
	tag = (sc_lv<TAG_SIZE>) (tag.to_uint() + 1);

	return temp;
}

sc_lv<HEADER_WIDTH> link_master_requester::create_header(){
	/*request header format
	 *63   61  |60     58|57                       24|23|22      21|11      7|6        0|
	 *CUB[2:0]   RES[2:0]           ADRS[33:0]        R  TAG[10:0]  LNG[4:0]    CMD[6:0]
	 */

    sc_lv<HEADER_WIDTH> header;
    header.range(6,0) = cmd_in.read();
    header.range(11,7) = decode_cmd();
    if(header.range(11,7).to_uint()!=0)
    	header.range(22,12) = generate_tag();
    header[23] = 0; // used for PIM instruction
    header.range(57,24) = address_in.read();
    header.range(60,58) = "111"; //reserved
    header.range(63,61) = cub_in.read();

	traffic -> l_monitor[link_to_send().to_uint()].read_bytes += reading_bytes;
	traffic -> l_monitor[link_to_send().to_uint()].write_bytes += writing_bytes;
	if(traffic -> l_monitor[link_to_send().to_uint()].t_tx_initial == 0)
		traffic -> l_monitor[link_to_send().to_uint()].t_tx_initial = sc_time_stamp().to_double()/1000;

	traffic -> v_monitor[vault_to_send].read_bytes += reading_bytes;
	traffic -> v_monitor[vault_to_send].write_bytes += writing_bytes;

    return header;
}

sc_lv<TAIL_WIDTH> link_master_requester::create_tail(){
	/*request tail format
	 *63                        32|31       29|28       26|25        22|21|20    18|17        9|8        0|
	 *              CRC[31:0]        RTC[2:0]   SLID[2:0]      RES[3:0] Pb SEQ[2:0]  FRP[8:0]     RRP[8:0]
	 */

    sc_uint<64> tail;
    tail.range(8,0) = "000000000";
    tail.range(17,9) = "000000000";
    tail.range(20,18) = "000";
    tail[21] = "0";
    tail.range(25,22) = "1111";
    tail.range(27,26) = link_to_send(); // link to send is the same to return
    tail.range(31,28) = "0000";

    return tail;
}

void link_master_requester::prc_flit_generator(){

	if(traffic -> t_hmc_initial == 0)
		traffic -> t_hmc_initial = sc_time_stamp().to_double()/1000;

	sc_lv<HEADER_WIDTH> header = create_header();
	sc_lv<TAIL_WIDTH> tail = create_tail();

	unsigned num_flits_to_send = header.range(11,7).to_uint();
	unsigned link_dst = link_to_send().to_uint();
	sc_lv<FLIT_SIZE> flit;
	for(unsigned i = 0; i < num_flits_to_send; i++){
		if(i == 0){
			flit.range(63,0) = header;
			if(num_flits_to_send == 1)
				flit.range(127,64) = tail;
			else
				flit.range(127,64) = data_in[i].read().range(63,0);
		}
		else if(i ==  num_flits_to_send - 1){
			flit.range(63,0) = data_in[i-1].read().range(127,64);
			flit.range(127,64) = tail;
		}
		else{
			flit.range(63,0) = data_in[i-1].read().range(127,64);
			flit.range(127,64) = data_in[i].read().range(63,0);
		}
		buffer[link_dst].push(flit);
	}
}

void link_master_requester::prc_send_flit_to_physical_tx(){
	for(int i = 0; i < NUMBER_OF_LINKS; i++){
		if(!buffer[i].empty()){
			flit_out[i].write(buffer[i].front());
			buffer[i].pop();
			flit_available[i].write(true);
		}
		else{
			flit_available[i].write(false);
		}
	}
}

void link_master_requester::prc_check_tag_ordering(){
	for(int i = 0; i < NUMBER_OF_LINKS; i++){
		if(!tag_buffer_per_link[i].empty()){
			request_tag_per_link[i].write(tag_buffer_per_link[i].front());
			if(en_pop_tag_buffer_per_link[i].read()){
				tag_buffer_per_link[i].pop();
			}
		}

	}

	if(!tag_buffer.empty()){
		request_global_tag.write(tag_buffer.front());
		if(en_pop_tag_buffer.read()){
			tag_buffer.pop();
		}
	}
}

void link_master_requester::prc_check_stop_simulation(){
	if(first_flit_already_received && tag_buffer.empty()){
		traffic -> hmc_still_busy = false;
		response_queue_status = true;
	}
	else{
		traffic -> hmc_still_busy = true;
		response_queue_status = false;
	}
}
