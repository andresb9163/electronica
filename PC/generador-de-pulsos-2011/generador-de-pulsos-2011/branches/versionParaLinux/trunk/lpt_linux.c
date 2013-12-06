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

unsigned int lpt_send_byte(unsigned int port_addr, unsigned char data){
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







/*************************************************************************/

/*  rutinas de Walter para comunicarse con el aparato                    */

/*************************************************************************/

/* Cambiar a CHAR el campo data de 'lpt_send_byte' */
bool direccion(unsigned char address){
    bool result = true;
    
    result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
    /* Delay() */
    
    if (result)
        result = lpt_send_byte(LPT_DATA, address) == 0;

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x07) == 0;
        /* Delay() */

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x05) == 0;
   
   if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
   
   return result;

}



bool escritura(unsigned char data){

    bool result = true;

    result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
    /* Delay() */
    
    if (result)
        result = lpt_send_byte(LPT_DATA, data) == 0;

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x0d) == 0;
        /* Delay() */

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x05) == 0;    
    
    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
        /* Delay() */    

    return result;
   
}





bool endLeer(void) {

    bool result = true;

    result = lpt_send_byte(LPT_CONTROL, 0x24) == 0;/* bus lpt = entrada */

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
    
    return result;
}




bool startLeer(void) {

    int result;

    result = lpt_send_byte(LPT_CONTROL, 0x24) == 0; /* bus lpt = entrada */
    
    return result;

}





unsigned char leer(void)   /*** leer un byte en la direccion corriente ***/

{   int dto = 0, result = 0;

    /* la funcion leer() devuelve el byte leido */


    result = lpt_send_byte(LPT_CONTROL, 0x21) == 0; /* bus inter_p = salida + RD */

    if(result)
        dto = lpt_recive_byte(LPT_DATA);

    if(result)
        result = lpt_send_byte(LPT_CONTROL, 0x24) == 0;

    return(dto);

}

void setganancia(unsigned int ga, unsigned int gb)
{
 /* clk = d0,  rst/ = d1,  dq = d2*/

 unsigned int i, x, w;

  ga = ga & 0xff;
  gb = gb & 0xff;
  w = (gb * 256) | ga;


       x = 2; /* RST =1 (habilitar DS1267)*/
       escritura(x);

    /* transmite bit de stack =0*/
        x = 3;
        escritura(x);
        x = 2;
        escritura(x);
    /* transmitir los 16 bits de ganacia*/

   for(i=0; i<=15; i++)
     {
          x= 2;  /* asume bit = 0 */

	if(w&0x8000) x=6;

	escritura(x); /* salida con fsync = 0 */

        /* pulso de reloj */

	x=x|1;

	escritura(x);

	x=x&06;

	escritura(x);

	w=w<<1;  /* desplazar a la izquierda un bit */

     }

  escritura(2);
  escritura(0);
}


