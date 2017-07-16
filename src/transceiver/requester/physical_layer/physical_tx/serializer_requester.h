#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "systemc.h"
#include <queue>
#include "../../../../common/defines.h"

SC_MODULE(serializer_requester){
public:
	sc_in_clk clk;
	sc_in<sc_lv<FLIT_SIZE> > flit;
	sc_in<bool> flit_available;

    sc_out<sc_lv<SERIALIZED_PACKET_WIDTH> > serialized_flit;
    sc_out<bool> serialized_flit_available;

    SC_CTOR(serializer_requester){
        SC_METHOD(prc_serialize);
        sensitive << clk.pos();
        SC_METHOD(prc_receive_flits);
        sensitive<< clk.pos();

        counter_actual = 0;
        flits_until_tail = 0;
        current_link = 0;
    }

private:
    void prc_serialize();
    void prc_receive_flits();

    std::queue<sc_lv<FLIT_SIZE> > buffer;
    int counter_actual;
    unsigned flits_until_tail;
    unsigned current_link;
};

#endif
