/*
 * vault.h
 *
 *  Created on: 7 de nov de 2016
 *      Author: root
 */

#ifndef SRC_VAULT_VAULT_H_
#define SRC_VAULT_VAULT_H_

#include "vault_controller/vault_controller.h"
#include "bank/bank.h"

SC_MODULE(vault){
public:
	sc_in_clk clk_transeiver, clk_vault, clk_pim;
	sc_in<bool> start_decoding, reset, en_pop_response;
	sc_in<sc_lv<128> > request;

	sc_out<sc_lv<128> > response;
	sc_out<bool> response_available;

	int vault_id;

	SC_CTOR(vault){
		vault_id = 0;
		mod_vault_controller = new vault_controller(sc_gen_unique_name("vault_controller"));
		mod_vault_controller -> clk_transeiver(clk_transeiver);
		mod_vault_controller -> clk_vault(clk_vault);
		mod_vault_controller -> clk_pim(clk_pim);
		mod_vault_controller -> request_from_host_available(start_decoding);
		mod_vault_controller -> reset(reset);
		mod_vault_controller -> en_pop_response(en_pop_response);
		mod_vault_controller -> request_from_host(request);
		mod_vault_controller -> response(response);
		mod_vault_controller -> response_available(response_available);

		for(int  i = 0; i < FAW; i++){
			mod_vault_controller -> cs[i](cs_s[i]);
			mod_vault_controller -> ras[i](ras_s[i]);
			mod_vault_controller -> we[i](we_s[i]);
			mod_vault_controller -> oe[i](oe_s[i]);
			mod_vault_controller -> cas[i](cas_s[i]);
			mod_vault_controller -> done[i](done_s[i]);
			mod_vault_controller -> bank_id[i](bank_id_s[i]);
			mod_vault_controller -> offset[i](offset_s[i]);
			mod_vault_controller -> address[i](address_s[i]);
		}

		for(int i = 0; i < NUMBER_OF_BANKS; i++){
			mod_vault_controller -> data_from_banks[i](data_from_banks_s[i]);
			mod_vault_controller -> data_available_to_read[i](data_available_to_read_s[i]);
			mod_vault_controller -> data_to_banks[i](data_to_banks_s[i]);
			mod_vault_controller -> write_available[i](write_available_s[i]);

		}

		for(int i = 0; i < NUMBER_OF_BANKS; i++){
			mod_bank[i] = new bank(sc_gen_unique_name("bank"));
			mod_bank[i] -> clk(clk_vault);
			for(int j = 0; j < FAW; j++){
				mod_bank[i] -> cs[j](cs_s[j]);
				mod_bank[i] -> ras[j](ras_s[j]);
				mod_bank[i] -> cas[j](cas_s[j]);
				mod_bank[i] -> oe[j](oe_s[j]);
				mod_bank[i] -> we[j](we_s[j]);
				mod_bank[i] -> done[j](done_s[j]);
				mod_bank[i] -> request_bank_id[j](bank_id_s[j]);
				mod_bank[i] -> address[j](address_s[j]);
				mod_bank[i] -> offset_in[j](offset_s[j]);
			}
			mod_bank[i] -> data_available_to_read(data_available_to_read_s[i]);
			mod_bank[i] -> data_in(data_to_banks_s[i]);
			mod_bank[i] -> data_out(data_from_banks_s[i]);
			mod_bank[i] -> data_available_to_write(write_available_s[i]);
		}
	}

public:
	vault_controller *mod_vault_controller;
	bank *mod_bank[NUMBER_OF_BANKS];

	sc_signal<sc_lv<256> > data_from_banks_s[NUMBER_OF_BANKS];
	sc_signal<bool> data_available_to_read_s[NUMBER_OF_BANKS];

	sc_signal<bool> cs_s[FAW], ras_s[FAW], cas_s[FAW], we_s[FAW], oe_s[FAW], done_s[FAW], write_available_s[NUMBER_OF_BANKS];
	sc_signal<sc_lv<LOG_NUMBER_OF_BANKS> > bank_id_s[FAW];
	sc_signal<sc_lv<4> > offset_s[FAW];
	sc_signal<sc_lv<20> > address_s[FAW];
	sc_signal<sc_lv<256> > data_to_banks_s[NUMBER_OF_BANKS];


};

#endif /* SRC_VAULT_VAULT_H_ */
