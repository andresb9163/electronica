#include "lpt.h"
#include <DOS.h>

#define PORT_BUSY 0x80

unsigned int lpt_send_byte(unsigned int port_addr, unsigned int data){
	unsigned int status = inportb(port_addr+1);
	unsigned int result = 0;
	if (status == PORT_BUSY){
		result = 1;
	}else{
		outportb(port_addr, data);
	}
	return result;
}

unsigned int lpt_recive_byte(unsigned int port_addr){
	unsigned int status = inportb(port_addr+1);
	unsigned int result = 0;
	if (status == PORT_BUSY){
		result = 1;
	}else{
		result = inportb(port_addr);
	}
	return result;
}

bool lpt_is_busy(unsigned int port_addr){
	unsigned int status = inportb(port_addr+1);
	
	return (status == PORT_BUSY);
}
