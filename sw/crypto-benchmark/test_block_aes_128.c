#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "util.h"

#include "api_aes.h"
#include "core_feature_base.h"

// #define CHECK_WITH_PY

// #define TEST_INSTRS
#define TEST_CYCLES
// #define TEST_TIMES

#ifdef CHECK_WITH_PY
void test_aes_128(int num_tests) {

    // Start with known inputs from FIPS 197, Appendix B.
    uint8_t  key [AES_128_KEY_BYTES ] = {0x2b ,0x7e ,0x15 ,0x16 ,0x28 ,0xae ,0xd2 ,0xa6 ,0xab ,0xf7 ,0x15 ,0x88 ,0x09 ,0xcf ,0x4f ,0x3c};
    uint8_t  pt  [AES_BLOCK_BYTES   ] = {0x32 ,0x43 ,0xf6 ,0xa8 ,0x88 ,0x5a ,0x30 ,0x8d ,0x31 ,0x31 ,0x98 ,0xa2 ,0xe0 ,0x37 ,0x07 ,0x34};
    uint32_t erk [AES_128_RK_WORDS  ]; //!< Roundkeys (encrypt)
    uint32_t drk [AES_128_RK_WORDS  ]; //!< Roundkeys (decrypt)
    uint8_t  ct  [AES_BLOCK_BYTES   ];
    uint8_t  pt2 [AES_BLOCK_BYTES   ];
    #ifdef TEST_INSTRS
    uint64_t start_instrs;
    #elif defined TEST_CYCLES
    uint64_t start_cycles;
    #endif

    for(int i = 0; i < num_tests; i ++) {

        #ifdef TEST_INSTRS
        start_instrs        = __get_rv_instret();
        #elif defined TEST_CYCLES
        start_cycles        = __get_rv_cycle();
        #endif

        aes_128_enc_key_schedule(erk, key    );

        #ifdef TEST_INSTRS
        uint64_t kse_icount = __get_rv_instret() - start_instrs;
        #elif defined TEST_CYCLES
        uint64_t kse_ccount = __get_rv_cycle() - start_cycles;
        #endif

        #ifdef TEST_INSTRS
        start_instrs        = __get_rv_instret();
        #elif defined TEST_CYCLES
        start_cycles        = __get_rv_cycle();
        #endif

        aes_128_ecb_encrypt     (ct , pt, erk);
        
        #ifdef TEST_INSTRS
        uint64_t enc_icount = __get_rv_instret() - start_instrs;
        #elif defined TEST_CYCLES
        uint64_t enc_ccount = __get_rv_cycle() - start_cycles;
        #endif

        
        #ifdef TEST_INSTRS
        start_instrs        = __get_rv_instret();
        #elif defined TEST_CYCLES
        start_cycles        = __get_rv_cycle();
        #endif

        aes_128_dec_key_schedule(drk, key    );

        #ifdef TEST_INSTRS
        uint64_t ksd_icount   = __get_rv_instret() - start_instrs;
        #elif defined TEST_CYCLES
        uint64_t ksd_ccount = __get_rv_cycle() - start_cycles;
        #endif

        #ifdef TEST_INSTRS
        start_instrs        = __get_rv_instret();
        #elif defined TEST_CYCLES
        start_cycles        = __get_rv_cycle();
        #endif

        aes_128_ecb_decrypt     (pt2, ct, drk);

        #ifdef TEST_INSTRS
        uint64_t dec_icount = __get_rv_instret() - start_instrs;
        #elif defined TEST_CYCLES
        uint64_t dec_ccount = __get_rv_cycle() - start_cycles;
        #endif
        
        printf("#\n# AES 128 test %d/%d\n",i , num_tests);

        printf("key=");puthex_py(key, AES_128_KEY_BYTES); printf("\n");
        printf("erk=");puthex_py((uint8_t*)erk,AES_128_RK_BYTES);printf("\n");
        printf("drk=");puthex_py((uint8_t*)drk,AES_128_RK_BYTES);printf("\n");
        printf("pt =");puthex_py(pt , AES_BLOCK_BYTES  ); printf("\n");
        printf("pt2=");puthex_py(pt2, AES_BLOCK_BYTES  ); printf("\n");
        printf("ct =");puthex_py(ct , AES_BLOCK_BYTES  ); printf("\n");

        #ifdef TEST_INSTRS
        printf("kse_icount = 0x"); puthex64(kse_icount); printf("\n");
        printf("ksd_icount = 0x"); puthex64(ksd_icount); printf("\n");
        printf("enc_icount = 0x"); puthex64(enc_icount); printf("\n");
        printf("dec_icount = 0x"); puthex64(dec_icount); printf("\n");
        #elif defined TEST_CYCLES
        printf("kse_ccount = 0x"); puthex64(kse_ccount); printf("\n");
        printf("ksd_ccount = 0x"); puthex64(ksd_ccount); printf("\n");
        printf("enc_ccount = 0x"); puthex64(enc_ccount); printf("\n");
        printf("dec_ccount = 0x"); puthex64(dec_ccount); printf("\n");
        #endif

        printf("testnum         = %d\n",i);

        printf("ref_ct          = AES.new(key,AES.MODE_ECB).encrypt(pt    )\n");
        printf("ref_pt          = AES.new(key,AES.MODE_ECB).decrypt(ref_ct)\n");
        printf("if( ref_ct     != ct        ):\n");
        printf("    print(\"AES 128 Test %d encrypt failed.\")\n", i);
        printf("    print( 'key == %%s' %% ( binascii.b2a_hex( key    )))\n");
        printf("    print( 'rk  == %%s' %% ( binascii.b2a_hex(erk     )))\n");
        printf("    print( 'pt  == %%s' %% ( binascii.b2a_hex( pt     )))\n");
        printf("    print( 'ct  == %%s' %% ( binascii.b2a_hex( ct     )))\n");
        printf("    print( '    != %%s' %% ( binascii.b2a_hex( ref_ct )))\n");
        printf("    sys.exit(1)\n");
        printf("elif( ref_pt     != pt2       ):\n");
        printf("    print(\"AES 128 Test %d decrypt failed.\")\n", i);
        printf("    print( 'key == %%s' %% ( binascii.b2a_hex( key    )))\n");
        printf("    print( 'rk  == %%s' %% ( binascii.b2a_hex(drk     )))\n");
        printf("    print( 'ct  == %%s' %% ( binascii.b2a_hex( ct     )))\n");
        printf("    print( 'pt  == %%s' %% ( binascii.b2a_hex( pt2    )))\n");
        printf("    print( '    != %%s' %% ( binascii.b2a_hex( ref_pt )))\n");
        printf("    sys.exit(1)\n");
        printf("else:\n");
        printf("    sys.stdout.write(\""STR(AES128)" AES 128 Test passed. \")\n");
        #ifdef TEST_INSTRS
        printf("    sys.stdout.write(\"enc: %%d, \" %% (enc_icount))\n");
        printf("    sys.stdout.write(\"dec: %%d, \" %% (dec_icount))\n");
        printf("    sys.stdout.write(\"kse: %%d, \" %% (kse_icount))\n");
        printf("    sys.stdout.write(\"ksd: %%d, \" %% (ksd_icount))\n");
        #elif defined TEST_CYCLES
        printf("    sys.stdout.write(\"enc: %%d, \" %% (enc_ccount))\n");
        printf("    sys.stdout.write(\"dec: %%d, \" %% (dec_ccount))\n");
        printf("    sys.stdout.write(\"kse: %%d, \" %% (kse_ccount))\n");
        printf("    sys.stdout.write(\"ksd: %%d, \" %% (ksd_ccount))\n");
        #endif
        printf("    print(\"\")\n");
        
        // New random inputs
        test_rdrandom(pt    , AES_BLOCK_BYTES   );
        test_rdrandom(key   , AES_128_KEY_BYTES );

    }

}
#else
void test_aes_128(int num_tests){
    // Start with known inputs from FIPS 197, Appendix B.
    uint8_t  key [AES_128_KEY_BYTES ] = {0x2b ,0x7e ,0x15 ,0x16 ,0x28 ,0xae ,0xd2 ,0xa6 ,0xab ,0xf7 ,0x15 ,0x88 ,0x09 ,0xcf ,0x4f ,0x3c};
    uint8_t  pt  [AES_BLOCK_BYTES   ] = {0x32 ,0x43 ,0xf6 ,0xa8 ,0x88 ,0x5a ,0x30 ,0x8d ,0x31 ,0x31 ,0x98 ,0xa2 ,0xe0 ,0x37 ,0x07 ,0x34};
    uint32_t erk [AES_128_RK_WORDS  ]; //!< Roundkeys (encrypt)
    uint32_t drk [AES_128_RK_WORDS  ]; //!< Roundkeys (decrypt)
    uint8_t  ct  [AES_BLOCK_BYTES   ];
    uint8_t  pt2 [AES_BLOCK_BYTES   ];

    uint64_t start_cycles;
    uint8_t error_flag ;

    for(int i = 0; i < num_tests; i++)
    {
    	error_flag = 0;
    	for(int j = 0; j < AES_128_RK_WORDS; j++)
    	{
    		erk[j] = 0;
    		drk[j] = 0;
    	}

    	//encrypt key schedule
    	start_cycles = __get_rv_cycle(); //record the cycles
    	aes_128_enc_key_schedule(erk, key);
    	uint64_t kse_ccount = __get_rv_cycle() - start_cycles;

    	//encrypt
    	start_cycles = __get_rv_cycle(); //record the cycles
    	aes_128_ecb_encrypt(ct, pt, erk);
    	uint64_t enc_ccount = __get_rv_cycle() - start_cycles;

    	//decrypt key schedule
    	start_cycles = __get_rv_cycle(); //record the cycles
    	aes_128_dec_key_schedule(drk, key);
    	uint64_t ksd_ccount = __get_rv_cycle() - start_cycles;

    	//decrypt
    	start_cycles = __get_rv_cycle(); //record the cycles
    	aes_128_ecb_decrypt(pt2, ct, drk);
    	uint64_t dec_ccount = __get_rv_cycle() - start_cycles;

    	printf("AES 128 test %d/%d\n", i+1 , num_tests);


    	for(int j = 0; j < AES_BLOCK_BYTES; j++)
    	{
    		if(pt[j] != pt2[j]){
    			error_flag = 1;
    			break;
    		}
    	}

    	if(error_flag == 0){
    		printf("test passed.");
            printf("kse_ccount = %d ",(unsigned int)kse_ccount);
            printf("ksd_ccount = %d ",(unsigned int)ksd_ccount);
            printf("enc_ccount = %d ",(unsigned int)enc_ccount);
            printf("dec_ccount = %d ",(unsigned int)dec_ccount); printf("\n");
    	}
    	else{
			printf("test failed!\n");
	    	printf("key = ");puthex(key, AES_128_KEY_BYTES);printf("\n");
	    	printf("erk = ");puthex((uint8_t *)erk, AES_128_RK_WORDS);printf("\n");
	    	printf("drk = ");puthex((uint8_t *)drk, AES_128_RK_WORDS);printf("\n");
	    	printf("ct = ");puthex(ct, AES_BLOCK_BYTES);printf("\n");
	    	printf("pt = ");puthex(pt, AES_BLOCK_BYTES);printf("\n");
	    	printf("pt2 = ");puthex(pt2, AES_BLOCK_BYTES);printf("\n");
    	}

    	//new random inputs
        test_rdrandom(pt    , AES_BLOCK_BYTES   );
        test_rdrandom(key   , AES_128_KEY_BYTES );

    }
}
#endif


int main(int argc, char ** argv) {

    printf("import sys, binascii, Crypto.Cipher.AES as AES\n");
    printf("benchmark_name = \"" STR(AES128)"\"\n");

    test_aes_128(10);

    return 0;

}