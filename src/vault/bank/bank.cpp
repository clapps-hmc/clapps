#include "bank.h"
#include <vector>

sc_lv<256> bank::decode_row_buffer(){
	sc_lv<256> decoded_r_b;
	unsigned offset;
	switch(MAX_BLOCK_SIZE){
	case 32:
		decoded_r_b = row_buffer;
		break;
	case 64:
		offset = offset_in[current_fsm].read().range(1,0).to_uint();
		if(offset != 3){
			decoded_r_b = row_buffer.range((offset+2)*128 - 1, offset*128);
		}
		else{
			decoded_r_b.range(127,0) = row_buffer.range(511,384);
			decoded_r_b.range(255,128) = row_buffer.range(127,0);
		}
		break;
	case 128:
		offset = offset_in[current_fsm].read().range(2,0).to_uint();
		if(offset != 7){
			decoded_r_b = row_buffer.range((offset+2)*128 - 1, offset*128);
		}
		else{
			decoded_r_b.range(127,0) = row_buffer.range(1023,896);
			decoded_r_b.range(255,128) = row_buffer.range(127,0);
		}
		break;
	case 256:
		offset = offset_in[current_fsm].read().range(3,0).to_uint();
		if(offset != 15){
			decoded_r_b = row_buffer.range((offset+2)*128 - 1, offset*128);
		}
		else{
			decoded_r_b.range(127,0) = row_buffer.range(2047,1920);
			decoded_r_b.range(255,128) = row_buffer.range(127,0);
		}
		break;
	}
	return decoded_r_b;

}

unsigned bank::get_offset(){
	unsigned offset;
	switch(MAX_BLOCK_SIZE){
	case 32:
		offset = 0;
		break;
	case 64:
		offset = offset_in[current_fsm].read().range(1,0).to_uint();
		break;
	case 128:
		offset = offset_in[current_fsm].read().range(2,0).to_uint();
		break;
	case 256:
		offset = offset_in[current_fsm].read().range(3,0).to_uint();
	}

	return offset;
}
void bank::write_row_buffer(){
	unsigned offset;
	switch(MAX_BLOCK_SIZE){
	case 32:
		row_buffer = data_in.read();
		break;
	case 64:
		offset = offset_in[current_fsm].read().range(1,0).to_uint();
		if(offset != 3){
			row_buffer((offset+2)*128 - 1, offset*128) = data_in.read();
		}
		else{
			row_buffer.range(511,384) = data_in.read().range(127,0);
			row_buffer.range(127,0) = data_in.read().range(255,128);
		}
		break;
	case 128:
		offset = offset_in[current_fsm].read().range(2,0).to_uint();
		if(offset != 7){
			row_buffer((offset+2)*128 - 1, offset*128) = data_in.read();
		}
		else{
			row_buffer.range(1023,896) = data_in.read().range(127,0);
			row_buffer.range(127,0) = data_in.read().range(255,128);
		}
		break;
	case 256:
		offset = offset_in[current_fsm].read().range(3,0).to_uint();

		if(offset != 15){
			row_buffer((offset+2)*128 - 1, offset*128) = data_in.read();
		}
		else{
			row_buffer.range(2047,1920) = data_in.read().range(127,0);
			row_buffer.range(127,0) = data_in.read().range(255,128);
		}
		break;
	}
}
void bank::prc_bank(){
	for(unsigned i=0; i < FAW; i++){
		if(cs[i].read() && request_bank_id[i].read().to_uint() == bank_id){
			current_fsm = i;
			address_buffer = address[current_fsm].read();
			data_available_to_read.write(false);
		}
	}
	if(current_fsm != 5){
		if(ras[current_fsm].read()){
			unsigned converted_address = get_converted_address();
			std::unordered_map<unsigned, sc_lv<8*MAX_BLOCK_SIZE> >::const_iterator got = bank_storage.find(converted_address);

			if(got != bank_storage.end())
				bank_buffer_read = bank_storage[converted_address];

			data_available_to_read.write(false);
		}
		else if(cas[current_fsm].read()){
			row_buffer = bank_buffer_read;
			data_available_to_read.write(false);
		}
		else if(oe[current_fsm].read()){
			data_out.write(decode_row_buffer());
			data_available_to_read.write(true);

		}
		else if(we[current_fsm].read()){
			if(data_available_to_write.read() && previous_offset != get_offset()){
				//cout << " @" << sc_time_stamp() << " we and datw in bank is high \n";
				write_row_buffer();
				unsigned converted_address = get_converted_address();

				data_available_to_read.write(false);
				bank_storage[converted_address] = row_buffer;

				/*cout << "bank write enable \n";
				cout << "address " << converted_address << ": ";
				for(int i = 0 ; i < 8; i++)
					cout << row_buffer.range((i+1)*31,i*32).to_uint() << " ";
				cout << endl << endl;*/
				previous_offset = get_offset();
			}
		}
		else if(done[current_fsm].read()){
			current_fsm = 5;
			previous_offset = 16;
			data_available_to_read.write(false);
			bank_available = true;
		}
		else{
			data_available_to_read.write(false);
		}
	}
	else{
		data_available_to_read.write(false);
	}
}

unsigned bank::get_converted_address(){
	switch (MAX_BLOCK_SIZE){
		case 32:
			return address_buffer.range(19,1).to_uint();
		break;
		case 64:
			return address_buffer.range(19,2).to_uint();
		break;
		case 128:
			return address_buffer.range(19,3).to_uint();
		break;
		case 256:
			return address_buffer.range(19,4).to_uint();
		break;
	}
	return 0;
}

void bank::initialize_address(unsigned address, std::vector<unsigned> elem){
	unsigned counter = 0;
	sc_lv<MAX_BLOCK_SIZE*8> tmp;
	for(std::vector<unsigned>::iterator it = elem.begin(); it != elem.end(); ++it){
		if(counter*32 < MAX_BLOCK_SIZE*8)
			tmp.range(31*(counter+1),32*counter) = *it;
		counter++;
	}
	bank_storage[address] = tmp;
}

void bank::print_bank_address(unsigned address){
	int number_of_intergers = MAX_BLOCK_SIZE/4;
	sc_lv<MAX_BLOCK_SIZE*8> tmp = bank_storage[address];
	cout << "address " << address << ": ";
	for(int i = 0; i < number_of_intergers; i++)
		cout << tmp.range(31*(i+1),32*i).to_uint() << " ";
	cout << "\n";
}






