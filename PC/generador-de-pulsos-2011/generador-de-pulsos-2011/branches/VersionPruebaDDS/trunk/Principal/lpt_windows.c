#include "lpt_windows.h"

#define PORT_BUSY 0x80
#define ENABLE_PORT 1
#define DISABLE_PORT 0

void delayN(unsigned int n);
void delay4(unsigned int n);


void delayN(unsigned int n)
{
     unsigned int z;
        for(z=0;z<n;z++);
}

//pausa de 4

void delay4(unsigned int n)
{
        delayN(4);
}


unsigned int lpt_send_byte(unsigned int port_addr, unsigned int data){
 	unsigned int status = _inp(port_addr+1);
	unsigned int result = 0;
	if (status == PORT_BUSY){
		result = 1;
	}else{
		_outp(port_addr, data);
	}
	return result;
}

unsigned int lpt_recive_byte(unsigned int port_addr){
	unsigned int status = _inp(port_addr+1);
	unsigned int result = 0;
	if (status == PORT_BUSY){
		result = 1;
	}else{
		result = _inp(port_addr);
	}
	return result;
}

bool lpt_is_busy(unsigned int port_addr){
	unsigned int status = _inp(port_addr+1);
/*	printf("%u\n",(status == PORT_BUSY));*/
	return (status == PORT_BUSY);
}

/*************************************************************************/

/*  rutinas de Walter para comunicarse con el aparato                    */

/*************************************************************************/

/* Cambiar a CHAR el campo data de 'lpt_send_byte' */
unsigned int direccion(unsigned char address){
    unsigned int result = 0;
    
    

    result = lpt_send_byte(LPT_CONTROL, 0x04);
    delayN(30);
    
    if (!result){
        printf("\nDEBUG 'direccion': cambiando de direccion a: %X\n", address);
        result = lpt_send_byte(LPT_DATA, address);
        getchar();
        }
    delayN(100);
    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x05);
    delayN(40);    
    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x07);
    delayN(40);

    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x05);
    delayN(40);   
    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x04);
    delayN(40);   
    

   return result;

}



unsigned int escritura(unsigned char data){

    unsigned int result = 0;

    result = lpt_send_byte(LPT_CONTROL, 0x04);

    delayN(80);
    if (!result){
        printf("\nDEBUG 'escritura': enviando el dato: %X\n", data);
        result = lpt_send_byte(LPT_DATA, data);
        getchar();
        }
    delayN(40);
    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x05);  
    delayN(60);
    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x0d);
        
    delayN(40);
    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x05);    
    delayN(40);    
    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x04);
    delayN(40);
    return result;
   
}





unsigned int endLeer(void) {

    unsigned int result = 0;

    result = lpt_send_byte(LPT_CONTROL, 0x24);/* bus lpt = entrada */

    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x04);
    
    return result;
}




unsigned int startLeer(void) {

    unsigned int  result;

    result = lpt_send_byte(LPT_CONTROL, 0x24); /* bus lpt = entrada */
    
    return result;

}

unsigned char leer(void)   /*** leer un byte en la direccion corriente ***/

{   int dto = 0, result = 0;

    /* la funcion leer() devuelve el byte leido */


    result = lpt_send_byte(LPT_CONTROL, 0x21) == 0; /* bus inter_p = salida + RD */

    if(result)
        dto = lpt_recive_byte(LPT_DATA);

    if(result)
        lpt_send_byte(LPT_CONTROL, 0x24);

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



