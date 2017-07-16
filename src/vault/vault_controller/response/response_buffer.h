#include "systemc.h"
#include <queue>

SC_MODULE(response_buffer){
public:
	sc_in_clk clk_vault;
	sc_in<bool> en_push, en_pop;
	sc_in<sc_lv<11> > tag_in;
	sc_in<sc_lv<3> > link_in;
	sc_in<bool> is_pim_instruction_in;

	sc_out<sc_lv<11> > tag_out;
	sc_out<sc_lv<3> > link_out;
	sc_out<bool> is_pim_instruction_out;

	SC_CTOR(response_buffer){
		SC_METHOD(prc_response_buffer_write);
		sensitive << clk_vault.pos();
		SC_METHOD(prc_response_buffer_read);
		sensitive << clk_vault.pos();

		vault_id = 0;
	}

	int vault_id;

private:
	std::queue<sc_lv<11> > tag; // each read from memory is 32 bytes (256 bits) long
	std::queue<bool> is_pim_instruction;
	std::queue<sc_lv<3> > link;

	void prc_response_buffer_write(){
		if(en_push.read()){
			tag.push(tag_in.read());
			is_pim_instruction.push(is_pim_instruction_in.read());
			link.push(link_in.read());
		}
	}

	void prc_response_buffer_read(){
		if(!tag.empty()){
			tag_out.write(tag.front());
			is_pim_instruction_out.write(is_pim_instruction.front());
			link_out.write(link.front());
			if(en_pop.read()){
				tag.pop();
				is_pim_instruction.pop();
				link.pop();
			}
		}
		else{
			is_pim_instruction_out.write(false);
		}
	}
};
