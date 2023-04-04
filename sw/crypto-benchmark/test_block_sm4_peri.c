#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "test.h"
#include "util.h"

#include "sm4_peripheral.h"
#include "core_feature_base.h"


// #define TEST_INSTRS
#define TEST_CYCLES
// #define TEST_TIMES

uint8_t pts [1][16]  = {
    {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
     0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10},
};

uint8_t mks [1][16] = {
    {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
     0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10},
};

uint8_t cts [1][16] = {
    {0x68, 0x1E, 0xDF, 0x34, 0xD2, 0x06, 0x96, 0x5E,
     0x86, 0xB3, 0xE9, 0x4F, 0x53, 0x6E, 0x42, 0x46},
};

int main(void) {
    
    printf("import sys, binascii, Crypto.Cipher.AES as AES\n");
    printf("benchmark_name =  SM4 \n");
    
    uint64_t start_instrs;
    uint64_t start_cycles;
    uint64_t start_times;

    for(int test = 0; test < 1; test ++) {

        uint8_t * pt = pts[test];
        uint8_t * mk = mks[test];
        uint8_t   ct [16];
        uint8_t   fi [16];

        uint32_t  erk[32];
        uint32_t  drk[32];

        #ifdef TEST_INSTRS
        start_instrs        = __get_rv_instret();
        #endif
        #ifdef TEST_TIMES
        start_times         = __get_rv_time();
        #endif
        #ifdef TEST_CYCLES
        start_cycles        = __get_rv_cycle();
        #endif
        sm4_key_schedule(mk);
        #ifdef TEST_CYCLES
        uint64_t kse_ccount = __get_rv_cycle() - start_cycles;
        #endif
        #ifdef TEST_INSTRS
        uint64_t kse_icount = __get_rv_instret() - start_instrs;
        #endif
        #ifdef TEST_TIMES
        uint64_t kse_tcount = __get_rv_time() - start_times;
        #endif

        #ifdef TEST_INSTRS
        start_instrs        = __get_rv_instret();
        #endif
        #ifdef TEST_TIMES
        start_times         = __get_rv_time();
        #endif
        #ifdef TEST_CYCLES
        start_cycles        = __get_rv_cycle();
        #endif
        sm4_block_enc_dec   (ct,pt,1);
        #ifdef TEST_CYCLES
        uint64_t enc_ccount = __get_rv_cycle() - start_cycles;
        #endif
        #ifdef TEST_INSTRS
        uint64_t enc_icount = __get_rv_instret() - start_instrs;
        #endif
        #ifdef TEST_TIMES
        uint64_t enc_tcount = __get_rv_time() - start_times;
        #endif
        

        #ifdef TEST_INSTRS
        start_instrs        = __get_rv_instret();
        #endif
        #ifdef TEST_TIMES
        start_times         = __get_rv_time();
        #endif
        #ifdef TEST_CYCLES
        start_cycles        = __get_rv_cycle();
        #endif
        sm4_block_enc_dec   (fi,ct,0);
        #ifdef TEST_CYCLES
        uint64_t dec_ccount = __get_rv_cycle() - start_cycles;
        #endif
        #ifdef TEST_INSTRS
        uint64_t dec_icount = __get_rv_instret() - start_instrs;
        #endif
        #ifdef TEST_TIMES
        uint64_t dec_tcount = __get_rv_time() - start_times;
        #endif
        
        printf("pt  = "); puthex_py(pt, 16); printf("\n");
        printf("mk  = "); puthex_py(mk, 16); printf("\n");
        printf("ct  = "); puthex_py(ct, 16); printf("\n");
        printf("fi  = "); puthex_py(fi, 16); printf("\n");
        
        #ifdef TEST_INSTRS
        printf("kse_icount = %d \n",(unsigned int)kse_icount); 
        printf("enc_icount = %d \n",(unsigned int)enc_icount); 
        printf("dec_icount = %d \n",(unsigned int)dec_icount);
        // printf("kse_icount = 0x"); puthex64(kse_icount); printf("\n");
        // printf("ksd_icount = 0x"); puthex64(ksd_icount); printf("\n");
        // printf("enc_icount = 0x"); puthex64(enc_icount); printf("\n");
        // printf("dec_icount = 0x"); puthex64(dec_icount); printf("\n");
        #endif
        #ifdef TEST_CYCLES
        printf("kse_ccount = %d \n",(unsigned int)kse_ccount); 
        printf("enc_ccount = %d \n",(unsigned int)enc_ccount); 
        printf("dec_ccount = %d \n",(unsigned int)dec_ccount);
        #endif
        #ifdef TEST_TIMES
        printf("kse_tcount = %d \n",(unsigned int)kse_tcount); 
        printf("ksd_tcount = %d \n",(unsigned int)ksd_tcount); 
        printf("enc_tcount = %d \n",(unsigned int)enc_tcount); 
        printf("dec_tcount = %d \n",(unsigned int)dec_tcount);
        #endif 
 
 

        int tr = 0;
        for(int i = 0; i < 16; i ++) {

            if(ct[i] != cts[test][i]) {
                tr |= 1; 
            }
            if(fi[i] != pt[i])        {
                tr |= 2;
            }

        }

        if(tr) {
            printf("print('"STR(SM4)"Test %d Failed with code: %d')\n", test, tr);
            printf("sys.exit(1)\n", test, tr);
            return tr;
        } else {
        printf("SM4 Test passed. \n");

        }
    }

    return 0;
}