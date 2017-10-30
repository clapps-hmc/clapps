#include "memory_controller.h"

#include <math.h>
#include <sysc/communication/sc_signal_ports.h>
#include <sysc/datatypes/bit/sc_lv.h>
#include <sysc/datatypes/bit/sc_lv_base.h>
#include <sysc/datatypes/bit/sc_proxy.h>
#include <sysc/kernel/sc_simcontext.h>
#include <sysc/kernel/sc_time.h>
#include <iostream>


void memory_controller::prc_fsm(){
	switch(current){
		case IDLE:

			previous = IDLE;
			cs.write(false); ras.write(false); cas.write(false); we.write(false); exec.write(false); done.write(false);

			en_pop_bank_request_buffer.write(false);
			en_pop_bank_data_request_buffer.write(false);
			en_pop_pim_data_request_buffer.write(false);

			counter_request_lng = 0;

			if(operation_in.read() == "01" || operation_in.read() == "00" || operation_in.read() == "10")
				current = CPY;
			else
				current = IDLE;
		break;
		case CPY:

			previous = CPY;

			reg_operation = operation_in.read();
			reg_operation_size = operation_size_in.read();
			reg_offset = offset_in.read();
			reg_bank_id = bank_id_in.read();
			reg_address = address_in.read();
			reg_pim_offset = offset_in.read();
			reg_pim_operation = pim_operation_in.read();


			operation_out.write(reg_operation);
			address_out.write(reg_address);
			bank_id_out.write(reg_bank_id);
			offset_out.write(reg_offset);
			pim_operation_out.write(reg_pim_operation);
			pim_offset_out.write(reg_pim_offset);

			en_pop_bank_request_buffer.write(true);
			en_pop_bank_data_request_buffer.write(false);
			en_pop_pim_data_request_buffer.write(false);


			current = CS;
		break;
		case CS:
			previous = CS;

			cs.write(true); ras.write(false); cas.write(false); we.write(false); oe.write(false); exec.write(false); done.write(false);

			en_pop_bank_request_buffer.write(false);
			en_pop_bank_data_request_buffer.write(false);

			current = RAS;
		break;
		case RAS:
			previous = RAS;

			cs.write(false); ras.write(true); cas.write(false); we.write(false); oe.write(false); exec.write(false); done.write(false);

			en_pop_bank_request_buffer.write(false);
			en_pop_bank_data_request_buffer.write(false);
			en_pop_pim_data_request_buffer.write(false);


			current = CAS;
		break;
		case CAS:
			previous = CAS;

			cs.write(false); ras.write(false); cas.write(true); we.write(false); oe.write(false); exec.write(false); done.write(false);

			en_pop_bank_request_buffer.write(false);
			en_pop_bank_data_request_buffer.write(false);
			en_pop_pim_data_request_buffer.write(false);

			if(reg_operation == "00" || reg_operation == "10"){
				current = OE;
			}
			else if(reg_operation == "01")
				current = WE;

		break;
		case OE:
			previous = OE;

			cs.write(false); ras.write(false); cas.write(false); we.write(false); oe.write(false); exec.write(false), done.write(false);

			en_pop_bank_request_buffer.write(false);
			en_pop_bank_data_request_buffer.write(false);
			en_pop_pim_data_request_buffer.write(false);


			number_of_tsv_requests = ((reg_operation_size.to_uint()+1)*16)/((float)TSV_WIDTH);

			if(counter_request_lng == (ceil(number_of_tsv_requests))){
				if(reg_operation == "00")
					current = DONE;
				else
					current = EXEC;
			}
			else{
				offset_out.write(reg_offset);
				unsigned next_offset = (reg_offset.to_uint() + 2);
				reg_offset = next_offset;

				counter_request_lng++;
				current = TSV_CONTROL;
			}
		break;
		case WE:
			previous = WE;

			cs.write(false); ras.write(false); cas.write(false); we.write(false); oe.write(false); exec.write(false), done.write(false);

			en_pop_bank_request_buffer.write(false);
			en_pop_bank_data_request_buffer.write(false);
			en_pop_pim_data_request_buffer.write(false);

			number_of_tsv_requests = ((reg_operation_size.to_uint()+1)*16)/((float)TSV_WIDTH);
			if(counter_request_lng == (ceil(number_of_tsv_requests)))
				current = DONE;
			else{
				offset_out.write(reg_offset);
				unsigned next_offset = (reg_offset.to_uint() + 2);
				reg_offset = next_offset;

				counter_request_lng++;
				current = TSV_CONTROL;
			}
		break;
		case EXEC:

			cs.write(false); ras.write(false); cas.write(false); we.write(false); oe.write(false); exec.write(true), done.write(false);
			en_pop_bank_request_buffer.write(false);
			en_pop_bank_data_request_buffer.write(false);
			en_pop_pim_data_request_buffer.write(true);

			counter_request_lng = 0;
			current = WE;

			break;
		case TSV_CONTROL:
			if(previous == WE){
				cs.write(false); ras.write(false); cas.write(false); we.write(true); oe.write(false); exec.write(false), done.write(false);
				if(ack_from_tsv.read()){
					en_pop_bank_data_request_buffer.write(true);
					current = WE;
				}
				else{
					current = TSV_CONTROL;
				}
			}
			else if(previous == OE){
				cs.write(false); ras.write(false); cas.write(false); we.write(false); oe.write(true); exec.write(false), done.write(false);;

				if(ack_from_tsv.read())
					current = OE;
				else
					current = TSV_CONTROL;
			}


		break;
		case DONE:
			previous = DONE;
			cs.write(false); ras.write(false); cas.write(false); we.write(false); oe.write(false); exec.write(false), done.write(true);

			en_pop_pim_data_request_buffer.write(false);

			if(rsp_will_send.read())
				current = IDLE;
			else
				current = DONE;
		break;
	}
}


