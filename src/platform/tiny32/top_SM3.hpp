#pragma once
#include "systemc.h"
#include "reg_ahb.hpp"

#define SIZE_BYTE  0b00
#define SIZE_HALF  0b01
#define SIZE_WORD  0b10
#define SM3_Reg_addr Reg_ofs_addr.read().range(11, 6)	    //register offset address
#define SM3_N Reg_ofs_addr.read().range(5, 2)				//Number
#define SM3_Reg_addr_R HADDR.read().range(11, 6)			//register offset address
#define SM3_N_R HADDR.read().range(5, 2)					//Number
#define addr_msg_in  0x000
#define addr_control 0x060
#define addr_status  0x064


SC_MODULE(top_SM3)
{
    // port declaration
    sc_in<bool> HRESETn;
    sc_in_clk HCLK;
    sc_in<bool> HWRITE;
    sc_in<bool> HSEL;
    sc_in<bool> HMASTLOOK;
    sc_in<sc_uint<3> > HSIZE, HBURST;
    sc_in<sc_uint<2> > HTRANS;
    sc_in<sc_uint<32> > HADDR;
    sc_in<sc_uint<32> > HWDATA;
    sc_in<sc_uint<4> > HPROT;
    sc_out<bool> HREADY;
    sc_out<bool> HRESP;
    sc_out<sc_uint<32> > HRDATA;

    sc_signal<sc_uint<32> > msg_in[16];                       //Define 16 word 32 bit register ,save x1
    sc_signal<sc_uint<32> > control_reg;                        //control register
    sc_signal<sc_lv<32> > msg_out[8];                      //Define 8 word 32 bit register ,save x3
    sc_signal<sc_lv<32> > msg_out8, msg_out7, msg_out6, msg_out5, msg_out4, msg_out3, msg_out2, msg_out1;
    sc_signal<sc_uint<32> > status_reg;

    sc_signal<bool> ahb_trans_valid;
    sc_signal<bool> ahb_trans_clear;
    sc_signal<bool> ahb_write_en;
    sc_signal<bool> ahb_read_en;
    sc_signal<bool> lite_write_req;
    sc_signal<bool> lite_read_req;

    sc_signal_rv<32> rdata;

    sc_signal<sc_uint<2> >  HTRANS_reg;
    sc_signal<bool>        HWRITE_reg, HSEL_reg;
    sc_signal<sc_uint<12> > Reg_ofs_addr_ff;
    sc_signal<sc_uint<12> > Reg_ofs_addr;
    sc_signal<sc_uint<3> >  HSIZE_reg;

    void assign_HRDATA();
    void assign_HREADY();
    void assign_HRESP();

    void assign_ahb_trans_valid();
    void assign_ahb_write_en();
    void assign_read_en();

    void assign_Reg_ofs_addr();

    void alwaysBlock0();
    void alwaysBlock1();
    void alwaysBlock2();
    void alwaysBlock3();
    void alwaysBlock4();
    void alwaysBlock5();

    SM3_reg_ahb SM3_x;

    SC_CTOR(top_SM3)
        :SM3_x("SM3")
    {
        SC_METHOD(assign_HRDATA);
        sensitive << rdata;
        SC_METHOD(assign_HREADY);
        sensitive << HCLK.pos();
        SC_METHOD(assign_HRESP);
        sensitive << HCLK.pos();

        SC_METHOD(assign_ahb_trans_valid);
        sensitive << HSEL << HREADY<< HTRANS;
        SC_METHOD(assign_ahb_write_en);
        sensitive << ahb_trans_valid<< HWRITE;
        SC_METHOD(assign_read_en);
        sensitive << ahb_trans_valid<< HWRITE;

        SC_METHOD(assign_Reg_ofs_addr);
        sensitive << Reg_ofs_addr_ff;

        SC_METHOD(alwaysBlock0);
        sensitive << msg_out8<< msg_out7<< msg_out6<< msg_out5<< msg_out4<< msg_out3<< msg_out2<< msg_out1;
        SC_METHOD(alwaysBlock1);
        sensitive << HCLK.pos()<< HRESETn.neg();
        SC_METHOD(alwaysBlock2);
        sensitive << HCLK.pos()<< HRESETn.neg();
        SC_METHOD(alwaysBlock3);
        sensitive << HCLK.pos()<< HRESETn.neg();
        SC_METHOD(alwaysBlock4);
        sensitive << HCLK.pos()<< HRESETn.neg();
        SC_METHOD(alwaysBlock5);
        sensitive << HCLK.pos()<< HRESETn.neg();

        SM3_x.clk(HCLK), SM3_x.rst(HRESETn), SM3_x.control(control_reg), SM3_x.status(status_reg);
        SM3_x.msg_in1(msg_in[0]), SM3_x.msg_in2(msg_in[1]), SM3_x.msg_in3(msg_in[2]), SM3_x.msg_in4(msg_in[3]), SM3_x.msg_in5(msg_in[4]), SM3_x.msg_in6(msg_in[5]), SM3_x.msg_in7(msg_in[6]), SM3_x.msg_in8(msg_in[7]);
        SM3_x.msg_in9(msg_in[8]), SM3_x.msg_in10(msg_in[9]), SM3_x.msg_in11(msg_in[10]), SM3_x.msg_in12(msg_in[11]), SM3_x.msg_in13(msg_in[12]), SM3_x.msg_in14(msg_in[13]), SM3_x.msg_in15(msg_in[14]), SM3_x.msg_in16(msg_in[15]);
        SM3_x.msg_out1(msg_out1), SM3_x.msg_out2(msg_out2), SM3_x.msg_out3(msg_out3), SM3_x.msg_out4(msg_out4), SM3_x.msg_out5(msg_out5), SM3_x.msg_out6(msg_out6), SM3_x.msg_out7(msg_out7), SM3_x.msg_out8(msg_out8);
    }
};