/*
 * ahbreg_sm4.h
 *
 *  Created on: Jun 26, 2021
 *      Author: junwei wu
 *    Function:	add the external access register interface for the SM4 module
 */

#ifndef AHBREG_SM4_H
#define AHBREG_SM4_H

#include "systemc.h"
#include "sm4.h"
#include "complie_conf.h"

SC_MODULE(ahbreg_sm4)
{
	sc_in_clk clk;
	sc_in<bool> rst_n;
	sc_in<sc_uint<32> > data_opt;
	sc_in<sc_biguint<128> > data_in;
	sc_out<sc_biguint<128> > data_out;
	sc_out<sc_uint<32> > data_stu;

	sm4 core_sm4;


	sc_signal<bool> sm4_rst_n;
	sc_signal<bool> key_extension;
	sc_signal<bool> mode;
	sc_signal<bool> din_valid;
	sc_signal<bool> dout_valid;
	sc_signal<bool> key_ext_finish;
	sc_signal<sc_biguint<128> > key;
	sc_signal<sc_biguint<128> > din;
	sc_signal<sc_biguint<128> > data_in_reg;
	sc_signal<bool> cmd_rst;
	sc_signal<bool> cmd_data_update;
	sc_signal<bool> cmd_key_ext;
	sc_signal<bool> cmd_enc;
	sc_signal<bool> cmd_dec;
	sc_signal<bool> cmd_key_ext_reg;
	sc_signal<bool> cmd_enc_reg;
	sc_signal<bool> cmd_dec_reg;
//	sc_signal<bool> data_com_flag;

	// sc_trace_file* trace_file;

	void control_proc();
	void cmd_delay();
	void reset_proc();
	void data_update_proc();
	void key_ext_proc();
	void enc_proc();
	void dec_proc();
	void default_proc();
	void status_reg_proc();
//	void data_complete_proc();

	SC_CTOR(ahbreg_sm4)
	: clk("clk"),
	  rst_n("rst_n"),
	  data_opt("data_opt"),
	  data_in("data_in"),
	  data_out("data_out"),
	  data_stu("data_stu"),
	  core_sm4("CORE_SM4"),
	  sm4_rst_n("sm4_rst_n")


	{
		core_sm4.clk(clk),
		core_sm4.rst_n(sm4_rst_n),
		core_sm4.key_extension_i(key_extension),
		core_sm4.key_i(key),
		core_sm4.key_ext_finish(key_ext_finish),
		core_sm4.din_valid(din_valid),
		core_sm4.mode_i(mode),
		core_sm4.data_i(din),
		core_sm4.dout_valid(dout_valid),
		core_sm4.data_o(data_out);


//		SC_CTHREAD(reg_wrap,clk.pos());
//		reset_signal_is(rst_n,false);

		SC_CTHREAD(control_proc,clk.pos());
		reset_signal_is(rst_n,false);

		SC_CTHREAD(cmd_delay,clk.pos());
		reset_signal_is(rst_n,false);

		SC_CTHREAD(reset_proc,clk.pos());
		reset_signal_is(rst_n,false);

		SC_CTHREAD(data_update_proc,clk.pos());
		reset_signal_is(rst_n,false);

		SC_CTHREAD(key_ext_proc,clk.pos());
		reset_signal_is(rst_n,false);

		SC_CTHREAD(enc_proc,clk.pos());
		reset_signal_is(rst_n,false);

		SC_CTHREAD(dec_proc,clk.pos());
		reset_signal_is(rst_n,false);

		SC_CTHREAD(default_proc,clk.pos());
		reset_signal_is(rst_n,false);

		SC_METHOD(status_reg_proc);
		sensitive << key_ext_finish << dout_valid << din_valid << cmd_enc << cmd_dec << sm4_rst_n;


//		SC_CTHREAD(data_complete_proc,clk.pos());
//		reset_signal_is(rst_n,false);

		// trace_file = sc_create_vcd_trace_file("ahbreg_sm4_wave");
		// //IO
		// sc_trace(trace_file,clk,"clk");
		// sc_trace(trace_file,rst_n,"rst_n");
		// sc_trace(trace_file,data_opt,"data_opt");
		// sc_trace(trace_file,data_in,"data_in");
		// sc_trace(trace_file,data_out,"data_out");
		// sc_trace(trace_file,data_stu,"data_stu");
		// //from SM4 core
		// sc_trace(trace_file,sm4_rst_n,"sm4_rst_n");
		// sc_trace(trace_file,key_extension,"key_extension");
		// sc_trace(trace_file,key,"key");
		// sc_trace(trace_file,key_ext_finish,"key_ext_finish");
		// sc_trace(trace_file,din_valid,"din_valid");
		// sc_trace(trace_file,mode,"mode");
		// sc_trace(trace_file,din,"din");
		// sc_trace(trace_file,dout_valid,"dout_valid");
		// sc_trace(trace_file,data_out,"data_out");
		// //cmd signal
		// sc_trace(trace_file,cmd_rst,"cmd_rst");
		// sc_trace(trace_file,cmd_data_update,"cmd_data_update");
		// sc_trace(trace_file,cmd_key_ext,"cmd_key_ext");
		// sc_trace(trace_file,cmd_enc,"cmd_enc");
		// sc_trace(trace_file,cmd_dec,"cmd_dec");
		// sc_trace(trace_file,cmd_key_ext_reg,"cmd_key_ext_reg");
		// sc_trace(trace_file,cmd_enc_reg,"cmd_enc_reg");
		// sc_trace(trace_file,cmd_dec_reg,"cmd_dec_reg");



	}

};


#endif


