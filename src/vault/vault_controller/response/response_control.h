#ifndef SRC_VAULT_CONTROLLER_RESPONSE_RESPONSE_H_
#define SRC_VAULT_CONTROLLER_RESPONSE_RESPONSE_H_

#include "systemc.h"
#include <queue>
#include "../../../common/defines.h"
#include "../../../traffic_monitor.h"




SC_MODULE(response_control){
enum states_fsm {IDLE, SENDING, FINISHED};
enum data_decoding {DATA_DATA, DATA_TAIL};
public:
	sc_in_clk clk_transeiver, clk_vault;
	sc_in<bool> bank_status[FAW], en_pop, en_pop_from_pim, is_pim_instruction;
	sc_in<sc_lv<11> > tag_from_bank_rsp_buffer[FAW];
	sc_in<sc_lv<11> > tag_from_rsp_buffer;
	sc_in<sc_lv<3> > link_from_pim_rqst;
	sc_in<bool> response_dst[FAW];
	sc_in<sc_lv<3> > link[FAW];
	sc_in<sc_lv<3> > cub[FAW];
	sc_in<sc_lv<5> > response_lng[FAW];
	sc_in<sc_lv<7> > cmd[FAW];
	sc_in<sc_lv<256> >	data[FAW];


	sc_out<sc_lv<128> > packet_out;
	sc_out<sc_lv<128> > packet_out_pim;
	sc_out<bool> packet_available_host;
	sc_out<bool> packet_available_pim;

	sc_out<bool> en_pop_bank_response_buffer[FAW];
	sc_out<bool> rsp_will_send[FAW];
	sc_out<bool> en_pop_data_buffer[FAW];
	sc_out<bool> en_pop_response_buffer;


	SC_CTOR(response_control){
		SC_METHOD(prc_response_control_write);
		sensitive << clk_vault.pos();

		SC_METHOD(prc_response_control_read);
		sensitive << clk_transeiver.pos();

		SC_METHOD(prc_response_control_read_pim);
		sensitive << clk_vault.pos();

		counter = 0;
		data_counter = 0;
		faw_source = 0;
		b_status = false;
		id = 0;
		decoded_tail = false;
		decoding = false;
		reg_is_pim_instruction = false;
		current_state = IDLE;
		num_of_pck = 0;
	    traffic = NULL;

	}

	int id;
private:
	void prc_response_control_write();
	void prc_response_control_read();
	void prc_response_control_read_pim();
	sc_lv<64> creat_header(int);
	sc_lv<64> creat_tail(int);
	sc_lv<64> creat_data(int);
	void creat_response_packet(int);
	unsigned counter;
	unsigned data_counter;
	std::queue<sc_lv<128> > buffer_w;
	std::queue<sc_lv<128> > buffer_r;
	std::queue<sc_lv<128> > buffer_r_pim;
	sc_lv<256> data_tmp;
	bool b_status;
	int faw_source;
	bool decoded_tail;
	bool decoding;
	bool reg_is_pim_instruction;
	sc_lv<11> reg_tag_from_rsp_buffer;
	sc_lv<3> reg_link_from_pim_rqst;
	unsigned num_of_pck;
	sc_lv<128> pck;
	states_fsm current_state;

	sc_lv<11> tag_bck;
	sc_lv<7> cmd_bck;

    traffic_monitor *traffic;
public:
    void set_traffic_monitor(traffic_monitor *t){
    	traffic = t;
    }
};



#endif /* SRC_VAULT_CONTROLLER_RESPONSE_RESPONSE_H_ */
