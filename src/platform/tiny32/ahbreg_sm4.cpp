/*
 * ahbreg_sm4.cpp
 *
 *  Created on: Jun 26, 2021
 *      Author: Junwei 	W
 */

#include "ahbreg_sm4.h"


/*void ahbreg_sm4::reg_wrap(){

	sc_uint<32> data_opt_reg = 0;		//hard reset
	sc_uint<32> data_stu_reg = 0;
	sc_biguint<128> data_in_reg = 0;
	sm4_rst_n = 0;
	wait();


	while(true){

		data_opt_reg = data_opt.read();


		//opcode decode

		if(data_opt_reg == 8)			//soft reset
		{

			sm4_rst_n = 0;
			data_stu_reg = 0;
			data_stu.write(data_stu_reg);
			wait();
			sm4_rst_n = 1;
			wait();
		}
		else if(data_opt_reg == 4)			//data input update
		{
			data_in_reg = data_in.read();
			cout<<"[AHBREG] data input reg update :"<<data_in_reg<<" @"<<sc_time_stamp()<<endl;
			wait();
		}
		else if(data_opt_reg == 1) 		//key extension
		{
			cout<<"[AHBREG] opcode = KEY_EXT"<<" @"<<sc_time_stamp()<<endl;
			sm4_rst_n = 1;
			key = data_in_reg;
			din_valid = 0;
			key_extension = 1;
			mode = 0;
			wait();
//			key_extension = 0;
			data_stu_reg[2] = 0;
			data_stu.write(data_stu_reg);
			do{ wait(); } while(!(key_ext_finish.read()==true));
		}
		else if(data_opt_reg == 2) //encrypt
		{
			cout<<"[AHBREG] opcode = ENCRYPT"<<" @"<<sc_time_stamp()<<endl;
			sm4_rst_n = 1;
			din = data_in_reg;
			din_valid = 1;
			key_extension = 0;
			mode = 1;
			wait();
			din_valid = 0;
			data_stu_reg[1] = 0;
			data_stu_reg[0] = 0;
			data_stu.write(data_stu_reg);
			do{ wait(); } while(!(dout_valid.read()==true));

		}
		else if(data_opt_reg == 3) //decrypt
		{
			cout<<"[AHBREG] opcode = DECRYPT"" @"<<sc_time_stamp()<<endl;
			sm4_rst_n = 1;
			din = data_in_reg;
			din_valid = 1;
			key_extension = 0;
			mode = 0;
			wait();
			din_valid = 0;
			data_stu_reg[1] = 1;
			data_stu_reg[0] = 0;
			data_stu.write(data_stu_reg);
			do{ wait(); } while(!(dout_valid.read()==true));
		}
		else
		{
			wait();
		}

		if(key_ext_finish.read()==true)		//status register update
		{
			key_extension = 0;
			data_stu_reg[2] = 1;
			data_stu_reg[0] = 0;
			data_stu.write(data_stu_reg);
		}
		else if(dout_valid.read()==true)
		{
			data_stu_reg[2] = 1;
			data_stu_reg[0] = 1;
			data_stu.write(data_stu_reg);

		}

		wait(2);


	}


}*/

void ahbreg_sm4::control_proc(){
	wait();
	while(true){
		#ifdef DEBUG_MODE
		printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
		#endif
		cmd_rst = 0;
		cmd_data_update = 0;
		cmd_key_ext = 0;
		cmd_enc = 0;
		cmd_dec = 0;

		if(data_opt.read()== 8){
			cmd_rst = 1;
			wait();
		}
		else if(data_opt.read()== 4){
			cmd_data_update = 1;
			wait();
		}
		else if(data_opt.read()== 1 ){
			cmd_key_ext = 1;
			wait();

		}
		else if(data_opt.read()== 2 ){
			cmd_enc = 1;
			wait();

		}
		else if(data_opt.read()== 3 ){
			cmd_dec = 1;
			wait();

		}
		else
			wait();
	}
}

