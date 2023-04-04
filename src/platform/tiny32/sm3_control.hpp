#pragma once
#include "systemc.h"
#define idle  0b00                // idle state, no operation
#define write_  0b01	           // write register 
#define encryption  0b10		   // encryption only
#define read_  0b11		    	   // read register

SC_MODULE(SM3_Controller)
{
	sc_in_clk clk;
	sc_in<bool> rst, W, R;
	sc_out<bool> finish;

	sc_out<sc_uint<7> > count_out;    // calculat the times of extend&compress&writeresult 
	sc_out<sc_uint<2> > ctrl;

	sc_signal<sc_uint<7> > count;
	sc_signal<sc_uint<2> > next_state, current_state;
	sc_signal<sc_uint<7> > count_plus;

	void assign_count_plus();
	void assign_ctrl();

	void alwaysBlock1(); //current_state reg
	void alwaysBlock2(); //next_state logic

	//output logic
	// at each state , we can output signals
	void alwaysBlock3();


	SC_CTOR(SM3_Controller)
	{
		SC_METHOD(assign_count_plus);
		sensitive << count;
		SC_METHOD(assign_ctrl);
		sensitive << next_state;

		SC_METHOD(alwaysBlock1);
		sensitive << clk.pos()<< rst.neg();
		SC_METHOD(alwaysBlock2);
		sensitive << clk.pos()<< rst.neg();
		SC_METHOD(alwaysBlock3);
		sensitive << clk.pos()<< rst.neg();
	}
	
};