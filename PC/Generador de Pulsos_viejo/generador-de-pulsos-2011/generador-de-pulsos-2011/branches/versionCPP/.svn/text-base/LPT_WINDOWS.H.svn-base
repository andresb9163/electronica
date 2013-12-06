#ifndef _LPT_H_
#define _LPT_H_



#define LPT1 0x378
#define LPT2 0x278

#define LPT_BASE    LPT1
#define LPT_DATA    LPT_BASE + 0
#define LPT_STATUS  LPT_BASE + 1
#define LPT_CONTROL LPT_BASE + 2

/****Envia los datos almacenados en data por el puerto ubicado en la direccion port_addr
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: Los datos guardados en el buffer de recepcion. 
    * 		  En caso de error los codigos son: 0 = ok
    * 		  				   1 = Puerto ocupado*****/

unsigned int lpt_send_byte(unsigned int port_addr, unsigned char data);

/****Toma los datos del buffer de recepcion
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: Los datos guardados en el buffer de recepcion o 1 en caso que el 
    * 		  puerto este ocupado. 		  
*****/
unsigned char lpt_recive_byte(unsigned int port_addr);

/****Checkea si el puerto ubicado en la direccion port_addr esta disponible
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: True si esta diponible y false en caso contrario
*****/
bool lpt_is_busy(unsigned int port_addr);

/*************************************************************************/

/*  rutinas de Walter para comunicarse con el aparato                    */

/*************************************************************************/

void delayN(unsigned int n);

void delay4(unsigned int n);

unsigned int direccion(unsigned char address);  /* escribe una direccion en el latch address */

unsigned int escritura(unsigned char data);   /* escritura de un byte de datos */

void dds_word(unsigned int w);   /* transmite un word en serie  al dds */

unsigned int startLeer(void);

unsigned int endLeer(void);

unsigned char leer(void) ;   /*** leer un byte en la direccion corriente ***/

void setganancia(unsigned int ga, unsigned int gb);


#endif

