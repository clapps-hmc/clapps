#include "systemc.h"
#include <queue>

SC_MODULE(bank_response_buffer){
public:
	sc_in_clk clk_vault;
	sc_in<bool>  en_push, en_pop;

	sc_in<sc_lv<11> > tag_in;
	sc_in<sc_lv<3> > link_in;
	sc_in<sc_lv<3> > cub_in;
	sc_in<sc_lv<5> > response_lng_in;
	sc_in<sc_lv<7> > cmd_in;
	sc_in<bool> response_dst_in;

	sc_out<sc_lv<11> > tag_out;
	sc_out<sc_lv<3> > link_out;
	sc_out<sc_lv<3> > cub_out;
	sc_out<sc_lv<5> > response_lng_out;
	sc_out<sc_lv<7> > cmd_out;
	sc_out<bool> response_dst_out;

	SC_CTOR(bank_response_buffer){
		SC_METHOD(prc_bank_response_buffer_write);
		sensitive << clk_vault.pos();
		SC_METHOD(prc_bank_response_buffer_read);
		sensitive << clk_vault.pos();

		buffer_size_counter = 0;
	}
private:
	std::queue<sc_lv<11> > tag;
	std::queue<sc_lv<3> > link;
	std::queue<sc_lv<4> > cub;
	std::queue<sc_lv<5> > response_lng;
	std::queue<sc_lv<7> > cmd;
	std::queue<bool> response_dst;

	unsigned buffer_size_counter;
	void prc_bank_response_buffer_write();
	void prc_bank_response_buffer_read();

};
