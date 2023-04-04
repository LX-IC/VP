#include "messagecompress.hpp"

void CompressExtend::assignWj_0() 
{
	Wj_0 = W0.read() ^ W7.read() ^ (W13.read().range(16, 0), W13.read().range(31, 17));
}

void CompressExtend::assignW16()
{
	W16 = (Wj_0.get_new_value() ^ 
		(Wj_0.get_new_value().range(16, 0), Wj_0.get_new_value().range(31, 17)) ^
		(Wj_0.get_new_value().range(8, 0), Wj_0.get_new_value().range(31, 9)) ^
		(W3.get_new_value().range(24, 0), W3.get_new_value().range(31, 25)) ^
		W10.get_new_value());
}

// SS1_0 ← ((A ≪ 12) + E + (Tj ≪ j)) ≪ 7
void CompressExtend::assignSS1_0()
{
	SS1_0 = (A.read().range(19, 0), A.read().range(31, 20)) + E.read() + Tj.read();
}
void CompressExtend::assignSS1()
{
	SS1 = (SS1_0.read().range(24, 0), SS1_0.read().range(31, 25));
}

// SS2 ← SS1 ⊕(A ≪ 12)
void CompressExtend::assignSS2()
{
	SS2 = SS1.read() ^ (A.read().range(19, 0), A.read().range(31, 20));
}

//  0  <= j <= 15   FFj(X,Y,Z) function
void CompressExtend::assignFF_0()
{
	FF_0 = A.read() ^ B.read() ^ C.read(); 
}

// 16 <= j <= 63   FFj(X,Y,Z) function
void CompressExtend::assignFF_1()
{
	FF_1 = (A.read() & B.read()) | (A.read() & C.read()) | (B.read() & C.read()); 
}

//  0 <= j <= 15   GGj(X,Y,Z) function
void CompressExtend::assignGG_0()
{
	GG_0 = E.read() ^ F.read() ^ G.read(); 
}

// 16 <= j <= 63   GGj(X,Y,Z) function
void CompressExtend::assignGG_1()
{
	GG_1 = (E.read() & F.read()) | (~E.read() & G.read()); 
}


void CompressExtend::assignFF()
{
	FF = (Round.read() < 21) ? FF_0 : FF_1;
}

void CompressExtend::assignGG()
{
	GG = (Round.read() < 21) ? GG_0 : GG_1;
}

// TT1 ← F Fj (A, B, C) + D + SS2 + W_j'
void CompressExtend::assignTT1()
{
	TT1 = FF.read() + D.read() + SS2.read() + Wout63.read();
}

// TT2 ← GGj (E, F, G) + H + SS1 + W_j
void CompressExtend::assignTT2()
{
	TT2 = GG.read() + H.read() + SS1.read() + Wout67.read();
}


void CompressExtend::assign_next_Tj()
{
	next_Tj = (Tj.read().range(30, 0), Tj.read()[31]);
}

void CompressExtend::assign_next_C()
{
	next_C = (B.read().range(22, 0), B.read().range(31, 23));
}

void CompressExtend::assign_next_E()
{
	next_E = TT2.read() ^ \
			(TT2.read().range(22, 0), TT2.read().range(31, 23)) ^ \
			(TT2.read().range(14, 0), TT2.read().range(31, 15));
}

void CompressExtend::assign_next_G()
{
	next_G = (F.read().range(12, 0), F.read().range(31, 13));
}

