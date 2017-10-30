#ifndef LINK_MASTER_H
#define LINK_MASTER_H

#include "systemc.h"
#include <queue>
#include "../../../../common/defines.h"
#include "../../../../traffic_monitor.h"

/*
 * This module will receive a request from the LLC and generate
 * an appropriate HMC packet.
 *
 * From testbench: address_in, cmd_in, data_in, cub_in
 * From link_coontrol: en_pop_tag_buffer_per_link
 * From reorder-responser: en_pop_tag_buffer
 *
 * To physical_tx: flit_out, flit_available
 * To link_control: request_tag_per_link
 * To reorder_responser: request_tag_per_link
 */
SC_MODULE(link_master_requester){
public:
	sc_in_clk  clk;
	sc_in<sc_lv<ADDRESS_WIDTH> > address_in; // UP to 16GB MEMORY
    sc_in<sc_lv<CMD_WIDTH> > cmd_in; //input command
    sc_in<sc_lv<FLIT_SIZE> > data_in[MAX_DATA_FLITS]; //MIN of 1 data FLIT up to 16 data FLIT
    sc_in<sc_lv<NUMBER_OF_CUBES> > cub_in;

    sc_in<bool> en_pop_tag_buffer;
    sc_in<bool> en_pop_tag_buffer_per_link[NUMBER_OF_LINKS];


    sc_out<sc_lv<FLIT_SIZE> > flit_out[NUMBER_OF_LINKS];
    sc_out<bool> flit_available[NUMBER_OF_LINKS];

    sc_out<sc_lv<TAG_SIZE> > request_global_tag;
    sc_out<sc_lv<TAG_SIZE> > request_tag_per_link[NUMBER_OF_LINKS];

    SC_CTOR(link_master_requester){
        SC_METHOD(prc_flit_generator);
        sensitive << address_in << cmd_in << cub_in;
        SC_METHOD(prc_send_flit_to_physical_tx);
        sensitive << clk.pos();
        SC_METHOD(prc_check_tag_ordering);
        sensitive << clk.pos();
        SC_METHOD(prc_check_stop_simulation);
        sensitive << clk.pos();

        tag = 0;
        round_robin = 0;
        traffic = NULL;

        reading_bytes = 0;
        writing_bytes = 0;
        vault_to_send = 0;
        first_flit_already_received = false;
        response_queue_status = false;
    }
private:
    void prc_flit_generator();
    void prc_send_flit_to_physical_tx();
    void prc_check_tag_ordering();
    void prc_check_stop_simulation();

    sc_lv<HEADER_WIDTH> create_header();
    sc_lv<TAIL_WIDTH> create_tail();
    sc_lv<CMD_WIDTH> decode_cmd();
    sc_lv<TAG_SIZE> generate_tag();
    sc_lv<MUX_SELECT_WIDTH> link_to_send();
    std::queue<sc_lv<FLIT_SIZE> > buffer[NUMBER_OF_LINKS];

    sc_lv<TAG_SIZE> tag;
    std::queue<sc_lv<TAG_SIZE> > tag_buffer;
    std::queue<sc_lv<TAG_SIZE> > tag_buffer_per_link[NUMBER_OF_LINKS];

    unsigned round_robin;
    bool first_flit_already_received;
    unsigned vault_to_send;
    int reading_bytes;
    int writing_bytes;
    bool response_queue_status; //true - empty false - not empty
    traffic_monitor *traffic;
public:
    void set_traffic_monitor(traffic_monitor *t){
    	traffic = t;
    }

    bool response_queue_is_empty(){
    	return response_queue_status;
    }
};

#endif

