/*
 * sm4.h
 *
 *  Created on: 2021年5月14日
 *      Author: Junwei W
 */

#ifndef __SM3_H__
#define __SM3_H__

#include <stdio.h>
#include <string.h>
 

typedef unsigned int Uns32; 
typedef unsigned short Uns16;
typedef unsigned char Uns8;
// SM3 相关控制命令定义
#define sm3_open  0xffffffdf

#define sm3_start 0xffffffcf

#define sm3_read  0xffffffd7

#define sm3_reset 0xffffffff

//　SM3 完成状态
#define state_finish 3

// SM3寄存器外部地址
#define addr_msg_in  0x50002000

#define addr_msg_out 0x50002040

#define addr_control 0x50002060

#define addr_status  0x50002064

// ４字节大小端转换函数
#define BLEndConvert(X)   (((X) & 0x000000ff) << 24 ) \
						| (((X) & 0x0000ff00) << 8 )  \
						| (((X) & 0x00ff0000) >> 8 )  \
						| (((X) & 0xff000000) >> 24 )

// 大小端模式（枚举类型）
typedef enum {LE=0, BE} Endian;

// 32位寄存器写函数
void writeReg32(Uns32 addr, Uns32 data, Endian mode)
{
	if (mode == LE)
		*(volatile Uns32*)addr = data;
	else
		*(volatile Uns32*)addr = BLEndConvert(data);
	
}

// 32位寄存器读函数
Uns32 readReg32(Uns32 addr, Endian mode)
{
	if (mode == LE)
		return *(volatile Uns32*)addr;
	else
	return BLEndConvert(*(volatile Uns32*)addr);
}

// 大小端模式判断函数
Endian get_endian()
{
	Uns16 data = 0x1122;
	Uns8 *p = (Uns8 *)&data;
	return (*p < *(p+1))? BE : LE; 
}

// SM3模块测试函数
void sm3_test()
{
	while (1)
	{
		Uns32 data = 0xffffffff;
		Endian mode = get_endian();
		writeReg32(addr_control, sm3_reset, mode);
		writeReg32(addr_control, sm3_open , mode);
		Uns16 i;
		Uns32 addr;
		for (i = 0; i < 16; i++)
		{
			addr = addr_msg_in + (i * 4);
			// 写入扩展后的消息分组，这里以官网例子 “abc”的消息扩展为输入
			switch (i)
			{
			case 0:
				writeReg32(addr, 0x61626380, mode); 
				break;
			case 15:
				writeReg32(addr, 0x00000018, mode);
				break;
			default:
				writeReg32(addr, 0x00000000, mode);
				break;
			}
		}
		// 消息写入完毕，启动SM3模块计算过程
		writeReg32(addr_control, sm3_start, mode);
		while (data != state_finish) // 等待SM3完成计算，即状态寄存器（sm3_status）的值为 3
		{
			data = readReg32(addr_status, mode);
		}
		// SM3计算已完成，开始读取编码后的hash值结果
		writeReg32(addr_control,  sm3_read, mode);
		// 从输出寄存器msg_out读取并打印 256位的哈希值（hash value）
		printf("hash value: \n");
		for (i = 0; i < 8; i++)
		{
			addr = addr_msg_out + (i * 4);
			data = readReg32(addr,  mode);
			printf("%08x ", data);
			if (i == 7)
				printf("\n");
		}
		break; // 结束 while(1)循环，程序退出！
	}
}

#endif /* _SM3_H_ */
