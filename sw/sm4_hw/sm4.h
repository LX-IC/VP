/*
 * sm4.h
 *
 *  Created on: 2021年5月14日
 *      Author: Junwei W
 */

#ifndef __SM4_H__
#define __SM4_H__

#include <stdio.h>
#include <string.h>
 
typedef unsigned int Uns32;
typedef unsigned short Uns16;
typedef unsigned char Uns8;

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

#define BLEndConvert(X) (((X) & 0x000000ff) << 24) | (((X) & 0x0000ff00) << 8) | (((X) & 0x00ff0000) >> 8) | (((X) & 0xff000000) >> 24)

typedef enum {LE=0, BE} Endian;

void writeReg32(Uns32 addr, Uns32 data, Endian mode)
{
	if (mode == LE)
		*(volatile Uns32*)addr = data;
	else
		*(volatile Uns32*)addr = BLEndConvert(data);
}

Uns32 readReg32(Uns32 addr, Endian mode)
{
	if (mode == LE)
		return *(volatile Uns32*)addr;
	else
	return BLEndConvert(*(volatile Uns32*)addr);
}

Endian get_endian()
{
	Uns16 data = 0x1122;
	Uns8 *p = (Uns8 *)&data;
	return (*p < *(p+1))? BE : LE;
}


void sm4_test()
{
	while(1)
	{
		Uns32 key[4];
		Uns32 plaintext[4];
		Uns32 ciphertext[4];
		Uns32 rdata;
		Uns16 i;
		Uns32 addr;
		
		key[0] = 0x01234567;
		key[1] = 0x89abcdef;
		key[2] = 0xfedcba98;
		key[3] = 0x76543210;
		plaintext[0] = 0x01234567;
		plaintext[1] = 0x89abcdef;
		plaintext[2] = 0xfedcba98;
		plaintext[3] = 0x76543210;

		Endian mode = get_endian();
		printf("CPU mode = %d \n",mode);
		//reset
		writeReg32(addr_control, SM4_RET, mode);
		
		//key extension
		writeReg32(addr_control, DATA_UPDATE, mode);
		for (i = 0; i < 4; i++)
		{
			addr = addr_x_in_0 + (i * 4);
			writeReg32(addr, key[i], mode);
		}
		writeReg32(addr_control, MOD_IN_KEY, mode);
		
		while (rdata != 0x04)
		{
			rdata = readReg32(addr_status, mode);
			//printf("data = %08x \n",data);		
		}
		printf("key extension complete! \n");


		//crypt
		writeReg32(addr_control, DATA_UPDATE, mode);
		for (i = 0; i < 4; i++)
		{
			addr = addr_x_in_0 + (i * 4);
			writeReg32(addr, plaintext[i], mode);
		}
		writeReg32(addr_control, MOD_IN_ENC, mode);		
		while (rdata != 0x05)
		{
			rdata = readReg32(addr_status, mode);
			// printf("data = %08x \n",rdata);		
		}
		printf("encrypt complete! result: \n");
		for(i = 0; i < 4; i++)
		{
			addr = addr_y_out_0 + (i * 4);
			ciphertext[i] = readReg32(addr, mode);
			printf("%08x ",ciphertext[i]);
			if(i == 3)
				printf("\n");
		}

		//decrypt
		writeReg32(addr_control, DATA_UPDATE, mode);
		for (i = 0; i < 4; i++)
		{
			addr = addr_x_in_0 + (i * 4);
			writeReg32(addr, ciphertext[i], mode);
		}
		writeReg32(addr_control, MOD_IN_DEC, mode);	
		while (rdata != 0x07)
		{
			rdata = readReg32(addr_status, mode);
			//printf("data = %08x \n",data);		
		}
		printf("decrypt complete! result: \n");
		for(i = 0; i < 4; i++)
		{
			addr = addr_y_out_0 + (i * 4);
			plaintext[i] = readReg32(addr, mode);
			printf("%08x ",plaintext[i]);
			if(i == 3)
				printf("\n");
		}

	break;
	}
}

#endif /* _SM4_H_ */
