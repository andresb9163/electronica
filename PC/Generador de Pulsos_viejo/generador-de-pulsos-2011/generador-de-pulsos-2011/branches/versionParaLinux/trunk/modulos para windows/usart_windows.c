#include "usart.h"
#include <dos.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINE_CONTROL_DIR_COM1 DIR_BASE_COM1+3
#define LINE_STATUS_DIR_COM1 DIR_BASE_COM1+5
#define DIVISOR_LATCH_LSB_DIR_COM1 DIR_BASE_COM1
#define DIVISOR_LATCH_MSB_DIR_COM1 DIR_BASE_COM1+1

#define SET_DLAB 0x80

#define MASCARA_TX_OK 0x20
#define MASCARA_DATA_READY 0x01
#define MASCARA_OVERRUN_ERR 0x02
#define MASCARA_PARITY_ERR 0x04
#define MASCARA_FRAMMING_ERR 0x08
#define TX_BUFFER_DIR 0x3F8
#define RX_BUFFER_DIR 0x3F8
#define DATA_RECIVED 0x04
#define TRANSMIT_ERR 0x02

void usart_configure(unsigned int port_addr, unsigned int conf){
	outportb(port_addr+3, SET_DLAB);
	outportb(port_addr, 0x0C);
	outportb(port_addr+1, 0x00);
	outportb(port_addr+3, conf);
}

unsigned int usart_send(unsigned int port_addr, unsigned int data, unsigned int status){
	status = status & MASCARA_TX_OK;
	if (status == 32){
		outportb(port_addr, data);
		return 0;
	}else{/*Buffer lleno*/
		return 1;
	}
}

int overrun_err(unsigned int status){
	int result = 0;
	result = status & MASCARA_OVERRUN_ERR;
	if (result == 2){
		result = 1;
	}
	return result;

}

int framming_err(unsigned int status){
	int result = 0;
	result = status & MASCARA_FRAMMING_ERR;
	if (result == 8){
		result = 1;
	}
	return result;
}

int parity_err(unsigned int status){
	int result = 0;
	result = status & MASCARA_PARITY_ERR;
	if (result == 4){
		result = 1;
	}
	return result;
}

unsigned int data_ready(unsigned int status){
	int result = 0;
	unsigned int s = 0;
	s = status & MASCARA_DATA_READY;

	if(s != 1){
		result = 1;
	};

	return result;
}

void usart_wait_reception(unsigned int status){
	while (!data_ready(status));
}

unsigned int usart_recive(unsigned int port_addr, unsigned int status){
	unsigned int result = 0;

	result = inportb(port_addr);
	/*Checkeo de errores de recepcion*/
	if (overrun_err(status)){
		result = 1;
	}else if(framming_err(status)){
		result = 2;
	}else if(parity_err(status)){
		result = 3;
	}

	return result;
}
unsigned int usart_read_line_status(unsigned int port_addr){
	unsigned int status = inportb(port_addr+5);
	return status;
}
