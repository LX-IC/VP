#include <stdio.h>
#include "sm4.h"

//#define ENCRY	//if define ENCRY, excute encry, else excute decry.

int main(void)
{
    printf("Hello, RISCV !\n");
    printf("This is a test for RISCV SM4 HW \n");

    sm4_test();
    
    return 0;
}
