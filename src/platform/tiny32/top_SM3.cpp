#include "top_SM3.hpp"

void top_SM3::assign_HRDATA()
{
    HRDATA = rdata.read();
}

void top_SM3::assign_HREADY()
{
    HREADY = 0b1;
}

void top_SM3::assign_HRESP()
{
    HRESP = 0b0;
}


void top_SM3::assign_ahb_trans_valid()
{
	ahb_trans_valid = HSEL.read() && HREADY && \
					  (HTRANS.read()[1] == 1);
}

void top_SM3::assign_ahb_write_en()
{
	ahb_write_en = ahb_trans_valid.read() && HWRITE.read();
}

void top_SM3::assign_read_en()
{
	ahb_read_en = ahb_trans_valid.read() && (!HWRITE);
}

void top_SM3::assign_Reg_ofs_addr()
{
    Reg_ofs_addr = Reg_ofs_addr_ff.read();
}

void top_SM3::alwaysBlock0()
{
    msg_out[0] = msg_out8.read();
    msg_out[1] = msg_out7.read();
    msg_out[2] = msg_out6.read();
    msg_out[3] = msg_out5.read();
    msg_out[4] = msg_out4.read();
    msg_out[5] = msg_out3.read();
    msg_out[6] = msg_out2.read();
    msg_out[7] = msg_out1.read();
}

void top_SM3::alwaysBlock1()
{
    if (!HRESETn.read())
        lite_write_req = 0;
    else if (ahb_write_en)
        lite_write_req = 1;
    else
        lite_write_req = 0;
}

void top_SM3::alwaysBlock2()
{
    if (!HRESETn.read())
    {
        Reg_ofs_addr_ff  = 0xfff;
        HSIZE_reg     = 0;
    }
    else if (ahb_trans_valid.read())
    {
        Reg_ofs_addr_ff  = HADDR.read().range(11, 0);
        HSIZE_reg  = HSIZE.read();
    }
    else
    {
        Reg_ofs_addr_ff  = Reg_ofs_addr.read();
        HSIZE_reg  = HSIZE_reg.read();
    }
}

void top_SM3::alwaysBlock3()
{
    if (!HRESETn.read())
        control_reg  = 0xffffffff;
    else if ((Reg_ofs_addr.read() == addr_control) && (lite_write_req.read() == 1) &&(HSIZE_reg.read() == SIZE_WORD) )
        control_reg  = HWDATA.read();
    else
        control_reg  = control_reg.read();
}

//////////////////////HWDATA input//////////////////////////////////////// 
void top_SM3::alwaysBlock4()
{
    int sm3_i;
    if(!HRESETn.read()) {
        for(sm3_i = 0; sm3_i<16; sm3_i=sm3_i+1){
            msg_in[sm3_i]  =  0x0;
        }
    }
	else if(control_reg.read()[5]== 1) {
        for(sm3_i = 0; sm3_i<16; sm3_i=sm3_i+1){
            msg_in[sm3_i]  =   0x0;
        }
	}
	else if( (SM3_Reg_addr==addr_msg_in)&&(lite_write_req.read() == 1)&&(HSIZE_reg.read() == SIZE_WORD) ){
		msg_in[SM3_N]  = HWDATA.read();
	}
    else {
        for(sm3_i = 0; sm3_i<16; sm3_i=sm3_i+1){
            msg_in[sm3_i]  = msg_in[sm3_i].read();
        }
    }
}


//////////////////////HRDATA output////////////////////////////////////////
void top_SM3::alwaysBlock5()
{
    if (!HRESETn.read()){
        rdata = 0xffffffff;
        }
    else if (ahb_read_en.read() && (HSIZE.read() == SIZE_WORD)){
        if (HADDR.read().range(11, 0) == addr_control)
            rdata = control_reg.read();
        else if (HADDR.read().range(11, 0) == addr_status)
            rdata = status_reg.read();
        else if (SM3_Reg_addr_R == addr_msg_in)        //Read msg_in
            rdata = msg_in[SM3_N_R].read();
        else if (!HADDR->read()[5])          //Read msg_out
            rdata = msg_out[SM3_N_R].read();
        else                               //Read invalid address,return 0xffff_ffff(-1)
            rdata = 0xffffffff;
    }
    else
        rdata = rdata.read();
}