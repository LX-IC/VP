/*
 * ahb_sm4.cpp
 *
 *  Created on: Jul 13, 2021
 *      Author: Junwei W
 *	Discrption: AHB Warper for SM4 core
 *	   Verison:
 *
 */
#include "ahb_sm4.h"

#define SIZE_BYTE 0b000
#define SIZE_HALF 0b001
#define SIZE_WORD 0b010

void ahb_sm4::assign_output(){
	//(y_out[0],y_out[1],y_out[2],y_out[3]) = dout.read();
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif
	sc_uint<32> y_out_var[4];

	y_out_var[0] = dout.read().range(127,96);
	y_out_var[1] = dout.read().range(95,64);
	y_out_var[2] = dout.read().range(63,32);
	y_out_var[3] = dout.read().range(31,0);

	y_out[0] = y_out_var[0];
	y_out[1] = y_out_var[1];
	y_out[2] = y_out_var[2];
	y_out[3] = y_out_var[3];

}

void ahb_sm4::assign_din(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif
	sc_biguint<128> x_in_var;
	x_in_var.range(127,96) = x_in[0].read();
	x_in_var.range(95,64) = x_in[1].read();
	x_in_var.range(63,32) = x_in[2].read();
	x_in_var.range(31,0) = x_in[3].read();

	din = x_in_var;
}

void ahb_sm4::assign_hrdata(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif
	HRDATA = rdata;
}

void ahb_sm4::assign_ahb(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif	
	HRESP = 0;
	HREADY = 1;
}

void ahb_sm4::assign_ahb_trans_valid(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif	
	ahb_trans_valid = HSEL.read() && HREADY && (HTRANS.read()[1] == 1);
}

void ahb_sm4::assign_ahb_write_en(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif	
	ahb_write_en = ahb_trans_valid.read() && HWRITE.read();
}

void ahb_sm4::assign_ahb_read_en(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif
	ahb_read_en = ahb_trans_valid.read() && (!HWRITE.read());
}

void ahb_sm4::write_enable(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif	
	if(!HRESETn.read())
	{
		lite_write_req = 0;
	}
	else if(ahb_write_en.read()==1)
	{
		lite_write_req = 1;
	}
	else
	{
		lite_write_req = 0;
	}
}

/*void ahb_sm4::read_enable(){
	if(!HRESETn.read())
	{
		lite_read_req = 0;
	}
	else if(ahb_read_en.read()==1)
	{
		lite_read_req = 1;
	}
	else
	{
		lite_read_req = 0;
	}
}*/

void ahb_sm4::ahb_addr(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif	
	if(!HRESETn.read())
	{
		sm4_addr.write("0xfff") ;
		HSIZE_reg.write("0b000") ;
	}
	else if(ahb_trans_valid)
	{
		sm4_addr = HADDR.read().range(11,0);
		HSIZE_reg = HSIZE.read();
	}
	else
	{
		sm4_addr.write("0xfff");
		HSIZE_reg.write("0b000");
	}
}

void ahb_sm4::control_reg_input(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif	
	if(!HRESETn.read())
	{
		control_reg = "0xffffffff";
	}
	else if((sm4_addr.read()== 0x020) && (lite_write_req.read()==1) && (HSIZE_reg.read() == SIZE_WORD))
	{
		control_reg = HWDATA.read();
	}
//	else
//	{
//		control_reg = control_reg.read();
//	}

}

void ahb_sm4::data_input(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif	
	if(!HRESETn.read())
	{
		for(int i = 0; i < 4; i++)
		{
			x_in[i] = 0;
		}
	}
	else if((sm4_addr.read().range(11,4) == 0b00000000) && (lite_write_req.read()==1) && (HSIZE_reg.read() == SIZE_WORD) && (control_reg.read()[2]==1))
	{
		x_in[sm4_addr.read().range(3,2)] = HWDATA.read();
	}
//	else
//	{
//		for(int i = 0; i < 4; i++)
//		{
//			x_in[i] = x_in[i].read();
//		}
//	}
}

void ahb_sm4::data_output(){
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif	
	if(!HRESETn.read())
	{
		rdata = 0xffffffff;
	}
	else if((ahb_read_en.read() == 1) && (HSIZE.read() == SIZE_WORD))
	{
		if(HADDR.read().range(11,0) == 0x020)
			rdata = control_reg.read();
		else if(HADDR.read().range(11,0) == 0x024)
			rdata = status_reg.read();
		else if(HADDR.read().range(11,4)==0b00000000)
			rdata = x_in[HADDR.read().range(3,2)];
		else if(HADDR.read().range(11,4)==0b00000001)
			rdata = y_out[HADDR.read().range(3,2)];
		else
			rdata = 0xffffffff;
	}
//	else
//	{
//		rdata = rdata.read();
//	}
}

