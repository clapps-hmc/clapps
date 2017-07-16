#include "deserializer_responser.h"

void deserializer_responser::prc_deserialize(){
if(serialized_flit_available.read()){
	switch (counter){
			case 0:
				flit_being_reconstructed.range(15,0) = serialized_flit.read();
				counter++;
				break;
			case 1:
				flit_being_reconstructed.range(31,16) = serialized_flit.read();
				counter++;
				break;
			case 2:
				flit_being_reconstructed.range(47,32) = serialized_flit.read();
				counter++;
				break;
			case 3:
				flit_being_reconstructed.range(63,48) = serialized_flit.read();
				counter++;
				break;
			case 4:
				flit_being_reconstructed.range(79,64) = serialized_flit.read();
				counter++;
				break;
			case 5:
				flit_being_reconstructed.range(95,80) = serialized_flit.read();
				counter++;
				break;
			case 6:
				flit_being_reconstructed.range(111,96) = serialized_flit.read();
				counter++;
				break;
			case 7:
				flit_being_reconstructed.range(127,112) = serialized_flit.read();
				buffer.push(flit_being_reconstructed);
				counter = 0;
				break;
		}
	}
}


void deserializer_responser::prc_decode_address(){
	sc_lv<34> address_tmp = buffer.front().range(57,24);

	switch (MAX_BLOCK_SIZE){
		case 32:
			quadrant.write(address_tmp.range(9,8));
			vault.write(address_tmp.range(7,5));
			reg_vault = address_tmp.range(7,5);
			break;
		case 64:
			quadrant.write(address_tmp.range(10,9));
			vault.write(address_tmp.range(8,6));
			reg_vault = address_tmp.range(8,6);
			break;
		case 128:
			quadrant.write(address_tmp.range(11,10));
			vault.write(address_tmp.range(9,7));
			reg_vault = address_tmp.range(9,7);
			break;
		case 256:
			quadrant.write(address_tmp.range(12,11));
			vault.write(address_tmp.range(10,8));
			reg_vault = address_tmp.range(10,8);
			break;
	}

}
void deserializer_responser::prc_send_flits(){
	switch(current){
	case IDLE:
		for(int i = 0; i < VAULTS_PER_QUADRANT; i++)
			deserialized_flit_available[i].write(false);

		counter_send = 0;
		if(!buffer.empty()){
			prc_decode_address();
			num_flits = buffer.front().range(11,7).to_uint();
			current = SENDING;
		}
		else
			current = IDLE;
		break;
	case SENDING:
		deserialized_flit_available[reg_vault.to_uint()].write(true);
		deserialized_flit.write(buffer.front());
		buffer.pop();
		counter_send++;
		if(counter_send == num_flits)
			current = IDLE;
		else
			current = WAIT;
		break;
	case WAIT:
		deserialized_flit_available[reg_vault.to_uint()].write(false);

		if(!buffer.empty())
			current = SENDING;
		else
			current = WAIT;
		break;
	}
}

