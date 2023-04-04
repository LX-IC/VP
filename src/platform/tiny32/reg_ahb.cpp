#include "reg_ahb.hpp"

void SM3_reg_ahb::assign_wout()
{
    msg_out8 = wout.read().range(255, 224);
    msg_out7 = wout.read().range(223, 192);
    msg_out6 = wout.read().range(191, 160);
    msg_out5 = wout.read().range(159, 128);
    msg_out4 = wout.read().range(127, 96);
    msg_out3 = wout.read().range(95, 64);
    msg_out2 = wout.read().range(63, 32);
    msg_out1 = wout.read().range(31, 0);
}

void SM3_reg_ahb::alwaysBlock1()
{
    if (control.read()[5])
        mes_in = 0x0;
    else if (state.read() == write_)
    {
        switch (round.read())
        {
        case 0:   mes_in = msg_in1.read();   break;
        case 1:   mes_in = msg_in2.read();   break;
        case 2:   mes_in = msg_in3.read();   break;
        case 3:   mes_in = msg_in4.read();   break;
        case 4:   mes_in = msg_in5.read();   break;
        case 5:   mes_in = msg_in6.read();   break;
        case 6:   mes_in = msg_in7.read();   break;
        case 7:   mes_in = msg_in8.read();   break;
        case 8:   mes_in = msg_in9.read();   break;
        case 9:   mes_in = msg_in10.read();  break;
        case 10:   mes_in = msg_in11.read();  break;
        case 11:   mes_in = msg_in12.read();  break;
        case 12:   mes_in = msg_in13.read();  break;
        case 13:   mes_in = msg_in14.read();  break;
        case 14:   mes_in = msg_in15.read();  break;
        case 15:   mes_in = msg_in16.read();  break;
        default: mes_in = 0x0; break;
        }
    }
    else
        mes_in = 0x0;
}

void SM3_reg_ahb::alwaysBlock2()
{
    if (!rst.read()){
        W = 0;
        R = 0;  
        write_trig = 0;  
        }
    else if (control.read()[5]){
        W = 0;
        R = 0;
        write_trig = 0;
        }
    else if (!control.read()[3]){
        W = 0;
        R = 1;
        }
    else if (control.read()[4]){
        W = 0;
        R = 0;
        write_trig = 0;
        }
    else if ((!control.read()[4]) && (write_trig.read() == 0) ){
        W = 1;
        R = 0;
        write_trig = 1;
        }
    else if ((round.read() > 16) && (round.read() <69)){
        W = 0;
        R = 0;
        }
    else {
        W = W.read();
        R = R.read();
    }
}

void SM3_reg_ahb::alwaysBlock3()
{
    if (!rst){
        status = 0;
        }
    else if (control.read()[5]){
        status = 0;
        }
    else if (finish == 1){       //compute finish
        status = sta_finish;
        }
    else if ((status.read() != sta_finish) || (control.read()[4])){
        if (state.read() == idle){
            status = 0;
            }
        else if (state.read() == write_){
            status = 1;
            }
        else if (state.read() == encryption){
            status = 2;
            }
        else
            status = 0;
            }
    else
        status = status;
}
