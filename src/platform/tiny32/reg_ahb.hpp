#pragma once
#include "systemc.h"
#include "messagecompress.hpp"
#include "sm3_control.hpp"

// The initial value 
#define IV_0 0x7380166f
#define IV_1 0x4914b2b9
#define IV_2 0x172442d7
#define IV_3 0xda8a0600
#define IV_4 0xa96f30bc
#define IV_5 0x163138aa
#define IV_6 0xe38dee4d
#define IV_7 0xb0fb0e4e

// The value for parameter Tj 
#define T0 0x79cc4519    //  0  <= j <= 15
#define T1 0x9d8a7a87    //  16 <= j <= 63

#define idle           0b00             // idle state, no operation
#define write_          0b01	            // write register 
#define encryption     0b10	            // encryption only
#define sta_finish     0b11		    	// read register

SC_MODULE(SM3_reg_ahb)
{
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in<sc_uint<32> > control;
    sc_out<sc_uint<32> > status;
    sc_in<sc_uint<32> >  msg_in1, msg_in2, msg_in3, msg_in4, msg_in5, msg_in6, msg_in7, msg_in8;
    sc_in<sc_uint<32> >  msg_in9, msg_in10, msg_in11, msg_in12, msg_in13, msg_in14, msg_in15, msg_in16;
    sc_out<sc_lv<32> > msg_out1, msg_out2, msg_out3, msg_out4, msg_out5, msg_out6, msg_out7, msg_out8;

    sc_signal<sc_uint<2> > state;
    sc_signal<sc_uint<32> > mes_in;
    sc_signal<bool> finish;
    sc_signal<bool> write_trig;            //Detect write signal
    sc_signal<bool> W;                     //SM3 write signal
    sc_signal<bool> R;                     //SM3 read signal

    sc_signal<sc_uint<7> > round;
    sc_signal_rv<256> wout;

    void assign_wout();
    void alwaysBlock1();
    void alwaysBlock2();
    void alwaysBlock3();

    SM3_Controller SC1;
    CompressExtend me1;

    SC_CTOR(SM3_reg_ahb)
    : SC1("sm3_controller"), me1("compress_extend")
    {
        SC1.clk(clk), SC1.rst(rst), SC1.W(W), SC1.R(R), SC1.count_out(round), SC1.ctrl(state), SC1.finish(finish);
        me1.clk(clk), me1.rst(rst), me1.ctrl(state), me1.finish(finish), me1.m_i(mes_in), me1.Round(round), me1.wout(wout);
        SC_METHOD(assign_wout);
        sensitive << wout;
        SC_METHOD(alwaysBlock1);
        sensitive << control<< state<< round<<
            msg_in1.value_changed()<< msg_in2.value_changed()<< msg_in3.value_changed()<< msg_in4.value_changed()<<
            msg_in5.value_changed()<< msg_in6.value_changed()<< msg_in7.value_changed()<< msg_in8.value_changed()<<
            msg_in9.value_changed()<< msg_in10.value_changed()<< msg_in11.value_changed()<< msg_in12.value_changed()<<
            msg_in13.value_changed()<< msg_in14.value_changed()<< msg_in15.value_changed()<< msg_in16.value_changed();
        SC_METHOD(alwaysBlock2);
        sensitive << clk.pos()<< rst.neg();
        SC_METHOD(alwaysBlock3);
        sensitive << clk.pos()<< rst.neg();
    }

};