#include "systemc.h"
#include <queue>
using namespace std;

#include "pim_interface_instruction_buffer.h"
#include "pim_interface_read_write_controller.h"

/*
 * From Request Decoder: flit_from_host, flit_available_from_host
 * To PIM: pim_instruction, pim_instruction_available
 * From PIM: en_pop_pim_instruction_buffer
 *
 */
SC_MODULE(pim_interface){
public:
	/*
	 * General
	 */
	sc_in_clk clk_pim, clk_vault;

	/*
	 * PIM Instruction Buffer
	 */
	sc_in<bool> flit_available_from_host, en_pop_pim_instruction_buffer;
	sc_in<sc_lv<128> > flit_from_host;
	sc_out<sc_lv<128> > pim_instruction;
	sc_out<bool> pim_instruction_available;

	/*
	 * PIM Read/Write Controller
	 */
	sc_in<bool> en_response_from_vault, en_read_request_from_pim, en_write_request_from_pim,
			    en_write_response_from_pim, en_read_response_from_pim;
	sc_in<sc_lv<34> > request_address_from_pim;
	sc_in<sc_lv<4 > > request_size_from_pim;
	sc_in<sc_lv<16*FLIT_SIZE> > request_data_in_from_pim;
	sc_in<sc_lv<128> > response_flit_from_vault;

	sc_out<sc_lv<128> > request_out_to_vault, response_data_out_to_pim;
	sc_out<bool> request_available_to_vault, en_pop_response_buffer_to_vault,
			     response_write_available, response_read_available;


	SC_CTOR(pim_interface){
		mod_instruction_buffer = new pim_instruction_buffer(sc_gen_unique_name("mod_inst_buffer"));
		mod_instruction_buffer -> clk_pim(clk_pim);
		mod_instruction_buffer -> clk_vault(clk_vault);
		mod_instruction_buffer -> en_push(flit_available_from_host);
		mod_instruction_buffer -> en_pop(en_pop_pim_instruction_buffer);
		mod_instruction_buffer -> flit_in(flit_from_host);
		mod_instruction_buffer -> instruction_out(pim_instruction);
		mod_instruction_buffer -> instruction_available(pim_instruction_available);

		mod_read_write_controller = new pim_interface_read_write_controller(sc_gen_unique_name("mod_read_write_contr"));
		mod_read_write_controller -> clk_pim(clk_pim);
		mod_read_write_controller -> clk_vault(clk_vault);
		mod_read_write_controller -> en_read_request(en_read_request_from_pim);
		mod_read_write_controller -> en_write_request(en_write_request_from_pim);
		mod_read_write_controller -> en_response(en_response_from_vault);
		mod_read_write_controller -> en_write_response(en_write_response_from_pim);
		mod_read_write_controller -> en_read_response(en_read_response_from_pim);
		mod_read_write_controller -> address(request_address_from_pim);
		mod_read_write_controller -> size(request_size_from_pim);
		mod_read_write_controller -> data_in(request_data_in_from_pim);
		mod_read_write_controller -> response_in(response_flit_from_vault);
		mod_read_write_controller -> request_out(request_out_to_vault);
		mod_read_write_controller -> data_out(response_data_out_to_pim);
		mod_read_write_controller -> request_available(request_available_to_vault);
		mod_read_write_controller -> en_pop_mem_response_buffer(en_pop_response_buffer_to_vault);
		mod_read_write_controller -> write_response_available(response_write_available);
		mod_read_write_controller -> read_response_available(response_read_available);

	}
private:
	pim_instruction_buffer *mod_instruction_buffer;
	pim_interface_read_write_controller *mod_read_write_controller;
};
