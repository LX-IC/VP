#pragma once
#include "systemc.h"
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

SC_MODULE(CompressExtend)
{
	// port declaration
	sc_in_clk clk;
	sc_in<bool> rst;
	sc_in<bool> finish;
	sc_in<sc_uint<2> > ctrl;
	sc_in<sc_uint<32> > m_i;
	sc_in<sc_uint<7> > Round;
	sc_out<sc_lv<256> > wout;

	sc_lv<256> WOUT_0;
	sc_signal<sc_uint<32> > W0, W1, W2, W3, W4, W5, W6, W7, W8, W9, W10, W11, W12, W13, W14, W15, Wout67, Wout63, Tj;
	sc_signal<sc_uint<32> > A, B, C, D, E, F, G, H;

	sc_signal<sc_uint<32> > Wj_0_out, W16, Wj_0;
	sc_signal<sc_uint<32> > SS1_0, SS1, SS2, TT1, TT2, FF, GG, next_Tj, next_C, next_E, next_G;
	sc_signal<sc_uint<32> > TT1_0, TT1_1, TT2_0, TT2_1, FF_0, FF_1, GG_0, GG_1;

	// assign blocks
	void assignWj_0();
	void assignW16();

	void assignSS1_0();
	void assignSS1();
	void assignSS2();

	void assignFF_0();
	void assignFF_1();

	void assignGG_0();
	void assignGG_1();

	void assignFF();
	void assignGG();

	void assignTT1();
	void assignTT2();

	void assign_next_Tj();
	void assign_next_C();
	void assign_next_E();
	void assign_next_G();

	// always blocks
	void always_block();

	SC_CTOR(CompressExtend)
	{
		SC_METHOD(assignWj_0);
		sensitive << W0<< W7<< W13;
		SC_METHOD(assignW16);
		sensitive << Wj_0<< W3<< W10;

		SC_METHOD(assignSS1_0);
		sensitive << A<< E<< Tj;
		SC_METHOD(assignSS1);
		sensitive << SS1_0;
		SC_METHOD(assignSS2);
		sensitive << SS1<< A;

		SC_METHOD(assignFF_0);
		sensitive << A<< B<< C;
		SC_METHOD(assignFF_1);
		sensitive << A<< B<< C;

		SC_METHOD(assignGG_0);
		sensitive << E<< F<< G;
		SC_METHOD(assignGG_1);
		sensitive << E<< F<< G;

		SC_METHOD(assignFF);
		sensitive << Round<< FF_0<< FF_1;
		SC_METHOD(assignGG);
		sensitive << Round<< GG_0<< GG_1;

		SC_METHOD(assignTT1);
		sensitive << FF<< D<< SS2<< Wout63;
		SC_METHOD(assignTT2);
		sensitive << GG<< H<< SS1<< Wout67;

		SC_METHOD(assign_next_Tj);
		sensitive << Tj;
		SC_METHOD(assign_next_C);
		sensitive << B;
		SC_METHOD(assign_next_E);
		sensitive << TT2;
		SC_METHOD(assign_next_G);
		sensitive << F;

		SC_METHOD(always_block);
		sensitive << clk.pos()<< rst.neg();
	}
};