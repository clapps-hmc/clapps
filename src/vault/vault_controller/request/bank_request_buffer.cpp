#include "bank_request_buffer.h"

void bank_request_buffer::prc_bank_request_buffer_write(){
	if(en_push.read()){//write

		address.push(address_in.read());
		tag.push(tag_in.read());
		operation.push(operation_in.read());
		operation_size.push(operation_size_in.read());
		bank.push(bank_in.read());
		offset.push(offset_in.read());
		alu_operation.push(alu_operation_in.read());
		buffer_size_counter++;

	}
}

void bank_request_buffer::prc_bank_request_buffer_read(){
	if(!operation.empty()){
		address_out.write(address.front());
		tag_out.write(tag.front());
		operation_out.write(operation.front());
		operation_size_out.write(operation_size.front());
		bank_out.write(bank.front());
		offset_out.write(offset.front());
		alu_operation_out.write(alu_operation.front());

		if(en_pop.read()){
			address.pop();
			tag.pop();
			operation.pop();
			operation_size.pop();
			bank.pop();
			offset.pop();
			alu_operation.pop();
		}
	}
	else{
		operation_out.write("11");
	}

}
