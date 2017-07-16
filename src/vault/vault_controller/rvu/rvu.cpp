/*
 * rvu.cpp
 *
 *  Created on: 4 de jul de 2017
 *      Author: root
 */

#include "rvu.h"

void rvu::prc_rvu(){
	switch(current){
	case FETCH:
		en_pop_instruction_buffer.write(false);
		en_read_request.write(false);
		en_pop_read_response.write(false);
		en_pop_write_response.write(false);
		counter = 0;

		traffic -> pim_still_busy[rvu_id] = false;
		if(rvu_instruction_available.read()){
			instruction = rvu_instruction.read();
			en_pop_instruction_buffer.write(true);
			current = DECODE;
			traffic -> pim_still_busy[rvu_id] = true;
		}
		else{
			current = FETCH;
		}
		break;
	case DECODE:
		en_pop_instruction_buffer.write(false);
		decode_instruction();
		if(type == READ){
			current = REQUEST_READ;
		}
		else if(type == WRITE){
			current = REQUEST_WRITE;
		}
		else if(type == ALU){
			current = EXEC;
		}
		break;
	case EXEC:
		exec();
		current = FETCH;
		break;
	case REQUEST_READ:
		address.write(addr);
		size.write(request_size);
		en_read_request.write(true);
		current = WAIT_READ;
		break;
	case REQUEST_WRITE:
		address.write(addr);
		size.write(request_size);
		en_write_request.write(true);
		write_request.write(reg_bank[rs]);

		current = WAIT_WRITE;
		break;
	case WAIT_READ:
		en_read_request.write(false);
		en_pop_read_response.write(false);
		if(read_response_available.read()){
			current = WRITE_BACK;
		}
		else
			current = WAIT_READ;
		break;
	case WAIT_WRITE:
		en_write_request.write(false);

		if(write_response_available.read()){
			en_pop_write_response.write(true);
			current = FETCH;
		}
		else
			current = WAIT_WRITE;
		break;
	case WRITE_BACK:

		reg_bank[rs].range((counter+1)*128 - 1,counter*128) = read_response.read();
		en_pop_read_response.write(true);

		counter++;
		if(counter == operation_size){
			current = FETCH;
		}
		else{
			current = WAIT_READ;
		}
		break;
	}
}


void rvu::decode_instruction(){
	/*
	 * Instruction format
	 * I-TYPE
	 * 	45	          12 11	   7  6		0
	 * 	       ADDRESS	|	RS	|	 CMD
	 * R-TYPE
	 * 28	 22     21		17    16	  12 11	   7  6		0
	 * SIZE	    |      RD      |     RT	|	RS	|	 CMD
	 *
	 * To identify instructions:
	 * 3 MSB:
	 * LOAD  100xxxx
	 * STORE 101xxxx
	 * ALU   010xxxx
	 * MULT  011xxxx
	 * DIV   001xxxx
	 *
	 */

	sc_lv<7> cmd = instruction.range(6,0);

	if(cmd == LOAD32) {type = READ; operation_size = 2;  request_size = B32;  rs = instruction.range(11,7).to_uint();  addr = instruction.range(45,12);}
	if(cmd == LOAD64) {type = READ; operation_size = 4;  request_size = B64;  rs = instruction.range(11,7).to_uint();  addr = instruction.range(45,12);}
	if(cmd == LOAD128){type = READ; operation_size = 8;  request_size = B128; rs = instruction.range(11,7).to_uint();  addr = instruction.range(45,12);}
	if(cmd == LOAD256){type = READ; operation_size = 16; request_size = B256; rs = instruction.range(11,7).to_uint();  addr = instruction.range(45,12);}

	if(cmd == STORE32) {type = WRITE; operation_size = 2;  request_size = B32;  rs = instruction.range(11,7).to_uint();  addr = instruction.range(45,12);}
	if(cmd == STORE64) {type = WRITE; operation_size = 4;  request_size = B64;  rs = instruction.range(11,7).to_uint();  addr = instruction.range(45,12);}
	if(cmd == STORE128){type = WRITE; operation_size = 8;  request_size = B128; rs = instruction.range(11,7).to_uint();  addr = instruction.range(45,12);}
	if(cmd == STORE256){type = WRITE; operation_size = 16; request_size = B256; rs = instruction.range(11,7).to_uint();  addr = instruction.range(45,12);}

	if(cmd.range(6,4) == "010"){type = ALU; operation_size = instruction.range(28,22).to_uint(); rs = instruction.range(11,7).to_uint(); rt = instruction.range(16,12).to_uint(); rd = instruction.range(21,17).to_uint(); alu_op = instruction.range(2,0); }

}

void rvu::exec(){
	unsigned func = alu_op.to_uint();
	for(unsigned i = 0; i < operation_size; i++){
		if(func == 0){ reg_bank[rd].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH) = (sc_lv<SCALAR_WIDTH>)(  reg_bank[rs].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint() + reg_bank[rt].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint());}
		if(func == 1){ reg_bank[rd].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH) = (sc_lv<SCALAR_WIDTH>)(   reg_bank[rs].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint() - reg_bank[rt].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint());}
		if(func == 2){ reg_bank[rd].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH) = (sc_lv<SCALAR_WIDTH>)(   reg_bank[rs].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint() | reg_bank[rt].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint());}
		if(func == 3){ reg_bank[rd].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH) = (sc_lv<SCALAR_WIDTH>)(   reg_bank[rs].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint() & reg_bank[rt].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint());}
		if(func == 4){ reg_bank[rd].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH) = (sc_lv<SCALAR_WIDTH>)(  reg_bank[rs].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint() ^ reg_bank[rt].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint());}
		if(func == 5){ reg_bank[rd].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH) = (sc_lv<SCALAR_WIDTH>)( !(reg_bank[rs].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint() | reg_bank[rt].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint()));}
		if(func == 6){ reg_bank[rd].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH) = (sc_lv<SCALAR_WIDTH>)( !(reg_bank[rs].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint() & reg_bank[rt].range((i+1)*SCALAR_WIDTH -1,i*SCALAR_WIDTH).to_uint()));}
	}
}
