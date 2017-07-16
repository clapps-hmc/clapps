/*
 * traffic_monitor.cpp
 *
 *  Created on: 20 de fev de 2017
 *      Author: root
 */

#include "traffic_monitor.h"
#include <iostream>

using namespace std;
traffic_monitor::traffic_monitor(){
	for(int i = 0; i < NUMBER_OF_LINKS; i++){
		l_monitor[i].read_bytes = 0;
		l_monitor[i].write_bytes = 0;
		l_monitor[i].t_rx_final = 0;
		l_monitor[i].t_tx_initial = 0;
	}

	for(int i = 0; i < NUMBER_OF_VAULTS; i++){
		v_monitor[i].read_bytes = 0;
		v_monitor[i].write_bytes = 0;
		v_monitor[i].t_final = 0;
		v_monitor[i].t_initial = 0;
	}

	t_hmc_final = 0;
	t_hmc_initial = 0;

	hmc_still_busy = true;
	for(int i = 0; i < NUMBER_OF_VAULTS; i++)
		pim_still_busy[i] = true;
}



void traffic_monitor::get_statistics(){
	float avg_b = 0;
	unsigned total_bytes_v = 0;
	cout << " ------------------------------------------ \n";
	cout << "|           PER VAULT STATISTICS  		    |\n";
	cout << " ------------------------------------------ \n";

	for(int i = 0; i < NUMBER_OF_VAULTS; i++){
		cout << "Vault: " << i << endl;
		cout << "Total (Bytes) read: " << v_monitor[i].read_bytes << endl;
		cout << "Total (Bytes) write: " << v_monitor[i].write_bytes << endl;
		cout << "Total (ns) time: " << v_monitor[i].t_final - v_monitor[i].t_initial << endl;
		if(v_monitor[i].t_final - v_monitor[i].t_initial != 0){
			cout << "Total (GB/s) bandwidth: " << (v_monitor[i].read_bytes + v_monitor[i].write_bytes)/(v_monitor[i].t_final - v_monitor[i].t_initial) << endl;
			avg_b += (v_monitor[i].read_bytes + v_monitor[i].write_bytes)/(v_monitor[i].t_final - v_monitor[i].t_initial);
			total_bytes_v += (v_monitor[i].read_bytes + v_monitor[i].write_bytes);
		}
		else
			cout << "Total (GB/s) bandwidth: " << 0 << endl;

		cout << endl;
	}
	cout << " Total bandwidth: " << avg_b << endl;
	cout << " Average bandwidth: " << avg_b/NUMBER_OF_VAULTS << endl;
	cout << " Total bytes: " << total_bytes_v << endl;
	cout << endl;

	float avg_b_link = 0;
	int total_r = 0;
	int total_w = 0;
	cout << " ------------------------------------------ \n";
	cout << "|           PER LINK STATISTICS  		    |\n";
	cout << " ------------------------------------------ \n";
	for(int i = 0; i < NUMBER_OF_LINKS; i++){
		cout << "Link: " << i << endl;
		cout << "Total (Bytes) read: " << l_monitor[i].read_bytes << endl;
		total_r += l_monitor[i].read_bytes;
		total_w += l_monitor[i].write_bytes;
		cout << "Total (Bytes) write: " << l_monitor[i].write_bytes << endl;
		cout << "Total (ns) time: " << l_monitor[i].t_rx_final - l_monitor[i].t_tx_initial << endl;
		if(l_monitor[i].t_rx_final - l_monitor[i].t_tx_initial !=0){
			cout << "Total (GB/s) bandwidth: " << (l_monitor[i].read_bytes + l_monitor[i].write_bytes)/(l_monitor[i].t_rx_final - l_monitor[i].t_tx_initial) << endl;
			avg_b_link += (l_monitor[i].read_bytes + l_monitor[i].write_bytes)/(l_monitor[i].t_rx_final - l_monitor[i].t_tx_initial);
		}
		else{
			cout << "Total (GB/s) bandwidth: " << 0 << endl;
		}
		cout << endl;
	}
	cout << "Total (GB/s) bandwidth: " << avg_b_link << endl;
	cout << " Average bandwidth: " << avg_b_link/NUMBER_OF_LINKS << endl;

	cout << endl;

	cout << " ------------------------------------------ \n";
	cout << "|           GENERAL STATISTICS  		    |\n";
	cout << " ------------------------------------------ \n";

	cout << "Total (Bytes) read: "  << total_r <<  endl;
	cout << "Total (Bytes) write: " << total_w << endl;
	cout << "Total (ns) time: " << t_hmc_final - t_hmc_initial << endl;
	cout << "Total (GB/s) bandwidth: " << (total_r + total_w)/(t_hmc_final - t_hmc_initial)  << endl << endl;

}
