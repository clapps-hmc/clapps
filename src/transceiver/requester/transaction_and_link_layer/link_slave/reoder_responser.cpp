
#include "reorder_responser.h"

void reorder_response::prc_reorder_response(){
	switch(current){
		case IDLE:
			for(int i=0; i < NUMBER_OF_LINKS; i++)
				en_pop_physical_rx_queue[i].write(false);

			en_pop_reorder_queue.write(false);
			set_mux.write(false);

			counter = 0;
			num_flits = 0;
			link_to_send = 0;

			for(int i=0; i < NUMBER_OF_LINKS; i++){
				if(request_tag.read() == response_flit[i].read().range(22,12) && physical_rx_flit_available[i].read()){
					link_to_send = i;
					num_flits = response_flit[link_to_send].read().range(11,7).to_uint();
					current = SET_SIGNALS;
					break;
				}
			}

			break;
		case SET_SIGNALS:
			en_pop_physical_rx_queue[link_to_send].write(true);
			set_mux.write(true);

			mux_select.write(link_to_send);
			current = WAIT_;

			break;
		case WAIT_:
			en_pop_physical_rx_queue[link_to_send].write(false);
			counter++;
			if(counter == num_flits)
				current = DONE;
			else
				current = WAIT;

			break;
		case WAIT:

			en_pop_physical_rx_queue[link_to_send].write(false);

			if(physical_rx_flit_available[link_to_send].read()) current = SET_SIGNALS;
			else current = WAIT;

			break;
		case DONE:
			set_mux.write(false);
			en_pop_physical_rx_queue[link_to_send].write(false);
			en_pop_reorder_queue.write(true);

			current = IDLE;
			break;
		}
}
