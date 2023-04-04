//#include "tlm/tlmAHBSocket.hpp"
#include "tlmAHBSocket.hpp"


AHBSocket::AHBSocket(
    sc_module_name module_name
    , Addr max_address
    , uint32_t cycle
    )
: sc_module(module_name)
, socket("socket")
, maxAddress(max_address)
, CYCLE(cycle)
{
    assert(maxAddress>=0 && CYCLE>=0); // if m_length < 1, program will be aborted at here
    socket.register_b_transport       (this, &AHBSocket::custom_b_transport);
    socket.register_transport_dbg     (this, &AHBSocket::debug_transport);
}

void AHBSocket::custom_b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
	#ifdef DEBUG_MODE
	printf("enter function: %s in %s \n",__FUNCTION__,__FILE__);
	#endif
	
	tlm::tlm_command cmd = trans.get_command();
	uint32_t addr = (uint32_t)trans.get_address();
	uint32_t* data = (uint32_t*)trans.get_data_ptr();
	trans.set_response_status(tlm::TLM_OK_RESPONSE);
	if (cmd == tlm::TLM_WRITE_COMMAND)
	{
		ahb_write(addr, *data);
		#ifdef DEBUG_MODE
		printf("[AHB socket] (write) addr: %08x  data: %08x\n",addr, *data);
		#endif
	}
		
	else if (cmd == tlm::TLM_READ_COMMAND)
	{
		ahb_read(addr, data);
		#ifdef DEBUG_MODE
		printf("[AHB socket] (read) addr: %08x  data: %08x\n",addr, *data);
		#endif
	}	
	else
		trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
}

unsigned int AHBSocket::debug_transport (tlm_generic_payload &trans)
{
    bool ok = checkPayloadAddress(trans);
    tlm::tlm_command cmd = trans.get_command();
	uint32_t addr = (uint32_t)trans.get_address();
	uint32_t* data = (uint32_t*)trans.get_data_ptr();
	trans.set_response_status(tlm::TLM_OK_RESPONSE);

	if (cmd == tlm::TLM_WRITE_COMMAND)
		ahb_write(addr, *data);
	else if (cmd == tlm::TLM_READ_COMMAND)
		ahb_read(addr, data);
	else
		trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
    return ok;
}

void AHBSocket::ahb_write(uint32_t addr, uint32_t data)
{
	wait(4*CYCLE, SC_NS); // enough time for sequence transfer
	wait(HCLK.posedge_event());
	HSEL = 1;
	HWRITE = 1;
	HADDR = addr;
	HTRANS = 0b10; // non-sequence mode
	HSIZE = 0b010;			//32bit
	wait(CYCLE, SC_NS);
	wait(HCLK.posedge_event());
	HSEL = 0;
	HWRITE = 0;
	HADDR = 0x0;
	HTRANS = 0b00;
	HSIZE = 0b000;			//32bit
	HWDATA = data;	//Reset SM3
	wait(CYCLE, SC_NS);
	// printf("AHB_WRITE addr: 0x%08x  wdata: 0x%08x\n", addr, data);
}

void AHBSocket::ahb_read(uint32_t addr, uint32_t *data)
{
	wait(4*CYCLE, SC_NS);
	wait(HCLK.posedge_event());
	HSEL = 1;
	HWRITE = 0;
	HADDR = addr;
	HTRANS = 0b10; // non-sequence mode
	HSIZE = 0b010;			//32bit
	wait(CYCLE, SC_NS);
	wait(HCLK.posedge_event());
	HSEL = 0;
	HWRITE = 0;
	HADDR = 0x0;
	HTRANS = 0b00;
	HSIZE = 0b000;	
	wait(HCLK.posedge_event());
	*data = HRDATA.read().to_uint();
	wait(CYCLE, SC_NS);
	// printf("AHB READ addr: 0x%08x  rdata: 0x%08x\n", addr, *data);
}

bool AHBSocket::checkAddressRange ( Addr address, Addr bytes) {
            if  ( address + bytes - 1 > maxAddress ) {
                return 0;
            } else {
                return 1;
            }
        }

bool AHBSocket::checkPayloadAddress (tlm_generic_payload &gp) {
    return checkAddressRange(gp.get_address(), gp.get_data_length());
}
