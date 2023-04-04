#ifndef __SM3_PERI_H
#define __SM3_PERI_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rvintrin.h"

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

//the block size in bytes
#define SM3_BLOCK_SIZE (16 * sizeof(uint32_t))

// Reverses the byte order of `V`
#define REVERSE_BYTES_32(V) (_rv32_grev((V), 0x18))

//debug mode
// #define _DEBUG

void writeReg32(uint32_t addr, uint32_t data)
{
    *(volatile uint32_t*)addr = data;
}

uint32_t readReg32(uint32_t addr)
{
    return *(volatile uint32_t*)addr;
}

void sm3_compress(uint32_t * s, uint8_t last_block){
    uint32_t addr;
    uint32_t r_deta = 0xffffffff;
    
    for(int i = 0; i < 16; i++){
        //reverse bytes order
        s[i] = REVERSE_BYTES_32(s[i]);

        addr = addr_msg_in + (i * 4);
        writeReg32(addr, s[i]);
    }
#ifdef _DEBUG
    printf("message block with padding: \n");
    for(int i = 0; i < 16; i++)
        printf("%08x ",s[i]);
    printf("\n");
#endif

    writeReg32(addr_control, sm3_start);
    while (r_deta != state_finish)
    {
        r_deta = readReg32(addr_status);
    }
    if(last_block == 1){
        writeReg32(addr_control, sm3_read);
#ifdef _DEBUG
        printf("hash finished.hash value = ");
#endif
        for(int i = 0; i < 8; i++){
            addr = addr_msg_out + (i * 4);
            s[i] = readReg32(addr);
        }
#ifdef _DEBUG
        for(int i = 0; i < 8; i++)    
            printf("%08x ",s[i]);
        printf("\n");
#endif        
    }
    else{
        writeReg32(addr_control, sm3_open);
#ifdef _DEBUG
        printf("last_block = %d\n",last_block);
#endif

    }
    
}




// Hashes `message` with `len` bytes with SM3 and stores it to `hash`
void sm3_hash(uint8_t hash[32], const uint8_t *message, size_t len)
{
    uint32_t s[16] ;
    uint8_t *b = (uint8_t *)&s[0];
    const uint8_t *m = message;
    size_t remaining = len;
    uint8_t last_block = 0;

    //initial sm3 module
    writeReg32(addr_control, sm3_reset);
	writeReg32(addr_control, sm3_open );

    //Hash complete blocks first
    while (remaining >= SM3_BLOCK_SIZE)
    {
        for (int i = 0; i < SM3_BLOCK_SIZE; ++i) {
            b[i] = m[i];
        }
        sm3_compress(s,last_block);
        remaining -= SM3_BLOCK_SIZE;
        m += SM3_BLOCK_SIZE;
    }
    
    //Hash the last block with padding
    for(int i = 0; i < remaining; i++){
        b[i] = m[i];
    }
    //Append bit 1 after the message
    b[remaining] = 0b10000000;
    ++remaining;
    if (remaining > SM3_BLOCK_SIZE - sizeof(uint64_t)) {
        sm3_compress(s,last_block);
        remaining = 0;
    }
    //Pad everything between the message and the length with zeros
    memset(&b[remaining],0x00,SM3_BLOCK_SIZE - 8 - remaining);
    //Append the length of the message in the last 64 bits
    uint64_t bitlen = 8 * (uint64_t)len;
    s[14] = REVERSE_BYTES_32((uint32_t)(bitlen >> 32));
    s[15] = REVERSE_BYTES_32((uint32_t)bitlen);
    last_block = 1;
#ifdef _DEBUG
    printf("bitlen = %x ,s[14] = %08x , s[15] = %08x\n",bitlen,s[14],s[15]);
#endif
    sm3_compress(s,last_block);

    // stores `s` in `hash` in big-endian
    for (size_t i = 0; i < 8; ++i) {
        hash[i * 4 + 0] = (uint8_t)(s[i] >> 24);
        hash[i * 4 + 1] = (uint8_t)(s[i] >> 16);
        hash[i * 4 + 2] = (uint8_t)(s[i] >> 8);
        hash[i * 4 + 3] = (uint8_t)(s[i] >> 0);
    }
}



#endif //SM3_PERI_H