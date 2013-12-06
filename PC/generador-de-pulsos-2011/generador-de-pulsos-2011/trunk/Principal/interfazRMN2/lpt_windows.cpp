#include "lpt_windows.h"

void lpt_windows::delayN(unsigned int n)
{
     unsigned int z;
        for(z=0;z<n;z++);
}

//pausa de 4

void lpt_windows::delay4(unsigned int n)
{
        delayN(4);
}

lpt_windows:: lpt_windows(void){

}

unsigned int lpt_windows::send_byte(unsigned int port_addr, unsigned int data){
    /*unsigned int status = _inp(port_addr+1);
	unsigned int result = 0;
	if (status == PORT_BUSY){
		result = 1;
	}else{
		_outp(port_addr, data);
	}
	return result;*/
    return 0;
}

unsigned int lpt_windows::recive_byte(unsigned int port_addr){
    /*unsigned int status = _inp(port_addr+1);
	unsigned int result = 0;
	if (status == PORT_BUSY){
		result = 1;
	}else{
		result = _inp(port_addr);
	}
	return result;*/
    return 0;
}

bool lpt_windows::is_busy(unsigned int port_addr){
    //unsigned int status = _inp(port_addr+1);
/*	printf("%u\n",(status == PORT_BUSY));*/
	//return (status == PORT_BUSY);
    return 0;
}

    /*************************************************************************/

    /*  rutinas de Walter para comunicarse con el aparato                    */

    /*************************************************************************/


unsigned int lpt_windows::direccion(unsigned char address){  /* escribe una direccion en el latch address */
    unsigned int result = 0;
    
    

    result = send_byte(LPT_CONTROL, 0x04);
    delayN(30);
    
    if (!result)
        result = send_byte(LPT_DATA, address);
    delayN(100);
    if (!result)
        result = send_byte(LPT_CONTROL, 0x05);
    delayN(40);
    if (!result)
        result = send_byte(LPT_CONTROL, 0x07);
    delayN(40);

    if (!result)
        result = send_byte(LPT_CONTROL, 0x05);
    delayN(40);
    if (!result)
        result = send_byte(LPT_CONTROL, 0x04);
    delayN(40);


   return result;
}

unsigned int lpt_windows::escritura(unsigned char data){   /* escritura de un byte de datos */
unsigned int result = 0;

    result = send_byte(LPT_CONTROL, 0x04);

    delayN(80);
    if (!result)
        result = send_byte(LPT_DATA, data);
    delayN(40);
    if (!result)
        result = send_byte(LPT_CONTROL, 0x05);
    delayN(60);
    if (!result)
        result = send_byte(LPT_CONTROL, 0x0d);

    delayN(40);
    if (!result)
        result = send_byte(LPT_CONTROL, 0x05);
    delayN(40);
    if (!result)
        result = send_byte(LPT_CONTROL, 0x04);
    delayN(40);
    return result;
}

void lpt_windows::dds_word(unsigned int w){   /* transmite un word en serie  al dds */


  unsigned char i,x;

  for(i=0; i<=15; i++){

        x=1;  /* asume bit = 0 */

	if(w&0x8000) x=3;

	escritura(x); /* salida con fsync = 0 */

        /* pulso de reloj */

	x=x&2;

	escritura(x);

	x=x|1;

	escritura(x);

	w=w<<1;  /* desplazar a la izquierda un bit */

  }

  escritura(7);
}

unsigned int lpt_windows::startLeer(void){
    unsigned int  result;

    result = send_byte(LPT_CONTROL, 0x24); /* bus lpt = entrada */
    
    return result;
}

unsigned int lpt_windows::endLeer(void){
    unsigned int result = 0;

    result = send_byte(LPT_CONTROL, 0x24);/* bus lpt = entrada */

    if (!result)
        result = send_byte(LPT_CONTROL, 0x04);

    return result;
}

unsigned char lpt_windows::leer(void){   /*** leer un byte en la direccion corriente ***/
    int dto = 0, result = 0;

    /* la funcion leer() devuelve el byte leido */


    result = send_byte(LPT_CONTROL, 0x21) == 0; /* bus inter_p = salida + RD */

    if(result)
        dto = recive_byte(LPT_DATA);

    if(result)
        send_byte(LPT_CONTROL, 0x24);

    return(dto);
}

void lpt_windows::setganancia(unsigned int ga, unsigned int gb)
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

