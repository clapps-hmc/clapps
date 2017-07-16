#include "serializer_requester.h"

using std::cout;
void serializer_requester::prc_serialize(){

	if(!buffer.empty()){
		if(flits_until_tail == 0){ //decoding header
			flits_until_tail = 8*buffer.front().range(11,7).to_uint() - 1;
		}
		else{
			flits_until_tail--;
		}

		sc_lv<16> ser_data_to_send = buffer.front().range(SERIALIZED_PACKET_WIDTH*counter_actual+15,SERIALIZED_PACKET_WIDTH*counter_actual);
		serialized_flit.write(ser_data_to_send);
		serialized_flit_available.write(true);


		if(counter_actual == 7){ //counter_actual selects the appropriate part of the packet to send
			counter_actual = 0;
			buffer.pop();
		}
		else{
			counter_actual++;
		}
	}
	else{
		serialized_flit_available.write(false);
	}
}

void serializer_requester::prc_receive_flits(){
	if(flit_available.read()){
		buffer.push(flit);
	}
}


