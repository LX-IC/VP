#include "sm3_control.hpp"

void SM3_Controller::assign_count_plus()
{
	count_plus = count.read() + 1; //the current state is assigned to output wire
}

void SM3_Controller::assign_ctrl()
{
	ctrl = next_state.read(); // counter ,to control the process of the sm3 algorithm
}


void SM3_Controller::alwaysBlock1()
{
    if (!rst.read())
        current_state = idle;
    else
        current_state = next_state.read();
}

void SM3_Controller::alwaysBlock2()
{
    if (!rst.read())
        next_state = idle;
    else
        switch (current_state.read())
        {
            case idle:
                if (W.read() == 1)
                    next_state = write_;
                else if (R.read() == 1)
                    next_state = read_;
                else
                    next_state = idle;
                break;
            case write_:
                if (count.read() < 16)
                    next_state = write_;
                else
                    next_state = encryption;
                break;
            case encryption:
                if (count.read() < 69)
                    next_state = encryption;
                else
                {
                    if (R == 1)
                        next_state = read_;
                    else
                        next_state = idle;
                }
                break;
            case read_:
                next_state = idle;
                break;
            default:
                //  next_state = idle;
                break;
    }
}

void SM3_Controller::alwaysBlock3()
{
    
        if (!rst.read())
        {
            count = 0;
            finish = 0;
            count_out = 0;
        }   
        else   
            switch (next_state.read())
            {
                case idle:
                    count_out = 0;
                    finish = 0;
                    count = 0;
                    break;
                case write_:
                    finish = 0;
                    count = count_plus.read();
                    count_out = count_plus.read();
                    break;
                case encryption:
                    if (count_out.read() < 69)
                    {
                        finish = 0;
                        count = count_plus.read();
                        count_out = count_plus.read();
                    }
                    else
                    {
                        finish = 1;
                        count = count_plus.read();
                        count_out = 0;
                    }
                    break;
                case read_:
                    finish = 0;
                    count_out = 0;
                    count = 0;
                    break;
                /*default:
                    finish = 0;
                    count_out = 0;
                    count = 0;
                    break; 
                    */
            }                
}