void CompressExtend::always_block()
{
	if (!rst.read())
	{
		Wout67  = 0b0;
		Wout63  = 0b0;
		wout = 0b0;  //modified by xhhu

		A  =  IV_0;
		B  =  IV_1;
		C  =  IV_2;
		D  =  IV_3;
		E  =  IV_4;
		F  =  IV_5;
		G  =  IV_6;
		H  =  IV_7;
		Tj  =  T0;
		WOUT_0.range(255, 224) = IV_0;
		WOUT_0.range(223, 192) = IV_1;
		WOUT_0.range(191, 160) = IV_2;
		WOUT_0.range(159, 128) = IV_3;
		WOUT_0.range(127 , 96) = IV_4;
		WOUT_0.range(95 , 64 ) = IV_5;
		WOUT_0.range(63 , 32 ) = IV_6;
		WOUT_0.range(31  , 0 ) = IV_7;
		// modified by xhhu
		W0  = 0;
		W1  = 0;
		W2  = 0;
		W3  = 0;
		W4  = 0;
		W5  = 0;
		W6  = 0;
		W7  = 0;
		W8  = 0;
		W9  = 0;
		W10  = 0;
		W11  = 0;
		W12  = 0;
		W13  = 0;
		W14  = 0;
		W15  = 0;
	}
	else
	{   // message extend + compress
		if (ctrl.read() == 0b01 || ctrl.read() == 0b10)
		{
			switch (Round.read())
			{
				case 0: 
					W0 = m_i.read();
					break;
				case 1: 
					W1 = m_i.read(); 
					break;
				case 2: 
					W2 = m_i.read(); 
					break;
				case 3: 
					W3 = m_i.read(); 
					break;
				case 4: 
					W4 = m_i.read(); 
					Wout67 = W0.read(); // used in TT1 function
					Wout63 = W0.read() ^ m_i.read(); // start calculation
					Tj = T0;
					break;
				case 5: 
					W5 = m_i.read(); 
					Wout67 = W1.read(); // used in TT1 function 
					Wout63 = W1.read() ^ m_i.read(); // start calculation j=0  

					Tj  = next_Tj.read(); // used the last value in SS1 function

					A  = TT1.read();     // message compress
					
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 6:
					W6  = m_i.read();

					Wout67  = W2.read();
					Wout63  = W2.read() ^ m_i.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 7:
					W7  = m_i.read();
					Wout67  = W3.read();
					Wout63  = W3.read() ^ m_i.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 8:
					W8  = m_i.read();
					Wout67  = W4.read();
					Wout63  = W4.read() ^ m_i.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 9:
					W9  = m_i.read();
					Wout67  = W5.read();
					Wout63  = W5.read() ^ m_i.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 10:
					W10  = m_i.read();
					Wout67  = W6.read();
					Wout63  = W6.read() ^ m_i.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 11:
					W11  = m_i.read();
					Wout67  = W7.read();
					Wout63  = W7.read() ^ m_i.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 12:
					W12  = m_i.read();
					Wout67  = W8.read();
					Wout63  = W8.read() ^ m_i.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 13:
					W13  = m_i.read();
					Wout67  = W9.read();
					Wout63  = W9.read() ^ m_i.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 14:
					W14  = m_i.read();
					Wout67  = W10.read();
					Wout63  = W10.read() ^ m_i.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 15:
					W15  = m_i.read();
					Wout67  = W11.read();
					Wout63  = W11.read() ^ m_i.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					break;
				case 16:
				case 17:
				case 18:
				case 19:
					Wout67 = W12.read();
					Wout63  = W12.read() ^ W16.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					W0  = W1.read();
					W1  = W2.read();
					W2  = W3.read();
					W3  = W4.read();
					W4  = W5.read();
					W5  = W6.read();
					W6  = W7.read();
					W7  = W8.read();
					W8  = W9.read();
					W9  = W10.read();
					W10  = W11.read();
					W11  = W12.read();
					W12  = W13.read();
					W13  = W14.read();
					W14  = W15.read();
					W15 = W16.read();
					break;
				case 20:
					Wout67  = W12.read();
					Wout63  = W12.read() ^ W16.read();

					Tj  = T1;    // prepare for j=16

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					W0  = W1.read();
					W1  = W2.read();
					W2  = W3.read();
					W3  = W4.read();
					W4  = W5.read();
					W5  = W6.read();
					W6  = W7.read();
					W7  = W8.read();
					W8  = W9.read();
					W9  = W10.read();
					W10  = W11.read();
					W11  = W12.read();
					W12  = W13.read();
					W13  = W14.read();
					W14  = W15.read();
					W15  = W16.read();
					break;
				case 21:   //j = 16  Tj
				case 22: 
				case 23:
				case 24: 
				case 25:
				case 26: 
				case 27:
				case 28:
				case 29:
				case 30: 
				case 31:
				case 32: 
				case 33:
				case 34: 
				case 35:
				case 36: 
				case 37:
				case 38: 
				case 39:
				case 40:
				case 41:
				case 42: 
				case 43:
				case 44: 
				case 45:
				case 46: 
				case 47:
				case 48: 
				case 49:
				case 50: 
				case 51:
				case 52:
				case 53:
				case 54: 
				case 55:
				case 56: 
				case 57:
				case 58: 
				case 59:
				case 60: 
				case 61:
				case 62: 
				case 63:
				case 64:
				case 65:
				case 66: 
				case 67:
					Wout67  = W12.read();
					Wout63  = W12.read() ^ W16.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					W0  = W1.read();
					W1  = W2.read();
					W2  = W3.read();
					W3  = W4.read();
					W4  = W5.read();
					W5  = W6.read();
					W6  = W7.read();
					W7  = W8.read();
					W8  = W9.read();
					W9  = W10.read();
					W10  = W11.read();
					W11  = W12.read();
					W12  = W13.read();
					W13  = W14.read();
					W14  = W15.read();
					W15  = W16.read();
					break;
				case 68:
					Wout67  = W12.read();
					Wout63  = W12.read() ^ W16.read();

					Tj  = next_Tj.read();

					A  = TT1.read();
					B  = A.read();
					C  = next_C.read();
					D  = C.read();
					E  = next_E.read();
					F  = E.read();
					G  = next_G.read();
					H  = G.read();
					W0  = W1.read();
					W1  = W2.read();
					W2  = W3.read();
					W3  = W4.read();
					W4  = W5.read();
					W5  = W6.read();
					W6  = W7.read();
					W7  = W8.read();
					W8  = W9.read();
					W9  = W10.read();
					W10  = W11.read();
					W11  = W12.read();
					W12  = W13.read();
					W13  = W14.read();
					W14  = W15.read();
					W15  = W16.read();

					WOUT_0.range(255, 224) = TT1.read() ^ WOUT_0.range(255, 224);
					WOUT_0.range(223, 192) = A.read() ^ WOUT_0.range(223, 192);
					WOUT_0.range(191, 160) = next_C.read() ^ WOUT_0.range(191, 160);
					WOUT_0.range(159, 128) = C.read() ^ WOUT_0.range(159, 128);
					WOUT_0.range(127 , 96) = next_E.read() ^ WOUT_0.range(127, 96);
					WOUT_0.range(95 , 64 ) = E.read() ^ WOUT_0.range(95, 64);
					WOUT_0.range(63 , 32 ) = next_G.read() ^ WOUT_0.range(63, 32);
					WOUT_0.range(31  , 0 ) = G.read() ^ WOUT_0.range(31, 0);
					break;
				case 69: 
					A = WOUT_0.range(255, 224).to_uint();
					B = WOUT_0.range(223, 192).to_uint();
					C = WOUT_0.range(191, 160).to_uint();
					D = WOUT_0.range(159, 128).to_uint();
					E = WOUT_0.range(127, 96).to_uint();
					F = WOUT_0.range(95, 64).to_uint();
					G = WOUT_0.range(63, 32).to_uint();
					H = WOUT_0.range(31, 0).to_uint();
					
					wout = WOUT_0;
					break;
				default:
					break;
			}		
		}
		else
			if (ctrl.read() == 0b11)
			{
				Wout67  = 0;
				Wout63  = 0;

				A  = IV_0;
				B  = IV_1;
				C  = IV_2;
				D  = IV_3;
				E  = IV_4;
				F  = IV_5;
				G  = IV_6;
				H  = IV_7;
				Tj  = T0;

				WOUT_0.range(255, 224) = IV_0;
				WOUT_0.range(223, 192) = IV_1;
				WOUT_0.range(191, 160) = IV_2;
				WOUT_0.range(159, 128) = IV_3;
				WOUT_0.range(127, 96) = IV_4;
				WOUT_0.range(95, 64) = IV_5;
				WOUT_0.range(63, 32) = IV_6;
				WOUT_0.range(31, 0) = IV_7;
			}
	}
}