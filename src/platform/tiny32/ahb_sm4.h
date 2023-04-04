/*
 * ahb_sm4.h
 *
 *  Created on: Jul 13, 2021
 *      Author: Junwei W
 *	Discrption: AHB Warper for SM4 core
 *	   Verison:
 *
 */
/*//////////////////////////////////////////////
 * AHB-Lite SM4 register address:
 *
 * addr_x_in_0	= 32'h50003000,
 * addr_x_in_1  = 32'h50003004,
 * addr_x_in_2  = 32'h50003008,
 * addr_x_in_3  = 32'h5000300c,
 *
 * addr_y_out_0 = 32'h50003010,
 * addr_y_out_1 = 32'h50003014,
 * addr_y_out_2 = 32'h50003018,
 * addr_y_out_3 = 32'h5000301c
 *
 * addr_control = 32'h50003020,
 * addr_status	= 32'h50003024
 *
 * *///////////////////////////////////////////


#ifndef AHB_SM4_H
#define AHB_SM4_H

#include "systemc.h"
#include <stdio.h>
#include "ahbreg_sm4.h"
#include "complie_conf.h"

//#define addr_x_in 		0b00000000;
//#define addr_y_out 		0b00000001;
//#define addr_control  	0x020;
//#define addr_status  		0x024;

SC_MODULE(ahb_sm4)
{
	sc_in_clk 				HCLK;
	sc_in	<bool> 			HRESETn;
	sc_in	<bool> 			HWRITE,HSEL,HMASTLOOK;
	sc_in	<sc_uint<2> > 	HTRANS;
	sc_in	<sc_uint<3> > 	HSIZE,HBURST;
	sc_in	<sc_uint<32> > 	HADDR,HWDATA;
	sc_in	<sc_uint<4> > 	HPROT;
	sc_out	<bool> 			HREADY,HRESP;
	sc_out	<sc_uint<32> > 	HRDATA;

	ahbreg_sm4 sm4;

	sc_signal<sc_uint<32> > control_reg;
	sc_signal<sc_uint<32> > x_in[4];
	sc_signal<sc_uint<32> > y_out[4];
	sc_signal<sc_biguint<128> > din;
	sc_signal<sc_biguint<128> > dout;
	sc_signal<sc_uint<32> > status_reg;

	sc_signal<bool> lite_write_req;
//	sc_signal<bool> lite_read_req;
	sc_signal<bool> ahb_write_en;
	sc_signal<bool> ahb_read_en;
	sc_signal<bool> ahb_trans_valid;
	sc_signal<sc_uint<12> > sm4_addr;
	sc_signal<sc_uint<3> > HSIZE_reg;
	sc_signal<sc_uint<32> > rdata;

	// sc_trace_file* trace_file;

	void assign_output();
	void assign_din();
	void assign_hrdata();
	void assign_ahb();
	void assign_ahb_write_en();
	void assign_ahb_read_en();
	void assign_ahb_trans_valid();
	void write_enable();
	void read_enable();
	void ahb_addr();
	void control_reg_input();
	void data_input();
	void data_output();

	SC_CTOR(ahb_sm4)
	:HCLK("HCLK"),
	 HRESETn("HRESETn"),
	 HWRITE("HWRITE"),
	 HSEL("HSEL"),
	 HMASTLOOK("HMASTLOOK"),
	 HTRANS("HTRANS"),
	 HSIZE("HSIZE"),
	 HBURST("HBURST"),
	 HADDR("HADDR"),
	 HWDATA("HWDATA"),
	 HPROT("HPROT"),
	 HREADY("HREADY"),
	 HRESP("HRESP"),
	 HRDATA("HRDATA"),
	 sm4("SM4")
	{
		sm4.clk(HCLK);
		sm4.rst_n(HRESETn);
		sm4.data_opt(control_reg);
		sm4.data_in(din);
		sm4.data_out(dout);
		sm4.data_stu(status_reg);

		SC_METHOD(assign_output);
		sensitive << dout;

		SC_METHOD(assign_din);
		sensitive << x_in[0] <<x_in[1] << x_in[2] << x_in[3];

		SC_METHOD(assign_ahb);

		SC_METHOD(assign_hrdata);
		sensitive << rdata;

		SC_METHOD(assign_ahb_write_en);
		sensitive << HWRITE << ahb_trans_valid;

		SC_METHOD(assign_ahb_read_en);
		sensitive << HWRITE << ahb_trans_valid;

		SC_METHOD(assign_ahb_trans_valid);
		sensitive << HSEL << HREADY << HTRANS;

		SC_METHOD(write_enable);
		sensitive << HCLK.pos();
		sensitive << HRESETn.neg();

//		SC_METHOD(read_enable);
//		sensitive << HCLK.pos();
//		sensitive << HRESETn.neg();

		SC_METHOD(ahb_addr);
		sensitive << HCLK.pos();
		sensitive << HRESETn.neg();

		SC_METHOD(control_reg_input);
		sensitive << HCLK.pos();
		sensitive << HRESETn.neg();

		SC_METHOD(data_input);
		sensitive << HCLK.pos();
		sensitive << HRESETn.neg();

		SC_METHOD(data_output);
		sensitive << HCLK.pos();
		sensitive << HRESETn.neg();

// 		//for debug
// 		trace_file = sc_create_vcd_trace_file("ahb_sm4_wave");
// 		//IO
// 		sc_trace(trace_file,HCLK,"HCLK");
// 		sc_trace(trace_file,HRESETn,"HRESETn");
// 		sc_trace(trace_file,HWRITE,"HWRITE");
// 		sc_trace(trace_file,HSEL,"HSEL");
// 		sc_trace(trace_file,HMASTLOOK,"HMASTLOOK");
// 		sc_trace(trace_file,HTRANS,"HTRANS");
// 		sc_trace(trace_file,HSIZE,"HSIZE");
// 		sc_trace(trace_file,HBURST,"HBURST");
// 		sc_trace(trace_file,HADDR,"HADDR");
// 		sc_trace(trace_file,HWDATA,"HWDATA");
// 		sc_trace(trace_file,HPROT,"HPROT");
// 		sc_trace(trace_file,HREADY,"HREADY");
// 		sc_trace(trace_file,HRESP,"HRESP");
// 		sc_trace(trace_file,HRDATA,"HRDATA");
// 		//from SM4
// 		sc_trace(trace_file,control_reg,"control_reg");
// 		sc_trace(trace_file,din,"din");
// 		sc_trace(trace_file,dout,"dout");
// 		sc_trace(trace_file,status_reg,"status_reg");
// 		char name[10];
// 		for(int i = 0; i < 4 ; i++)
// 		{
// 			sprintf(name, "x_in[%d]",i);
// 			sc_trace(trace_file,x_in[i],name);
// 		}
// 		for(int i = 0; i < 4 ; i++)
// 		{
// 			sprintf(name, "y_out[%d]",i);
// 			sc_trace(trace_file,y_out[i],name);
// 		}
// 		sc_trace(trace_file,lite_write_req,"lite_write_req");
// //		sc_trace(trace_file,lite_read_req,"lite_read_req");
// 		sc_trace(trace_file,ahb_write_en,"ahb_write_en");
// 		sc_trace(trace_file,ahb_read_en,"ahb_read_en");
// 		sc_trace(trace_file,ahb_trans_valid,"ahb_trans_valid");
// 		sc_trace(trace_file,sm4_addr,"sm4_addr");
// 		sc_trace(trace_file,HSIZE_reg,"HSIZE_reg");


	}
};

#endif


