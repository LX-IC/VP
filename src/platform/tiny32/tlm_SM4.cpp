/*
 * tlm_SM4.cpp
 *
 *  Created on: Aug 31, 2021
 *      Author: Junwei W
 *	Discrption:
 *	   Verison:
 *
 */

#include "tlm_SM4.h"

// tlmSM4::tlmSM4(tlmModule &parent, sc_module_name module_name, Addr high_address)
// 	:sc_module(module_name),
// 	 clk("clk",CYCLE,SC_NS),
// 	 top_SM4("top_SM4"),
// 	 bport_SM4("SM4_bport1",high_address,CYCLE)
// {
// 	SC_THREAD(ahb_SM4_init);
// 	sensitive << clk.posedge_event();

// 	top_SM4.HRESETn(rst_n);
// 	top_SM4.HCLK(clk);
// 	top_SM4.HWRITE(HWRITE);
// 	top_SM4.HSEL(HSEL);
// 	top_SM4.HMASTLOOK(HMASTLOOK);
// 	top_SM4.HTRANS(HTRANS);
// 	top_SM4.HSIZE(HSIZE);
// 	top_SM4.HBURST(HBURST);
// 	top_SM4.HPROT(HPROT);
// 	top_SM4.HWDATA(HWDATA);
// 	top_SM4.HADDR(HADDR);
// 	top_SM4.HREADY(HREADY);
// 	top_SM4.HRESP(HRESP);
// 	top_SM4.HRDATA(HRDATA);

// 	bport_SM4.HRESETn(rst_n);
// 	bport_SM4.HCLK(clk);
// 	bport_SM4.HWRITE(HWRITE);
// 	bport_SM4.HSEL(HSEL);
// 	bport_SM4.HMASTLOOK(HMASTLOOK);
// 	bport_SM4.HTRANS(HTRANS);
// 	bport_SM4.HSIZE(HSIZE);
// 	bport_SM4.HBURST(HBURST);
// 	bport_SM4.HPROT(HPROT);
// 	bport_SM4.HWDATA(HWDATA);
// 	bport_SM4.HADDR(HADDR);
// 	bport_SM4.HREADY(HREADY);
// 	bport_SM4.HRESP(HRESP);
// 	bport_SM4.HRDATA(HRDATA);
// }

tlmSM4::tlmSM4(sc_module_name module_name)
	:sc_module(module_name),
	 clk("clk",CYCLE,SC_NS),
	 top_SM4("top_SM4"),
	 bport_SM4("SM4_bport1",0x50003fff,CYCLE)
{
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif
	
	SC_THREAD(ahb_SM4_init);
	sensitive << clk.posedge_event();

	top_SM4.HRESETn(rst_n);
	top_SM4.HCLK(clk);
	top_SM4.HWRITE(HWRITE);
	top_SM4.HSEL(HSEL);
	top_SM4.HMASTLOOK(HMASTLOOK);
	top_SM4.HTRANS(HTRANS);
	top_SM4.HSIZE(HSIZE);
	top_SM4.HBURST(HBURST);
	top_SM4.HPROT(HPROT);
	top_SM4.HWDATA(HWDATA);
	top_SM4.HADDR(HADDR);
	top_SM4.HREADY(HREADY);
	top_SM4.HRESP(HRESP);
	top_SM4.HRDATA(HRDATA);

	bport_SM4.HRESETn(rst_n);
	bport_SM4.HCLK(clk);
	bport_SM4.HWRITE(HWRITE);
	bport_SM4.HSEL(HSEL);
	bport_SM4.HMASTLOOK(HMASTLOOK);
	bport_SM4.HTRANS(HTRANS);
	bport_SM4.HSIZE(HSIZE);
	bport_SM4.HBURST(HBURST);
	bport_SM4.HPROT(HPROT);
	bport_SM4.HWDATA(HWDATA);
	bport_SM4.HADDR(HADDR);
	bport_SM4.HREADY(HREADY);
	bport_SM4.HRESP(HRESP);
	bport_SM4.HRDATA(HRDATA);
}

void tlmSM4::ahb_SM4_init()
{
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif
	
	wait(clk.posedge_event());
	rst_n = 0;
	HMASTLOOK = 1;
	HBURST = 0b000;
	HPROT = 0b0000;
	wait(10 * CYCLE, SC_NS);
	rst_n = 1;
	wait(CYCLE * 2, SC_NS);
}
