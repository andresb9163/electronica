#ifndef PP2_H
#define PP2_H

#include <list>
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stack>
#include <iostream>
#include "tipos.h"
#include "error.h"
//#include "paralelo.h"
#include "usb.h"
//#include "direccion.h"


/** Define los delimitadores finales para las lineas de los programas **/
/** No se me ocurrio como solucionarlo el compilador **/
#define	DELIMITADOR	';'
#define TAM_BUFFER		1024

/** Comandos disponibles **/
#define	CMD_CONTINUE		"CONTINUE"
#define	CMD_LAZO		"LAZO"
#define	CMD_RETL		"RETL"
#define	CMD_FIN			"FIN"

/** Codigos de comandos disponibles **/
#define	CMD_CONTINUE_CODE	0x01
#define	CMD_LAZO_CODE		0x02
#define	CMD_RETL_CODE  		0x03
#define	CMD_FIN_CODE		0x07

/** Define el numero maximo de lazos anidados **/
#define MAX_NUM_LAZOS		0x03



 /** TIENE QUE HABER ALGUNA FORMA DE OCULTAR ESTO EN EL CPP **/

/**********************************************
 * Esta es la clase de instrucciones del pp2  *
 **********************************************/
class pp2_inst {
	word	nl;		// Linea de codigo
	byte 	inst[8];	// Instrucciones de 8 bits

public:
	/** Constructor de instrucciones **/
        void cargar_inst (word numl, word patron, word dato, word nivel,
                            word cmd, word demora);

        /** Devuelve el i-esimo byte que compone la instruccion **/
        byte val_byte (int i);

	/** Proyectores **/
        byte val_comando (void);
        word val_dato (void);
        word val_nl (void);
};


using namespace std;

/**********************************************
 * Esta es la clase del programador de pulsos *
 **********************************************/

class pp2 {
	list<pp2_inst> programa;

public:
	/** Manejadores **/
        void reset_micro (void);
        void modo_carga (void);
        void modo_micro (void);
        void inic_secuencia (void);

        PuertoUSB usb;
        /** Compilador de pseudo-codigo **/
	int compilar (char *nombre_archivo, int *nl, char **e_linea);

        /** Carga el programa en la memoria RAM del dispositivo **/
        void transferir_programa (void);

        /** Tal vez solo sirva de debugeo **/
        void mostrar_programa (void);
};





/***************************************************
 LAS INSTRUCCIONES SON DE LA FORMA:

 XX CONTINUE <PATRON> <TIEMPO> ;
 XX LAZO <PATRON> <VECES> <TIEMPO> ;
 XX RETL <PATRON> <DIRECCION> <TIEMPO> ;
 XX FIN <PATRON> ;

***************************************************/



#endif /*PP2_H*/
 