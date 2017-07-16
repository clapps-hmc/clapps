/*
 * vault_controller.h
 *
 *  Created on: 7 de nov de 2016
 *      Author: root
 */

#ifndef SRC_VAULT_CONTROLLER_VAULT_CONTROLLER_H_
#define SRC_VAULT_CONTROLLER_VAULT_CONTROLLER_H_

#include "request/request_decoder.h"
#include "request/bank_request_buffer.h"
#include "request/data_request_buffer.h"
#include "request/request_traffic_controller.h"
#include "response/response_control.h"
#include "response/response_buffer.h"
#include "response/data_response_buffer.h"
#include "response/bank_response_buffer.h"
#include "memory_controller/memory_controller.h"
#include "tsv_controller/tsv_controller.h"
#include "pim/pim_hmc/pim_hmc.h"
#include "pim/pim_hmc/pim_hmc_data_buffer.h"
#include "pim/pim_interface/pim_interface.h"
#include "rvu/rvu.h"


SC_MODULE(vault_controller){
public:
	/*
	 * From transceiver
	 */
	sc_in_clk clk_transeiver, clk_vault, clk_pim;
	sc_in<bool> request_from_host_available, reset, en_pop_response;
	sc_in<sc_lv<128> > request_from_host;

	/*
	 * From banks
	 */
	sc_in<sc_lv<256> > data_from_banks[NUMBER_OF_BANKS];
	sc_in<bool> data_available_to_read[NUMBER_OF_BANKS];


	/*
	 * To banks
	 */
	sc_out<bool> cs[FAW], ras[FAW], cas[FAW], we[FAW], oe[FAW], done[FAW], write_available[NUMBER_OF_BANKS];
	sc_out<sc_lv<LOG_NUMBER_OF_BANKS> > bank_id[FAW];
	sc_out<sc_lv<4> > offset[FAW];
	sc_out<sc_lv<20> > address[FAW];
	sc_out<sc_lv<256> > data_to_banks[NUMBER_OF_BANKS];

	/*
	 * To transceiver
	 */
	sc_out<sc_lv<128> > response;
	sc_out<bool> response_available;

	SC_CTOR(vault_controller){
		mod_request_traffic_controller = new request_traffic_controller(sc_gen_unique_name("mod_request_traffic_controller"));
		mod_request_traffic_controller -> clk_transiever(clk_transeiver);
		mod_request_traffic_controller -> clk_vault(clk_vault);
		mod_request_traffic_controller -> clk_pim(clk_pim);
		mod_request_traffic_controller -> request_from_host(request_from_host);
		mod_request_traffic_controller -> request_from_host_available(request_from_host_available);
		mod_request_traffic_controller -> request_from_pim(request_from_pim_s);
		mod_request_traffic_controller -> request_from_pim_available(request_from_pim_available_s);
		mod_request_traffic_controller -> request_to_vault(request_to_rqst_decoder_s);
		mod_request_traffic_controller -> request_available_to_vault(request_available_to_vault_s);

		mod_request_decoder = new request_decoder(sc_gen_unique_name("rsqt_decoder"));
		mod_request_decoder -> clk(clk_vault);
		mod_request_decoder -> start_decoding(request_available_to_vault_s);
		mod_request_decoder -> request(request_to_rqst_decoder_s);

		mod_request_decoder -> tag(tag_s);
		mod_request_decoder -> link(link_s);
		mod_request_decoder -> cub_id(cub_id_s);
		mod_request_decoder -> response_lng(response_lng_s);
		mod_request_decoder -> cmd(cmd_s);
		mod_request_decoder -> pim_instruction(pim_instruction_s);
		mod_request_decoder -> response_dst(response_dst_s);
		mod_request_decoder -> packet_to_pim_interface(packet_to_pim_interface_s);
		mod_request_decoder -> en_pim_instruction_buffer(en_pim_instruction_buffer);

		mod_request_decoder -> operation(operation_s);
		mod_request_decoder -> operation_size(operation_size_s);
		mod_request_decoder -> address(address_s);
		mod_request_decoder -> bank_id(bank_id_s);
		mod_request_decoder -> address_offset(address_offset_s);
		mod_request_decoder -> alu_operation(alu_operation_from_rqst_s);

		mod_request_decoder -> data_from_packet(data_from_packet_s);
		mod_request_decoder -> en_response_buffer(en_response_buffer_s);
		for(int i = 0; i < FAW; i++){
			mod_request_decoder -> en_bank_response_buffer[i](en_bank_response_buffer_s[i]);
			mod_request_decoder -> en_bank_operation_buffer[i](en_bank_operation_buffer_s[i]);
			mod_request_decoder -> en_bank_data_buffer[i](en_bank_data_buffer_s[i]);
		}

		for(int i = 0; i < FAW; i++){
			mod_bank_request_buffer[i] = new bank_request_buffer(sc_gen_unique_name("mod_bank_request_buffer"));
			mod_bank_request_buffer[i] -> clk_vault(clk_vault);

			mod_bank_request_buffer[i] -> en_pop(en_pop_bank_request_buffer_s[i]);
			mod_bank_request_buffer[i] -> en_push(en_bank_operation_buffer_s[i]);
			mod_bank_request_buffer[i] -> address_in(address_s);
			mod_bank_request_buffer[i] -> tag_in(tag_s);
			mod_bank_request_buffer[i] -> operation_in(operation_s);
			mod_bank_request_buffer[i] -> operation_size_in(operation_size_s);
			mod_bank_request_buffer[i] -> bank_in(bank_id_s);
			mod_bank_request_buffer[i] -> offset_in(address_offset_s);
			mod_bank_request_buffer[i] -> alu_operation_in(alu_operation_from_rqst_s);

			mod_bank_request_buffer[i] -> operation_size_out(operation_size_out_s[i]);
			mod_bank_request_buffer[i] -> operation_out(operation_out_s[i]);
			mod_bank_request_buffer[i] -> address_out(address_out_s[i]);
			mod_bank_request_buffer[i] -> tag_out(tag_out_s[i]);
			mod_bank_request_buffer[i] -> bank_out(bank_out_s[i]);
			mod_bank_request_buffer[i] -> offset_out(offset_out_s[i]);
			mod_bank_request_buffer[i] -> alu_operation_out(alu_operation_from_buffer_s[i]);
		}

		for(int i = 0; i < FAW; i++){
			mod_data_request_buffer[i] = new data_request_buffer(sc_gen_unique_name("mod_data_request_buffer"));
			mod_data_request_buffer[i] -> clk_vault(clk_vault);
			mod_data_request_buffer[i] -> en_pop(en_pop_data_request_buffer_s[i]);
			mod_data_request_buffer[i] -> en_push(en_bank_data_buffer_s[i]);
			mod_data_request_buffer[i] -> data_in(data_from_packet_s);
			mod_data_request_buffer[i] -> data_out(data_to_bank_in_s[i]);
		}

		for(int i = 0; i < FAW; i++){
			mod_memory_controller[i] = new memory_controller(sc_gen_unique_name("mod_memory_controller"));
			mod_memory_controller[i] -> clk(clk_vault);
			mod_memory_controller[i] -> operation_in(operation_out_s[i]);
			mod_memory_controller[i] -> operation_size_in(operation_size_out_s[i]);
			mod_memory_controller[i] -> offset_in(offset_out_s[i]);
			mod_memory_controller[i] -> address_in(address_out_s[i]);
			mod_memory_controller[i] -> bank_id_in(bank_out_s[i]);
			mod_memory_controller[i] -> rsp_will_send(rsp_will_send_s[i]);
			mod_memory_controller[i] -> ack_from_tsv(ack_from_tsv_s[i]);
			mod_memory_controller[i] -> pim_operation_in(alu_operation_from_buffer_s[i]);
			mod_memory_controller[i] -> en_pop_bank_request_buffer (en_pop_bank_request_buffer_s[i]);
			mod_memory_controller[i] -> en_pop_bank_data_request_buffer(en_pop_data_request_buffer_s[i]);

			mod_memory_controller[i] -> cs(cs[i]);
			mod_memory_controller[i] -> we(we[i]);
			mod_memory_controller[i] -> oe(oe[i]);
			mod_memory_controller[i] -> ras(ras[i]);
			mod_memory_controller[i] -> cas(cas[i]);
			mod_memory_controller[i] -> done(done[i]);
			mod_memory_controller[i] -> offset_out(offset[i]);
			mod_memory_controller[i] -> bank_id_out(bank_id[i]);
			mod_memory_controller[i] -> address_out(address[i]);
			mod_memory_controller[i] -> en_pop_pim_data_request_buffer(en_pop_pim_data_request_buffer_s[i]);
			mod_memory_controller[i] -> exec(exec_s[i]);
			mod_memory_controller[i] -> pim_offset_out(pim_offset_out_s[i]);
			mod_memory_controller[i] -> pim_operation_out(pim_operation_out_s[i]);
			mod_memory_controller[i] -> operation_out(operation_out_to_tsv_s[i]);
		}


		mod_tsv_controller = new tsv_controller(sc_gen_unique_name("tsv_controller"));
		mod_tsv_controller -> clk(clk_vault);
		for(int i = 0; i < FAW; i++){
			mod_tsv_controller -> we[i](we[i]);
			mod_tsv_controller -> oe[i](oe[i]);
			mod_tsv_controller -> bank_id[i](bank_id[i]);
			mod_tsv_controller -> data_from_data_buffer[i](data_to_bank_in_s[i]);
			mod_tsv_controller -> ack_request[i](ack_from_tsv_s[i]);
			mod_tsv_controller -> data_to_data_buffer[i](data_to_rsp_buffer_s[i]);
			mod_tsv_controller -> en_push_data_buffer[i](en_push_data_rsp_buffer_s[i]);
			mod_tsv_controller -> data_from_pim_data_buffer[i](pim_data_out_s[i]);
			mod_tsv_controller -> fsm_operation[i](operation_out_to_tsv_s[i]);
			mod_tsv_controller -> en_push_pim_data_buffer[i](en_push_pim_data_buffer_s[i]);
		}
		for(int i = 0; i < NUMBER_OF_BANKS; i++){
			 mod_tsv_controller -> data_from_bank[i](data_from_banks[i]);
			 mod_tsv_controller -> data_available_to_read[i](data_available_to_read[i]);

			 mod_tsv_controller -> write_available[i](write_available[i]);
			 mod_tsv_controller -> data_to_bank[i](data_to_banks[i]);
		}


		for(int i = 0; i < FAW; i++){
			mod_pim_hmc[i] = new pim_hmc(sc_gen_unique_name("mod_pim_hmc"));
			mod_pim_hmc[i] -> data_in(pim_data_buffer_out_s[i]);
			mod_pim_hmc[i] -> operate(exec_s[i]);
			mod_pim_hmc[i] -> immediate_in(data_to_bank_in_s[i]);
			mod_pim_hmc[i] -> data_select(pim_offset_out_s[i]);
			mod_pim_hmc[i] -> alu_operation(pim_operation_out_s[i]);
			mod_pim_hmc[i] -> data_out(pim_data_out_s[i]);
		}

		for(int i = 0; i < FAW; i++){
			mod_pim_hmc_data_buffer[i] = new pim_hmc_data_buffer(sc_gen_unique_name("mod_pim_hmc_data_buffer"));
			mod_pim_hmc_data_buffer[i] -> clk(clk_vault);
			mod_pim_hmc_data_buffer[i] -> data_in(data_to_rsp_buffer_s[i]);
			mod_pim_hmc_data_buffer[i] -> en_push(en_push_pim_data_buffer_s[i]);
			mod_pim_hmc_data_buffer[i] -> en_pop(en_pop_pim_data_request_buffer_s[i]);
			mod_pim_hmc_data_buffer[i] -> data_out(pim_data_buffer_out_s[i]);
		}

		for(int i = 0; i < FAW; i++){
			mod_bank_response_buffer[i] = new bank_response_buffer(sc_gen_unique_name("mod_bank_response_buffer"));
			mod_bank_response_buffer[i] -> clk_vault(clk_vault);
			mod_bank_response_buffer[i] -> en_push(en_bank_response_buffer_s[i]);
			mod_bank_response_buffer[i] -> en_pop(en_pop_bank_response_buffer_s[i]);
			mod_bank_response_buffer[i] -> tag_in(tag_s);
			mod_bank_response_buffer[i] -> link_in(link_s);
			mod_bank_response_buffer[i] -> cub_in(cub_id_s);
			mod_bank_response_buffer[i] -> response_lng_in(response_lng_s);
			mod_bank_response_buffer[i] -> cmd_in(cmd_s);
			mod_bank_response_buffer[i] -> response_dst_in(response_dst_s);
			mod_bank_response_buffer[i] -> tag_out(tag_out_rsp_buffer_s[i]);
			mod_bank_response_buffer[i] -> link_out(link_out_rsp_buffer_s[i]);
			mod_bank_response_buffer[i] -> cub_out(cub_out_rsp_buffer_s[i]);
			mod_bank_response_buffer[i] -> response_lng_out(response_out_rsp_buffer_s[i]);
			mod_bank_response_buffer[i] -> cmd_out(cmd_out_rsp_buffer_s[i]);
			mod_bank_response_buffer[i] -> response_dst_out(response_dst_out_s[i]);
		}

		for(int i = 0; i < FAW; i++){
			mod_data_response_buffer[i] = new data_response_buffer(sc_gen_unique_name("mod_data_response_buffer"));
			mod_data_response_buffer[i] -> clk(clk_vault);
			mod_data_response_buffer[i] -> en_push(en_push_data_rsp_buffer_s[i]);
			mod_data_response_buffer[i] -> en_pop(en_pop_data_rst_buffer_s[i]);
			mod_data_response_buffer[i] -> data_in(data_to_rsp_buffer_s[i]);
			mod_data_response_buffer[i] -> data_out(data_from_rsp_buffer_s[i]);
		}

		mod_response_buffer = new response_buffer(sc_gen_unique_name("mod_response_buffer"));
		mod_response_buffer -> clk_vault(clk_vault);
		mod_response_buffer -> en_push(en_response_buffer_s);
		mod_response_buffer -> is_pim_instruction_in(pim_instruction_s);
		mod_response_buffer -> en_pop(en_pop_response_buffer_s);
		mod_response_buffer -> tag_in(tag_s);
		mod_response_buffer -> link_in(link_s);
		mod_response_buffer -> tag_out(tag_rsp_s);
		mod_response_buffer -> is_pim_instruction_out(pim_instruction_out_s);
		mod_response_buffer -> link_out(link_out_from_rsp_buffer_s);

		mod_response_control = new response_control(sc_gen_unique_name("mod_response_control"));
		mod_response_control -> clk_vault(clk_vault);
		mod_response_control -> clk_transeiver(clk_transeiver);
		mod_response_control -> en_pop(en_pop_response);
		mod_response_control -> en_pop_from_pim(en_pop_response_buffer_to_vault_s);
		mod_response_control -> tag_from_rsp_buffer(tag_rsp_s);
		mod_response_control -> packet_out(response);
		mod_response_control -> packet_out_pim(rsp_packet_to_pim_s);
		mod_response_control -> packet_available_pim(rsp_packet_available_to_pim_s);

		mod_response_control -> packet_available_host(response_available);
		mod_response_control -> en_pop_response_buffer(en_pop_response_buffer_s);
		mod_response_control -> is_pim_instruction(pim_instruction_out_s);
		mod_response_control -> link_from_pim_rqst(link_out_from_rsp_buffer_s);
		for(int i = 0; i < FAW; i++){
			mod_response_control -> response_dst[i](response_dst_out_s[i]);
			mod_response_control -> bank_status[i](done[i]);
			mod_response_control -> tag_from_bank_rsp_buffer[i](tag_out_rsp_buffer_s[i]);
			mod_response_control -> link[i](link_out_rsp_buffer_s[i]);
			mod_response_control -> cub[i](cub_out_rsp_buffer_s[i]);
			mod_response_control -> response_lng[i](response_out_rsp_buffer_s[i]);
			mod_response_control -> cmd[i](cmd_out_rsp_buffer_s[i]);
			mod_response_control -> data[i](data_from_rsp_buffer_s[i]);
			mod_response_control -> en_pop_bank_response_buffer[i](en_pop_bank_response_buffer_s[i]);
			mod_response_control -> en_pop_data_buffer[i](en_pop_data_rst_buffer_s[i]);
			mod_response_control -> rsp_will_send[i](rsp_will_send_s[i]);
		}

		mod_pim_interface = new pim_interface(sc_gen_unique_name("mod_pim_interface"));
		mod_pim_interface -> clk_vault(clk_vault);
		mod_pim_interface -> clk_pim(clk_pim);
		mod_pim_interface -> flit_available_from_host(en_pim_instruction_buffer);
		mod_pim_interface -> en_pop_pim_instruction_buffer(en_pop_pim_inst_buffer_s);
		mod_pim_interface -> flit_from_host(packet_to_pim_interface_s);
		mod_pim_interface -> pim_instruction(pim_instr_s);
		mod_pim_interface -> pim_instruction_available(pim_instr_avl_s);
		mod_pim_interface -> en_response_from_vault(rsp_packet_available_to_pim_s);
		mod_pim_interface -> en_read_request_from_pim(rqst_read_s);
		mod_pim_interface -> en_write_request_from_pim(rqst_write_s);
		mod_pim_interface -> en_write_response_from_pim(rsp_write_s);
		mod_pim_interface -> en_read_response_from_pim(rsp_read_s);
		mod_pim_interface -> request_address_from_pim(rqst_addr_pim_s);
		mod_pim_interface -> request_size_from_pim(rqst_size_pim_s);
		mod_pim_interface -> request_data_in_from_pim(rqst_data_pim_s);
		mod_pim_interface -> response_flit_from_vault(rsp_packet_to_pim_s);
		mod_pim_interface -> request_out_to_vault(request_from_pim_s);
		mod_pim_interface -> response_data_out_to_pim(response_data_out_to_pim_s); //flit back to rvu
		mod_pim_interface -> request_available_to_vault(request_from_pim_available_s);
		mod_pim_interface -> en_pop_response_buffer_to_vault(en_pop_response_buffer_to_vault_s);
		mod_pim_interface -> response_write_available(response_write_available_s); //bool to rvu
		mod_pim_interface -> response_read_available(response_read_available_s); //bool to rvu

		mod_rvu = new rvu(sc_gen_unique_name("mod_rvu"));
		mod_rvu -> clk_rvu(clk_pim);
		mod_rvu -> rvu_instruction(pim_instr_s);
		mod_rvu -> rvu_instruction_available(pim_instr_avl_s);
		mod_rvu -> read_response(response_data_out_to_pim_s);
		mod_rvu -> read_response_available(response_read_available_s);
		mod_rvu -> write_response_available(response_write_available_s);
		mod_rvu -> en_pop_instruction_buffer(en_pop_pim_inst_buffer_s);
		mod_rvu -> en_read_request(rqst_read_s);
		mod_rvu -> en_write_request(rqst_write_s);
		mod_rvu -> en_pop_read_response(rsp_read_s);
		mod_rvu -> en_pop_write_response(rsp_write_s);
		mod_rvu -> address(rqst_addr_pim_s);
		mod_rvu -> size(rqst_size_pim_s);
		mod_rvu -> write_request(rqst_data_pim_s);
	}

public:
	request_decoder * mod_request_decoder;
	bank_request_buffer *mod_bank_request_buffer[FAW];
	data_request_buffer *mod_data_request_buffer[FAW];
	memory_controller *mod_memory_controller[FAW];
	tsv_controller *mod_tsv_controller;
	bank_response_buffer *mod_bank_response_buffer[FAW];
	data_response_buffer *mod_data_response_buffer[FAW];
	response_buffer *mod_response_buffer;
	response_control *mod_response_control;
	pim_hmc *mod_pim_hmc[FAW];
	pim_hmc_data_buffer *mod_pim_hmc_data_buffer[FAW];
	request_traffic_controller *mod_request_traffic_controller;
	pim_interface *mod_pim_interface;
	rvu *mod_rvu;



	sc_signal<sc_lv<11> > tag_s;
	sc_signal<sc_lv<3> > link_s, link_out_rsp_buffer_s[FAW];
	sc_signal<sc_lv<3> > cub_id_s, cub_out_rsp_buffer_s[FAW];
	sc_signal<sc_lv<5> > response_lng_s, response_out_rsp_buffer_s[FAW];
	sc_signal<sc_lv<7> > cmd_s, cmd_out_rsp_buffer_s[FAW];
	sc_signal<sc_lv<2> > operation_s, en_bank_data_buffer_s[FAW];
	sc_signal<sc_lv<4> > operation_size_s;
	sc_signal<sc_lv<20> > address_s;
	sc_signal<sc_lv<4> > bank_id_s, address_offset_s;
	sc_signal<bool> en_bank_response_buffer_s[FAW];
	sc_signal<bool> en_response_buffer_s;
	sc_signal<bool> en_bank_operation_buffer_s[FAW];
	sc_signal<sc_lv<4> > operation_size_out_s[FAW];
	sc_signal<sc_lv<2> > operation_out_s [FAW];
	sc_signal<sc_lv<20> > address_out_s[FAW];
	sc_signal<sc_lv<11> > tag_out_s[FAW], tag_out_rsp_buffer_s[FAW], tag_rsp_s;
	sc_signal<sc_lv<4> > bank_out_s[FAW];
	sc_signal<bool> send_en_s[FAW];
	sc_signal<bool> cs_s[FAW], we_s[FAW], oe_s[FAW], ras_s[FAW], cas_s[FAW], done_s[FAW], en_pop_bank_request_buffer_s[FAW];
	sc_signal<sc_lv<4> > offset_in_s[FAW], offset_out_s[FAW], offset_to_tsv_s[FAW];
	sc_signal<sc_lv<128> > data_from_packet_s;
	sc_signal<sc_lv<256> > data_to_rsp_buffer_s[FAW], data_from_rsp_buffer_s[FAW], data_to_bank_in_s[FAW];
	sc_signal<bool> en_pop_bank_response_buffer_s[FAW];
	sc_signal<bool> en_pop_data_rst_buffer_s[FAW], en_pop_data_request_buffer_s[FAW], rsp_will_send_s[FAW], en_pop_response_buffer_s;
	sc_signal<bool> en_push_data_rsp_buffer_s[FAW], ack_from_tsv_s[FAW];

	sc_signal<sc_lv<5> > alu_operation_from_rqst_s, alu_operation_from_buffer_s[FAW];
	sc_signal<bool> en_pop_pim_data_request_buffer_s[FAW], exec_s[FAW];
	sc_signal<sc_lv<4> > pim_offset_out_s[FAW];
	sc_signal<sc_lv<5> > pim_operation_out_s[FAW];
	sc_signal<sc_lv<2 > > operation_out_to_tsv_s[FAW];
	sc_signal<sc_lv<256> > pim_data_buffer_out_s[FAW], pim_data_out_s[FAW];
	sc_signal<bool> en_push_pim_data_buffer_s[FAW];

	sc_signal<bool> request_from_pim_available_s, request_available_to_vault_s, pim_instruction_s, response_dst_s, en_pim_instruction_buffer;
	sc_signal<bool> response_dst_out_s[FAW], pim_instruction_out_s, rsp_packet_available_to_pim_s;
	sc_signal<sc_lv<FLIT_SIZE> > request_from_pim_s, request_to_rqst_decoder_s, packet_to_pim_interface_s, rsp_packet_to_pim_s;
	sc_signal<sc_lv<3> > link_out_from_rsp_buffer_s;

	sc_signal<bool> en_pop_pim_inst_buffer_s, pim_instr_avl_s, rqst_read_s, rqst_write_s,
				    rsp_write_s, rsp_read_s, en_pop_response_buffer_to_vault_s, response_write_available_s,
					response_read_available_s;
	sc_signal<sc_lv<FLIT_SIZE> > pim_instr_s, response_data_out_to_pim_s;
	sc_signal<sc_lv<34> > rqst_addr_pim_s;
	sc_signal<sc_lv<4> > rqst_size_pim_s;
	sc_signal<sc_lv<16*FLIT_SIZE> > rqst_data_pim_s;
};

#endif /* SRC_VAULT_CONTROLLER_VAULT_CONTROLLER_H_ */
