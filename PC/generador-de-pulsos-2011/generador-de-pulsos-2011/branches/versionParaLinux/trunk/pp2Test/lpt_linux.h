#ifndef _LPT_LINUX_H_
#define _LPT_LINUX_H_

#include <stdbool.h>

#define LPT1_ADDR 0x378
#define LPT2_ADDR 0x278

/****Envia los datos alpacendos en data por el puerto ubicado en la direccion port_addr
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: Los datos guardados en el buffer de recepcion. 
    * 		  En caso de error los codigos son: 0 = ok
    * 		  				   					1 = Puerto ocupado
*****/

unsigned int lpt_send_byte(unsigned int port_addr, unsigned int data);

/****Toma los datos del buffer de recepcion
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: Los datos guardados en el buffer de recepcion o 1 en caso que el 
    * 		  puerto este ocupado. 		  
*****/
unsigned int lpt_recive_byte(unsigned int port_addr);

/****Checkea si el puerto ubicado en la direccion port_addr esta disponible
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: True si esta diponible y false en caso contrario
*****/
bool lpt_is_busy(unsigned int port_addr);

#endif
