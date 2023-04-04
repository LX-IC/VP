/*
 * sm4.cpp
 *
 *  Created on: Jun 15, 2021
 *      Author: Junwei W
 *    Function: SM4 core implement file
 */

#include "sm4.h"






void sm4::keyextension(){

	sc_biguint<128> key_var;

	key_var = 0;					//reset state
	key_ext_finish.write(false);
	for(int i = 0 ; i < 36 ; i++)
		ulKeyList[i] = 0;


	while(true){
		key_ext_finish.write(false);

		do{ wait(); }while(!(key_extension_i == true));
		key_var = key_i.read();
//		cout.setf(ios::hex,ios::basefield);
//		cout<<"[KEYEXTENSION] get key input: 0x"<<key_var<<endl;
		ulKeyList[3] = key_var.range(31,0)   ^ TBL_SYS_PARAMS[3];
		ulKeyList[2] = key_var.range(63,32)  ^ TBL_SYS_PARAMS[2];
		ulKeyList[1] = key_var.range(95,64)  ^ TBL_SYS_PARAMS[1];
		ulKeyList[0] = key_var.range(127,96) ^ TBL_SYS_PARAMS[0];

		for(int i = 0; i < 32; i++){
			ulKeyList[i+4] =  ulKeyList[i] ^ func_key(ulKeyList[i+1] ^ ulKeyList[i+2] ^ ulKeyList[i+3] ^ TBL_FIX_PARAMS[i]);
			wait();
		}
/*		cout<<"********ulKeyList**************"<<endl;
		for(int i = 0; i < 36; i++){
			printf("%x \t",(int)ulKeyList[i]);
			if((i+1)%12==0) printf("\n");
		}
		cout<<"*******************************"<<endl;*/
		key_ext_finish.write(true);
		wait();
	}
}

void sm4::f_function(){

	sc_biguint<128> data_i_var;
	sc_biguint<128> data_o_var;

	dout_valid.write(false);		//reset state
	data_o.write(0);
	data_i_var=0;
	data_o_var=0;
	for(int i = 0; i < 36 ; i++)
		ulDataList[i] = 0;

	while(true){
		//dout_valid.write(false);

//		wait(3);
		if(din_valid.read()==true)
		{
			dout_valid.write(false);

			data_i_var = data_i.read();
//			cout.setf(ios::hex,ios::basefield);
//			cout<<"[F_FUNCTION] get data input: 0x"<<data_i_var<<" @"<<sc_time_stamp()<<endl;
			ulDataList[3] = data_i_var.range(31,0);
			ulDataList[2] = data_i_var.range(63,32);
			ulDataList[1] = data_i_var.range(95,64);
			ulDataList[0] = data_i_var.range(127,96);
			if(mode_i.read() == 1){
				for(int i = 0; i < 32; i++){
					ulDataList[i+4] = ulDataList[i] ^ func_data(ulDataList[i+1] ^ ulDataList[i+2] ^ ulDataList[i+3] ^ ulKeyList[i+4]);
					wait();
				}
			} else{
				for(int i = 0; i < 32; i++){
					ulDataList[i+4] = ulDataList[i] ^ func_data(ulDataList[i+1] ^ ulDataList[i+2] ^ ulDataList[i+3] ^ ulKeyList[35-i]);
					wait();
				}
			}
			data_o_var = (ulDataList[35],ulDataList[34],ulDataList[33],ulDataList[32]);

			data_o.write(data_o_var);
			dout_valid.write(true);

			wait();
		}
		else wait();
	}
}

