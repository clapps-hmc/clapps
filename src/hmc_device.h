/*
 * hmc_device.h
 *
 *  Created on: 10 de nov de 2016
 *      Author: root
 */

#ifndef SRC_HMC_DEVICE_H_
#define SRC_HMC_DEVICE_H_

#include "vault/vault.h"
#include "transceiver/transceiver.h"
#include "traffic_monitor.h"


SC_MODULE(hmc_device){
public:
	sc_in_clk clk_transeiver, clk_vault, clk_pim;
    sc_in<bool> reset;
	sc_in<sc_lv<34> > address_in; // UP to 16GB MEMORY
    sc_in<sc_lv<7> > cmd_in; //input command
    sc_in<sc_lv<FLIT_SIZE> > data_in[16]; //MIN of 1 data FLIT up to 16 data FLIT
    sc_in<sc_lv<3> > cub_in;
	sc_out<sc_lv<FLIT_SIZE> > flit_out;


	SC_CTOR(hmc_device){
		for(int i = 0; i < NUMBER_OF_VAULTS; i++){
			mod_vault[i] = new vault(sc_gen_unique_name("vault"));
			mod_vault[i] -> vault_id = i;
			mod_vault[i] -> mod_vault_controller -> mod_request_decoder -> request_decoder_id = i;
			mod_vault[i] -> clk_transeiver(clk_transeiver);
			mod_vault[i] -> clk_vault(clk_vault);
			mod_vault[i] -> clk_pim(clk_pim);
			mod_vault[i] -> start_decoding(packet_avaliable_s[i]);
			mod_vault[i] -> reset(reset);
			mod_vault[i] -> en_pop_response(en_pop_response_s[i]);
			mod_vault[i] -> request(data_to_vauls_s[i]);
			mod_vault[i] -> response(packets_from_vaults_s[i]);
			mod_vault[i] -> response_available(response_tag_status_s[i]);
			mod_vault[i] -> mod_vault_controller -> mod_tsv_controller -> vault_id = i;
			mod_vault[i] -> mod_vault_controller -> mod_response_control -> id = i;
			mod_vault[i] -> mod_vault_controller -> mod_response_buffer -> vault_id = i;
			mod_vault[i] -> mod_vault_controller -> mod_request_decoder -> id = i;
			mod_vault[i] -> mod_vault_controller -> mod_rvu -> rvu_id = i;
			for(int j = 0; j < FAW; j++){
				mod_vault[i] -> mod_vault_controller -> mod_bank_request_buffer[j] -> vault_id = i;
				mod_vault[i] -> mod_vault_controller -> mod_bank_request_buffer[j] -> bank_request_id = j;
				mod_vault[i] ->  mod_vault_controller -> mod_memory_controller[j] -> vault_id = i;
				mod_vault[i] ->  mod_vault_controller -> mod_memory_controller[j] -> memory_controller_id = j;
				mod_vault[i] ->  mod_vault_controller -> mod_data_request_buffer[j] -> vault_id = i;
				mod_vault[i] ->  mod_vault_controller -> mod_data_response_buffer[j] -> vault_id = i;
				mod_vault[i] ->  mod_vault_controller -> mod_data_response_buffer[j] ->  fsm_id = j;
			}

			for(unsigned j = 0; j < NUMBER_OF_BANKS; j++){
				mod_vault[i] -> mod_bank[j] -> bank_id = j;
				mod_vault[i] -> mod_bank[j] -> vault_id = i;
			}
		}

		mod_transceiver = new transceiver("mod_transceiver");
		mod_transceiver -> clk(clk_transeiver);
		mod_transceiver -> address_in(address_in);
		mod_transceiver -> cmd_in(cmd_in);
		for(int i = 0; i < 16; i++)
			mod_transceiver -> data_in[i](data_in[i]);
		mod_transceiver -> cub_in(cub_in);
		for(int i = 0; i < NUMBER_OF_VAULTS; i++){
			mod_transceiver -> flit_available_from_vault[i](response_tag_status_s[i]);
			mod_transceiver -> flit_from_vault[i](packets_from_vaults_s[i]);
			mod_transceiver -> flit_to_vault[i](data_to_vauls_s[i]);
			mod_transceiver -> flit_available_to_vault[i](packet_avaliable_s[i]);
			mod_transceiver -> en_pop_response_from_vault[i](en_pop_response_s[i]);

		}

		mod_transceiver -> flit_out_to_host(flit_out);
		mod_transceiver -> mod_requester -> mod_transaction_and_link_layer  -> mod_link_master -> set_traffic_monitor(&traffic);
		mod_transceiver -> mod_requester -> mod_transaction_and_link_layer  -> mod_link_slave -> mod_mux_4x1_w_set_bit -> set_traffic_monitor(&traffic);
		for(int i = 0; i < NUMBER_OF_LINKS; i++){
			mod_transceiver -> mod_responser -> mod_physical_layer -> mod_physical_rx -> mod_deserializer[i]-> set_traffic_monitor(&traffic);
		 	mod_transceiver -> mod_responser -> mod_physical_layer -> mod_physical_tx -> mod_serializer[i] -> set_traffic_monitor(&traffic);
		 	mod_transceiver -> mod_requester -> mod_physical_layer -> mod_physical_rx -> mod_deserializer[i] -> set_traffic_monitor(&traffic);
		}
		for(int i = 0; i < NUMBER_OF_VAULTS; i++){
			mod_vault[i] -> mod_vault_controller -> mod_request_decoder -> set_traffic_monitor(&traffic);
			mod_vault[i] -> mod_vault_controller -> mod_response_control -> set_traffic_monitor(&traffic);
			mod_vault[i] -> mod_vault_controller -> mod_rvu -> set_traffic_monitor(&traffic);
		}

		SC_METHOD(checker);
		sensitive << clk_pim.pos() << clk_transeiver.pos();
	}
private:
	transceiver *mod_transceiver;
	vault *mod_vault[NUMBER_OF_VAULTS];

	sc_signal<bool> response_tag_status_s[NUMBER_OF_VAULTS];
	sc_signal<bool> packet_avaliable_s[NUMBER_OF_VAULTS];
	sc_signal<bool> en_pop_response_s[NUMBER_OF_VAULTS];
	sc_signal<sc_lv<FLIT_SIZE> > packets_from_vaults_s[NUMBER_OF_VAULTS];

	sc_signal<sc_lv<TAG_SIZE> > response_tag_s[NUMBER_OF_VAULTS];
	sc_signal<sc_lv<PACKET_LNG> > response_lng_s[NUMBER_OF_VAULTS];
    sc_signal<sc_lv<FLIT_SIZE> > data_to_vauls_s[NUMBER_OF_VAULTS];

    sc_signal<sc_lv<FLIT_SIZE> > pck_from_vault_tmp_s[NUMBER_OF_VAULTS];

    traffic_monitor traffic;
public:
    void get_statistics(){
    	traffic.get_statistics();
    }

    void checker(){
    	bool status_all_pims = false;
    	for(int i = 0; i < NUMBER_OF_VAULTS; i++)
    		status_all_pims |= traffic.pim_still_busy[i];
    	if(!traffic.hmc_still_busy && !status_all_pims)
    		sc_stop();
    }

    void initialize_banks(){

    }
};


#endif /* SRC_HMC_DEVICE_H_ */
