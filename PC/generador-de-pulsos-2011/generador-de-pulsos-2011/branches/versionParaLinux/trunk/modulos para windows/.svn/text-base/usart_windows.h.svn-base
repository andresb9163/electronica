#ifndef _USART_H_
#define _USART_H_

/*Direccion de los puertos COM1 y COM2 estadares*/
#define DIR_BASE_COM1 0x3F8
#define DIR_BASE_COM2 0x2F8

/*Configuracion del puerto COM1 por defecto*/
#define COM1_CONF 0x03

/****Configura el puerto ubicado en la direccion port_addr con la configuracion conf
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: Nada
*****/
void usart_configure(unsigned int port_addr, unsigned int conf);

/****
    *Requiere: Leer el registro LINE_STATUS y colocar su valor en el parametro status
    *Modifica: Nada
    *Retorna: codigo de error: 0 = ok
    * 		  				   1 = Error buffer lleno
*****/

unsigned int usart_send(unsigned int port_addr, unsigned int data, unsigned int status);

/****Espera a que haya algo nuevo en el buffer de recepcion (sincronamente)
    *Requiere: Leer el registro LINE_STATUS y colocar su valor en el parametro status
    *Modifica: Nada
    *Retorna: Nada
*****/
void usart_wait_reception(unsigned int status);

/****Toma los datos del buffer de recepcion
    *Requiere: Leer el registro LINE_STATUS y colocar su valor en el parametro status
    *Modifica: Nada
    *Retorna: Los datos guardados en el buffer de recepcion. 
    * 		  En caso de error los codigos son: 0 = ok
    * 		  				   					1 = Error de overrun
    * 											2 = Error de framming
    * 											3 = Error de parity
*****/
unsigned int usart_recive(unsigned int port_addr, unsigned int status);

/****Lee el valor del registro de estado del puerto ubicado en la direccion port_addr
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: El valor del registro de estado del puerto ubicado en la direccion port_addr
*****/
unsigned int usart_read_line_status(unsigned int port_addr);

#endif
