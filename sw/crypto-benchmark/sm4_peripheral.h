#ifndef __SM4_PERI_H
#define __SM4_PERI_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

//control cmd
#define MOD_IN_KEY 	    0x00000001
#define MOD_IN_ENC 	    0x00000002
#define MOD_IN_DEC 	    0x00000003
#define DATA_UPDATE 	0x00000004
#define SM4_RET 	    0x00000008
//address
#define addr_x_in_0	    0x50003000
#define addr_x_in_1    	0x50003004
#define addr_x_in_2     0x50003008
#define addr_x_in_3     0x5000300c
#define addr_y_out_0    0x50003010
#define addr_y_out_1    0x50003014
#define addr_y_out_2    0x50003018
#define addr_y_out_3    0x5000301c
#define addr_control   	0x50003020
#define addr_status	    0x50003024


void writeReg32(uint32_t addr, uint32_t data)
{
		*(volatile uint32_t*)addr = data;
}

uint32_t readReg32(uint32_t addr)
{
		return *(volatile uint32_t*)addr;
}

void    sm4_key_schedule (
    uint8_t  mk [16]  //!< Input cipher key
){
    int i;
    uint32_t * mkp = (uint32_t*)mk;
    uint32_t key[4];
    uint32_t addr;
    uint32_t rdata;

    for(i = 0; i < 4; i++)
    {
        key[i] = __builtin_bswap32(mkp[i]);
    }

    writeReg32(addr_control, SM4_RET);
    writeReg32(addr_control, DATA_UPDATE);
    for (i = 0; i < 4; i++)
    {
        addr = addr_x_in_0 + (i * 4);
        writeReg32(addr, key[i]);
    }
    writeReg32(addr_control, MOD_IN_KEY);
    
    while (rdata != 0x04)
    {
        rdata = readReg32(addr_status);
        //printf("data = %08x \n",data);		
    }

}


void    sm4_block_enc_dec (
    uint8_t  out [16], // Output block
    uint8_t  in  [16], // Input block
    uint8_t  mode  // (encrypt or decrypt)
){
    int i;
    uint32_t * inp = (uint32_t*)in ;
    uint32_t * op  = (uint32_t*)out;
    uint32_t x[4];
    uint32_t addr;
    uint32_t rdata;

    for(i = 0; i < 4; i++)
    {
        x[i] = __builtin_bswap32(inp[i]);
    }
    
    if(mode == 1){  //enc
        writeReg32(addr_control, DATA_UPDATE);
        for (i = 0; i < 4; i++)
		{
			addr = addr_x_in_0 + (i * 4);
			writeReg32(addr, x[i]);
		}
        writeReg32(addr_control, MOD_IN_ENC);
        while (rdata != 0x05)
		{
			rdata = readReg32(addr_status);
			// printf("data = %08x \n",rdata);		
		}

        for(i = 0; i < 4; i++)
		{
			addr = addr_y_out_0 + (i * 4);
			x[i] = readReg32(addr);
		}
        for(i = 0; i < 4; i++)
        {
            op[i] = __builtin_bswap32(x[i]); 
        }
    }
    else{           //dec
        writeReg32(addr_control, DATA_UPDATE);
        for (i = 0; i < 4; i++)
		{
			addr = addr_x_in_0 + (i * 4);
			writeReg32(addr, x[i]);
		}
        writeReg32(addr_control, MOD_IN_DEC);
        while (rdata != 0x07)
		{
			rdata = readReg32(addr_status);
			//printf("data = %08x \n",data);		
		}

        for(i = 0; i < 4; i++)
		{
			addr = addr_y_out_0 + (i * 4);
			x[i] = readReg32(addr);
		}
        for(i = 0; i < 4; i++)
        {
            op[i] = __builtin_bswap32(x[i]); 
        }
    }

}

#endif //__SM4_PERI_H
