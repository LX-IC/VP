#include <stdio.h>
#include "sm3.h"

//#define ENCRY	//if define ENCRY, excute encry, else excute decry.

int main(void)
{
    printf("Hello, RISCV !\n");
    printf("This is a test for RISCV SM3 HW \n");

    sm3_test();
    
    return 0;
}