void ahbreg_sm4::cmd_delay(){
	cmd_key_ext_reg = 0;
	cmd_enc_reg = 0;
	cmd_dec_reg = 0;
	wait();
	while(true){
		#ifdef DEBUG_MODE
		printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
		#endif
		cmd_key_ext_reg = cmd_key_ext;
		cmd_enc_reg = cmd_enc;
		cmd_dec_reg = cmd_dec;
		wait();
	}
}

void ahbreg_sm4::reset_proc(){
	wait();
	while(true){
		#ifdef DEBUG_MODE
		printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
		#endif	
		if(cmd_rst.read()== 1){
			sm4_rst_n = 0;
			wait();
		}
		else
			wait();
	}
}

void ahbreg_sm4::data_update_proc(){
	wait();
	while(true){
		#ifdef DEBUG_MODE
		printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
		#endif
		if(cmd_data_update.read()== 1){
			data_in_reg = data_in.read();
			sm4_rst_n = 1;
//			cout<<"[AHBREG] data input reg update :"<<data_in.read()<<" @"<<sc_time_stamp()<<endl;
			wait();
		}
		else
			wait();
	}
}

void ahbreg_sm4::key_ext_proc(){
	wait();
	while(true){
		#ifdef DEBUG_MODE
		printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
		#endif
		if(cmd_key_ext.read()== 1){
//			cout<<"[AHBREG] opcode = KEY_EXT"<<" @"<<sc_time_stamp()<<endl;
			sm4_rst_n = 1;
			key = data_in_reg;
			din_valid = 0;
			key_extension = cmd_key_ext.read() &  (!cmd_key_ext_reg.read());
			mode = 0;
			wait();
		}
		else
			wait();
	}
}

void ahbreg_sm4::enc_proc(){
	wait();
	while(true){
		#ifdef DEBUG_MODE
		printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
		#endif
		if(cmd_enc.read()==1){
//			cout<<"[AHBREG] opcode = ENCRYPT"<<" @"<<sc_time_stamp()<<endl;
			sm4_rst_n = 1;
			din = data_in_reg;
			din_valid = cmd_enc.read() &  (!cmd_enc_reg.read());
			key_extension = 0;
			mode = 1;
			wait();
		}
		else
			wait();
	}
}

void ahbreg_sm4::dec_proc(){
	wait();
	while(true){
		if(cmd_dec == 1){
			#ifdef DEBUG_MODE
			printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
			#endif
//			cout<<"[AHBREG] opcode = DECRYPT"" @"<<sc_time_stamp()<<endl;
			sm4_rst_n = 1;
			din = data_in_reg;
			din_valid = cmd_dec.read() &  (!cmd_dec_reg.read());
			key_extension = 0;
			mode = 0;
			wait();
		}
		else
			wait();
	}
}

void ahbreg_sm4::default_proc(){
	wait();
	while(true){
		#ifdef DEBUG_MODE
		printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
		#endif
		if(cmd_rst == 0 && cmd_data_update == 0 && cmd_key_ext == 0 && cmd_enc ==0 && cmd_dec == 0){
			sm4_rst_n = 1;
			din = data_in_reg;
			din_valid = 0;
			key_extension = 0;
			mode = 0;
			wait();
		}
		else
			wait();
	}
}

void ahbreg_sm4::status_reg_proc(){

//	data_out = 0;
//	data_in_reg = 0;
	sc_uint<32> data_stu_reg ;


		#ifdef DEBUG_MODE
		printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
		#endif
		if(key_ext_finish.read()){
//			key_extension = 0;
			data_stu_reg = 0x04;
		}
		else if(dout_valid.read()==true && cmd_enc.read()==true && din_valid.read()== false && cmd_enc_reg.read()==true){
			data_stu_reg = 0x05;

		}
		else if(dout_valid.read()==true && cmd_dec.read()==true && din_valid.read()== false && cmd_dec_reg.read()==true){
			data_stu_reg = 0x07;

		}
		else if(cmd_enc.read()==true){
			data_stu_reg = 0x04;
		}
		else if(cmd_dec.read()==true){
			data_stu_reg = 0x06;
		}
		else if(sm4_rst_n.read() == false)
			data_stu_reg = 0x0;
		else
			data_stu_reg = data_stu.read();

		data_stu.write(data_stu_reg);

	}






