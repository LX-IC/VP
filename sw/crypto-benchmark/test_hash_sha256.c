#include <stdlib.h>
#include <string.h>

#include "test.h"

#include "api_sha256.h"
#include "core_feature_base.h"

// #define SIM_DETAIL

// #define TEST_INSTRS
#define TEST_CYCLES
// #define TEST_TIMES

#ifdef SIM_DETAIL
int main(int argc, char ** argv) {

    printf("import sys, binascii, Crypto.Hash.SHA256 as SHA2_256\n");
    printf("benchmark_name = \"" STR(TEST_NAME)"\"\n");

    const int num_tests = 10;
        
    size_t     message_len  = TEST_HASH_INPUT_LENGTH  ;
    uint8_t  * message      ;
    uint32_t   digest    [8];

    for(int i = 0; i < num_tests; i ++) {

        message  = calloc(message_len, sizeof(unsigned char));

        test_rdrandom(message, message_len);

        #ifdef TEST_INSTRS
        const uint64_t start_instrs   = __get_rv_instret();
        #elif defined TEST_CYCLES
        const uint64_t start_cycles   = __get_rv_cycle();
        #endif

        sha256_hash (
            digest      ,
            message     ,
            message_len
        );
        
        #ifdef TEST_INSTRS
        const uint64_t end_instrs     = __get_rv_instret();
        const uint64_t final_instrs   = end_instrs - start_instrs;
        #elif defined TEST_CYCLES
        const uint64_t end_cycles     = __get_rv_cycle();
        const uint64_t final_cycles   = end_cycles - start_cycles;
        #endif


        printf("#\n# test %d/%d\n",i , num_tests);

        printf("input_len       = %u\n", message_len);
        
        printf("input_data      = ");
        puthex_py(message,message_len);
        printf("\n");

        printf("signature       = ");
        puthex_py((uint8_t*)digest, 8*4);
        printf("\n");

        #ifdef TEST_INSTRS
        printf("instr_count     = 0x");
        puthex64(final_instrs);
        printf("\n");
        #elif defined TEST_CYCLES
        printf("cycle_count     = 0x");
        puthex64(final_cycles);
        printf("\n");
        #endif
        
        printf("testnum         = %d\n",i);
        #ifdef TEST_INSTRS
        printf("ipb             = instr_count / input_len\n");
        #elif defined TEST_CYCLES
        printf("cpb             = cycle_count / input_len\n");
        #endif

        printf("reference       = SHA2_256.new(input_data).digest()\n");
        printf("if( reference  != signature ):\n");
        printf("    print(\"Test %d failed.\")\n", i);
        printf("    print( 'input     == %%s' %% ( binascii.b2a_hex( input_data ) ) )" "\n"   );
        printf("    print( 'reference == %%s' %% ( binascii.b2a_hex( signature ) ) )" "\n"   );
        printf("    print( '          != %%s' %% ( binascii.b2a_hex( reference ) ) )" "\n"   );
        printf("    sys.exit(1)\n");
        printf("else:\n");
        #ifdef TEST_INSTRS
        printf("    print(\""STR(TEST_NAME)" Test %%d passed. "
               "%%d instrs / %%d bytes. IPB=%%f\" %% "
               "(testnum,instr_count,input_len,ipb))\n");
        #elif defined TEST_CYCLES
        printf("    print(\""STR(TEST_NAME)" Test %%d passed. "
        "%%d cycles / %%d bytes. CPB=%%f\" %% "
        "(testnum,cycle_count,input_len,cpb))\n");
        #endif

        message_len += TEST_HASH_INPUT_LENGTH / 2;

        free(message);

    }

    return 0;
}
#else
int main(void)
{

    printf("benchmark_name = \"" STR(SHA256)"\"\n");

    const int num_test = 1;

    uint32_t hash_input_len = TEST_HASH_INPUT_LENGTH;
    uint8_t * hash_input ;
    uint32_t hash_signature[8];

    for(int i = 0; i < num_test; i++){

    	printf("test %d/%d\n", i+1, num_test);

    	hash_input = calloc(hash_input_len, sizeof(uint8_t));

    	test_rdrandom(hash_input, hash_input_len);

    	const uint64_t start_cycles = __get_rv_cycle();

        sha256_hash (
        	hash_signature ,
			hash_input     ,
			hash_input_len
        );

    	const uint64_t end_cycles = __get_rv_cycle();
    	const uint64_t final_cycles = end_cycles - start_cycles;

    	printf("input_len = %d\n", hash_input_len);
    	printf("input_data = ");puthex(hash_input,hash_input_len);printf("\n");
    	printf("signature = ");puthex((uint8_t *)hash_signature, 8*4);printf("\n");
    	printf("cycle_count = %d\n",(unsigned int)final_cycles);

    	hash_input_len += TEST_HASH_INPUT_LENGTH / 2;

    	free(hash_input);
    }


    return 0;
}
#endif