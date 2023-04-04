#pragma once
#include "top_SM3.hpp"
#include "tlm_utils/simple_target_socket.h"
#include "tlmAHBSocket.hpp"

#define CYCLE 10
typedef uint64_t Addr;

using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace sc_dt;


class tlmSM3: public sc_module
{
  public:
    SC_HAS_PROCESS(tlmSM3);
    sc_signal<bool> rst_n;
    sc_clock clk;
    sc_signal<bool> HWRITE;
    sc_signal<bool> HSEL;
    sc_signal<bool> HMASTLOOK;
    sc_signal<sc_uint<3> > HSIZE, HBURST;
    sc_signal<sc_uint<2> > HTRANS;
    sc_signal<sc_uint<32> > HADDR;
    sc_signal<sc_uint<32> > HWDATA;
    sc_signal<sc_uint<4> > HPROT;
    sc_signal<bool> HREADY;
    sc_signal<bool> HRESP;
    sc_signal<sc_uint<32> > HRDATA;
    top_SM3 t_SM3;
    
    AHBSocket bport1;
    
    void ahb_init();
    // tlmSM3(tlmModule  &parent,  sc_module_name  module_name,  Addr high_address);
    tlmSM3(sc_module_name  module_name);
};