#include "systemc.h"
#include "../../../common/defines.h"
#include "../../../traffic_monitor.h"
#include <queue>
#include <vector>

enum state {IDLE, DECODE, WAIT};

using namespace std;
SC_MODULE(request_decoder){
public:
	int request_decoder_id;
	sc_in_clk clk;
	sc_in<bool> start_decoding;
	sc_in<sc_lv<128> > request;


	/*
	 * To response buffer
	 */
	sc_out<sc_lv<11> > tag;
	sc_out<sc_lv<3> > link;
	sc_out<sc_lv<3> > cub_id;
	sc_out<sc_lv<5> > response_lng;
	sc_out<sc_lv<7> > cmd;
	sc_out<bool > pim_instruction;
	sc_out<bool> response_dst; // 0 if host, 1 if pim
	/*
	 * To bank buffer
	 */

	sc_out<sc_lv<2> > operation; // read/write/atomic
	sc_out<sc_lv<4> > operation_size; //16B - 256B
	sc_out<sc_lv<20> > address;
	sc_out<sc_lv<4> > bank_id;
	sc_out<sc_lv<4> > address_offset;
	sc_out<sc_lv<5> > alu_operation;

	/*
	 * To bank data buffer
	 * en_bank_data_buffer:
	 * 00 - data   header
	 * 01 - header header
	 * 10 - tail   data
	 * 11 - disabled
	 */
	sc_out<sc_lv<128> > data_from_packet;
	sc_out<sc_lv<2> > en_bank_data_buffer[FAW];

	/* At a given time, 4 different banks can
	 * be working at the same time.
	 */


	/*
	 * response buffer controls
	 */
	sc_out<bool> en_bank_response_buffer[FAW];
	sc_out<bool> en_response_buffer;

	/*
	 * request buffer controls
	 */
	sc_out<bool> en_bank_operation_buffer[FAW];


	/* Request does not know the instruction
	 * so send it to the PIM Interface
	 *
	 */
	sc_out<sc_lv<FLIT_SIZE> > packet_to_pim_interface;
	sc_out<bool> en_pim_instruction_buffer;

	SC_CTOR(request_decoder){
		request_decoder_id = 0;
		SC_METHOD(prc_decode);
		sensitive << clk.pos();

		counter = 0;
		round_robin = 0;
		for(int i = 0; i < 4; i++)
			actived_bank_fsm[i] = 0;
		bank_fms_to_send_request = 0;
		current_bank_id = 0 ;
		number_of_packets_to_send = 0;
		counter_tmp = 0;
		hmc_instruction = false;
		is_pim_instruction = false;
		traffic = NULL;
		id = 0;
		current = IDLE;
	}
private:
	unsigned counter;
	state current;
	unsigned number_of_packets_to_send;
	unsigned actived_bank_fsm[FAW];
	unsigned current_bank_id; // current bank id
	int round_robin;
	int bank_fms_to_send_request;

	sc_lv<CMD_WIDTH> cmd_bck;
	sc_lv<TAG_SIZE> tag_bck;
	sc_lv<2*FLIT_SIZE> reg_pim_instruction;
	int counter_tmp;

	bool hmc_instruction;
	bool is_pim_instruction;

	void prc_decode();
	void prc_header_decoder();
	void prc_tail_decoder();
	void prc_data_decoder(int);
	void prc_bank_policy();
	void prc_disable_bank();
	void prc_decode_address();
	void prc_decode_cmd(sc_lv<7> &);
	void prc_decode_pim_instruction(sc_lv<7> &);


    traffic_monitor *traffic;
public:
    void set_traffic_monitor(traffic_monitor *t){
    	traffic = t;
    }

    int id;
};
