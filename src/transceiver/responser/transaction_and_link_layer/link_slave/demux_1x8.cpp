#include "demux_1x8.h"

void demux_1x8::prc_demux_1x8(){
	unsigned tmp = select.read().to_uint();
    for(unsigned i = 0; i < 8; i++){
    	if(i == tmp){
    	    data_out[i].write(data_in.read());
    	}
    }
}
