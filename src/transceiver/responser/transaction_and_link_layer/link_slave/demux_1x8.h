#ifndef DEMUX_1x8_H
#define DEMUX_1x8_H

#include "systemc.h"
#include "../../../../common/defines.h"

SC_MODULE(demux_1x8){
public:
    sc_in<sc_lv<FLIT_SIZE> > data_in;
    sc_in<sc_lv<3> > select;
    sc_out<sc_lv<FLIT_SIZE> > data_out[VAULTS_PER_QUADRANT];

    SC_CTOR(demux_1x8){
        SC_METHOD(prc_demux_1x8);
        sensitive << data_in << select;
    }
private:
    void prc_demux_1x8();
};

#endif
