/*
 * traffic_monitor.h
 *
 *  Created on: 20 de fev de 2017
 *      Author: root
 */

#ifndef SRC_TRAFFIC_MONITOR_H_
#define SRC_TRAFFIC_MONITOR_H_

#include "systemc.h"
#include <string>
#include <unordered_map>
#include "common/defines.h"

struct link_monitor{
	float t_rx_final;
	float t_tx_initial;
	int read_bytes;
	int write_bytes;
};

struct vault_monitor{
	float t_final;
	float t_initial;
	int read_bytes;
	int write_bytes;
};


class traffic_monitor{
public:
	traffic_monitor();

	link_monitor l_monitor[NUMBER_OF_LINKS];
	vault_monitor v_monitor[NUMBER_OF_VAULTS];

	float t_hmc_initial;
	float t_hmc_final;

	void get_statistics();
	bool hmc_still_busy;
	bool pim_still_busy[NUMBER_OF_VAULTS];
};

#endif /* SRC_TRAFFIC_MONITOR_H_ */
