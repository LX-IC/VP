#pragma once
#include "tlm.h"                                // TLM headers
#include "tlm_utils/simple_target_socket.h"
#include <iostream>
#include <string.h>
#include "systemc.h"
#include "complie_conf.h"


typedef uint64_t Addr;
using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace sc_dt;


class AHBSocket:  public sc_module  {
  public:
    sc_in_clk HCLK;
    sc_out<bool> HRESETn;
    sc_out<bool> HWRITE;
    sc_out<bool> HSEL;
    sc_out<bool> HMASTLOOK;
    sc_out<sc_uint<3> > HSIZE, HBURST;
    sc_out<sc_uint<2> > HTRANS;
    sc_out<sc_uint<32> > HADDR;
    sc_out<sc_uint<32> > HWDATA;
    sc_out<sc_uint<4> > HPROT;
    sc_in<bool> HREADY;
    sc_in<bool> HRESP;
    sc_in<sc_uint<32> > HRDATA;

    void  custom_b_transport(tlm_generic_payload &payload, sc_time &delay_time);
    bool checkAddressRange ( Addr address, Addr bytes);
    bool checkPayloadAddress (tlm_generic_payload &gp);
    unsigned int debug_transport (tlm_generic_payload &payload);

    void ahb_write(uint32_t addr, uint32_t data);
    void ahb_read(uint32_t addr, uint32_t* data);
    AHBSocket (sc_module_name  module_name,  Addr max_address, uint32_t cycle);

  public:
    tlm_utils::simple_target_socket<AHBSocket>  socket;
  
  private:
    uint32_t maxAddress;
    uint32_t CYCLE;
};
