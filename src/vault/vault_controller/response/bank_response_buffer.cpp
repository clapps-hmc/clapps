#include "../response/bank_response_buffer.h"

void bank_response_buffer::prc_bank_response_buffer_write(){
	if(en_push.read()){//write
		tag.push(tag_in.read());
		link.push(link_in.read());
		cub.push(cub_in.read());
		response_lng.push(response_lng_in.read());
		cmd.push(cmd_in.read());
		response_dst.push(response_dst_in.read());
		buffer_size_counter++;
	}
}

void bank_response_buffer::prc_bank_response_buffer_read(){
	if(buffer_size_counter!=0){//read
		tag_out.write(tag.front());
		link_out.write(link.front());
		cub_out.write(cub.front());
		response_lng_out.write(response_lng.front());
		cmd_out.write(cmd.front());
		response_dst_out.write(response_dst.front());
		if(en_pop.read()){
			tag.pop();
			link.pop();
			cub.pop();
			response_lng.pop();
			cmd.pop();
			response_dst.pop();
			buffer_size_counter--;
		}
	}
}
