#include "systemc.h"
#include <queue>

SC_MODULE(data_response_buffer){
public:
	sc_in<bool> clk, en_push, en_pop;
	sc_in<sc_lv<256> > data_in;

	sc_out<sc_lv<256> > data_out;
	SC_CTOR(data_response_buffer){
		SC_METHOD(prc_data_response_buffer);
		sensitive_neg << clk;
		counter = 0;
		vault_id = 0;
		fsm_id = 0;
	}

	int vault_id;
	int fsm_id;
private:
	std::queue<sc_lv<256> > data; // each read from memory is 32 bytes (256 bits) long
	int counter;
	void prc_data_response_buffer(){
		if(en_push.read()){
			data.push(data_in.read());
			counter = 0;
		}


		if(!data.empty()){
			data_out.write(data.front());
			if(en_pop.read()){
				data.pop();
			}
		}
	}
};
