#include "systemc.h"
#include <queue>

SC_MODULE(bank_request_buffer){
public:
	sc_in_clk clk_vault;
	sc_in<bool> en_push, en_pop;
	sc_in<sc_lv<20> > address_in;
	sc_in<sc_lv<11> > tag_in;
	sc_in<sc_lv<2> > operation_in;
	sc_in<sc_lv<4> > operation_size_in;
	sc_in<sc_lv<4> > bank_in;
	sc_in<sc_lv<4> > offset_in;
	sc_in<sc_lv<5> > alu_operation_in;

	sc_out<sc_lv<4> > operation_size_out;
	sc_out<sc_lv<2> > operation_out; // read, write or atomic
	sc_out<sc_lv<20> > address_out;
	sc_out<sc_lv<11> > tag_out;
	sc_out<sc_lv<4> > bank_out;
	sc_out<sc_lv<4> > offset_out;
	sc_out<sc_lv<5> > alu_operation_out;

	SC_CTOR(bank_request_buffer){
		SC_METHOD(prc_bank_request_buffer_write);
		sensitive << clk_vault.pos();
		SC_METHOD(prc_bank_request_buffer_read);
		sensitive << clk_vault.pos();
		buffer_size_counter = 0;
		fsm_already_actived = false;
		vault_id = 0;
		bank_request_id = 0;
		max_buffer = 0;
	}

	int vault_id;
	int bank_request_id;
private:
	std::queue<sc_lv<20> > address;
	std::queue<sc_lv<11> > tag;
	std::queue<sc_lv<2> > operation;
	std::queue<sc_lv<4> > operation_size;
	std::queue<sc_lv<4> > bank;
	std::queue<sc_lv<4> > offset;
	std::queue<sc_lv<5> > alu_operation;

	unsigned buffer_size_counter;
	unsigned max_buffer;
	void prc_bank_request_buffer_write();
	void prc_bank_request_buffer_read();
	bool fsm_already_actived;

};
