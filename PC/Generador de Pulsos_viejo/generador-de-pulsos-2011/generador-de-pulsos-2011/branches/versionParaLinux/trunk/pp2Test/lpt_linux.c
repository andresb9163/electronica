/*
 * Agregar la opcion -O2 (que es de optimizacion) para la compilacion
 * 
 * En el caso que haya que usar algun tipo de delay usar la funcion usleep de 
 * la libreria unistd.h
 * 
 * Al momento de correr el programa hacerlo como root
 */
#include "lpt_linux.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <stdbool.h>

#define PORT_BUSY 0x80
#define ENABLE_PORT 1
#define DISABLE_PORT 0

unsigned int lpt_send_byte(unsigned int port_addr, unsigned int data){
	unsigned int result = 0;
	/*Habilito permiso de acceso al puerto de direccion port_addr*/
	if (ioperm(port_addr, 1, ENABLE_PORT)) {
		perror("ioperm"); 
		result = 1;
	}
	outb(data, port_addr);
	/*Deshabilito permiso de acceso al puerto de direccion port_addr*/
	if (ioperm(port_addr, 1, DISABLE_PORT)) {
		perror("ioperm"); 
		result = 1;
	}
	return result;
}

unsigned int lpt_recive_byte(unsigned int port_addr){
	unsigned int result = 0;
	/*Habilito permiso de acceso al puerto de direccion port_addr*/
	if (ioperm(port_addr, 1, ENABLE_PORT)) {
		perror("ioperm"); 
		result = 1;
	}
	result = inb(port_addr);
	/*Deshabilito permiso de acceso al puerto de direccion port_addr*/
	if (ioperm(port_addr, 1, DISABLE_PORT)) {
		perror("ioperm"); 
		result = 1;
	}
	return result;
}

bool lpt_is_busy(unsigned int port_addr){
	unsigned int result = false;
	/*Habilito permiso de acceso al puerto de direccion port_addr*/
	if (ioperm(port_addr, 1, ENABLE_PORT)) {
		perror("ioperm"); 
		result = true;
	}
	result = inb(port_addr);
	/*Deshabilito permiso de acceso al puerto de direccion port_addr*/
	if (ioperm(port_addr, 1, DISABLE_PORT)) {
		perror("ioperm"); 
		result = 1;
	}
	return (result == PORT_BUSY);
}
