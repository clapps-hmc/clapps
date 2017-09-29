
#include "systemc.h"
#include "hmc_device.h"
#include "hmc_testbench.h"
#include "traffic_monitor.h"
#include "common/defines.h"
#include "transceiver/transceiver.h"

int sc_main(int argc, char* argv[]){

  sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING );
  sc_report_handler::set_actions (SC_WARNING, SC_DO_NOTHING);

   sc_signal<bool> reset;
   sc_signal<sc_lv<34> > address_in; // UP to 16GB MEMORY
   sc_signal<sc_lv<7> > cmd_in; //input command
   sc_signal<sc_lv<128> > data_in[16]; //MIN of 1 data FLIT up to 16 data FLIT
   sc_signal<sc_lv<3> > cub_in;

   sc_signal<sc_lv<128> > flit_out;
   sc_clock clk_transiever("clk_trans", TRANS_CLOCK_RATE, SC_NS);
   sc_clock clk_vault("clk_vault", VAULT_CLOCK_RATE, SC_NS);

   hmc_device hmc("hmc");
   hmc.clk_transeiver(clk_transiever);
   hmc.clk_vault(clk_vault);
   hmc.clk_pim(clk_vault);
   hmc.reset(reset);
   hmc.address_in(address_in);
   hmc.cmd_in(cmd_in);

   for(int i = 0; i < 16; i++)
	   hmc.data_in[i](data_in[i]);
   hmc.cub_in(cub_in);
   hmc.flit_out(flit_out);

       
   hmc_tb testbench("hmc_testbench");
   testbench.trace_name.clear();
   if(argc == 2){
        testbench.trace_name = std::string(argv[1]);
   }
   testbench.clk(clk_transiever);
   testbench.reset(reset);
   testbench.address_in_t(address_in);
   testbench.cmd_in_t(cmd_in);
   testbench.cub_in_t(cub_in);
	for(int i = 0; i < 16; i++){
		testbench.data_in_t[i](data_in[i]);
	}
	testbench.packet_out_t(flit_out);

    testbench.open_trace();
    sc_start();

    hmc.get_statistics();


    return (0);
}


