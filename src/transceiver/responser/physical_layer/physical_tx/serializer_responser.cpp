#include "serializer_responser.h"

void serializer_responser::prc_store_flits(){
	if(flit_from_link_master_available.read()){
		buffer.push(flit_from_link_master.read());
	}

}
void serializer_responser::prc_serializer(){
	if(!buffer.empty()){
		sc_lv<128> temp = buffer.front();

		serializer_flit.write(temp.range((counter+1)*16 -1, counter*16));
		if(counter == 7){
			counter = 0;
			buffer.pop();
		}
		else{
			counter++;
		}
		serializer_flit_available.write(true);
	}
	else{
		serializer_flit_available.write(false);
	}
}
