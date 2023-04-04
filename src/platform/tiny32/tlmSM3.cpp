#include "tlmSM3.hpp"

tlmSM3::tlmSM3(sc_module_name  module_name)
	: sc_module(module_name)
	, clk("clk", CYCLE, SC_NS)
	, t_SM3("t_SM3")
	, bport1("sm3_bport1", 0x50002fff, CYCLE)
{
	SC_THREAD(ahb_init);
	sensitive << clk.posedge_event();
	t_SM3.HRESETn(rst_n), t_SM3.HCLK(clk), t_SM3.HWRITE(HWRITE);
	t_SM3.HSEL(HSEL), t_SM3.HMASTLOOK(HMASTLOOK), t_SM3.HTRANS(HTRANS);
	t_SM3.HSIZE(HSIZE), t_SM3.HBURST(HBURST), t_SM3.HPROT(HPROT);
	t_SM3.HWDATA(HWDATA), t_SM3.HADDR(HADDR), t_SM3.HREADY(HREADY);
	t_SM3.HRESP(HRESP), t_SM3.HRDATA(HRDATA);
	
	// bus slave port for AHB transfer
	bport1.HRESETn(rst_n), bport1.HCLK(clk), bport1.HWRITE(HWRITE);
	bport1.HSEL(HSEL), bport1.HMASTLOOK(HMASTLOOK), bport1.HTRANS(HTRANS);
	bport1.HSIZE(HSIZE), bport1.HBURST(HBURST), bport1.HPROT(HPROT);
	bport1.HWDATA(HWDATA), bport1.HADDR(HADDR), bport1.HREADY(HREADY);
	bport1.HRESP(HRESP), bport1.HRDATA(HRDATA);
}

void tlmSM3::ahb_init()
{
	wait(clk.posedge_event());
	rst_n = 0;
	HMASTLOOK = 1;
	HBURST = 0b000;
	HPROT = 0b0000;
	wait(10 * CYCLE, SC_NS);
	rst_n = 1;
	wait(CYCLE * 2, SC_NS);
}