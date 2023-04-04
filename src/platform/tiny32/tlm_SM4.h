/*
 * tlm_SM4.h
 *
 *  Created on: Aug 31, 2021
 *      Author: Junwei W
 *	Discrption:
 *	   Verison:
 *
 */

#ifndef TLM_SM4_H_
#define TLM_SM4_H_

#include "ahb_sm4.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlmAHBSocket.hpp"
#include "complie_conf.h"

#define CYCLE 10

typedef uint64_t Addr;

using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace sc_dt;


class tlmSM4 : public sc_module
{
public:
	SC_HAS_PROCESS(tlmSM4);
	//AHB IF signal
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

    //SM4 inst
    ahb_sm4 top_SM4;

    //AHBSocket inst
    AHBSocket bport_SM4;

    void ahb_SM4_init();//SM4 reset

    // tlmSM4(tlmModule &parent, sc_module_name module_name, Addr high_address); //OVP

    tlmSM4(sc_module_name module_name);
};


#endif /* TLM_SM4_H_ */
