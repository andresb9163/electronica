#ifndef ERROR_H
#define ERROR_H

/** Codigos de error **/
#define EFREQMAX        1	// Frecuencia maxima excedida
#define EFREQMIN        2	// Frecuencia minima excedida
#define EFASEMAX        3	// Fase maxima excedida
#define EFASEMIN        4	// Fase minima excedida
#define	EPOSINUM	5	// Posicion de frecuencia o fase incorrecta
#define EFAILURE	6	// Falla general
#define ENOCOMANDO	7       // No se ingreso comando valido
#define	ENOVALLINEA	8       // No se encontro numero de linea
#define	ENONUMLINEA	9       // Formato de numero de linea incorrecto
#define	ENOVALPATRON	10      // No se encontro un valor de patron
#define	ENOPATRON	11      // Formato de patron incorrecto
#define	ENOVALDATO	12      // No se encontro un valor de dato
#define	ENODATO		13      // Formato de dato incorrecto
#define	ENOVALTIEMPO	14      // No se encontro un valor de demora
#define	ENOTIEMPO	15	// Formato de demora incorrecto
#define EMAXLAZOS	16	// Numero maximo de lazos excedido
#define ENOARCHIVO	17	// Se intento acceder algo que no es un archivo
#define ELAZO		18	// Se abrio un lazo que nunca fue cerrado.
#define	ESINTAXIS	80	// Error sintactico en el programa
#define ESEMANTICO	90	// Error semantico en el programa

/** Mensajes de error **/
#define MEFREQMAX       "Se ha excedido el rango de frecuencia maxima."
#define MEFREQMIN       "Se ha excedido el rango de frecuencia minima."
#define MEFASEMAX       "Se ha excedido el rango de fase maxima."
#define MEFASEMIN       "Se ha excedido el rango de fase minima."
#define	MEPOSINUM	"Se ha ingresado una posicion no valida."
#define MEFAILURE	"Se ha producido un fallo general en el sistema."
#define MENOCOMANDO	"No se ha ingresado un comando válido."
#define	MENOVALLINEA	"No se pudo encontrar el numero de linea."
#define	MENONUMLINEA	"El formato del numero de linea no es correcto."
#define	MENOVALPATRON	"El campo 'patron' no es de tipo numérico."
#define	MENOPATRON	"El campo 'patron' esta fuera del rango establecido."
#define	MENOVALDATO	"El campo 'dato' no es de tipo numérico."
#define	MENODATO	"El campo 'dato' esta fuera del rango establecido."
#define	MENOVALTIEMPO	"El campo 'demora' no es de tipo numérico."
#define	MENOTIEMPO	"El campo 'demora' esta fuera del rango establecido."
#define MEMAXLAZOS	"Se ha excedido el numero de lazos anidados posibles."
#define MENOARCHIVO	"Se intento acceder un archivo invalido."
#define	MESINTAXIS	"Se detecto un error sintactico en el programa."
#define MELAZO		"Se ejecuto una sentencia de lazo sin un retorno de lazo."
#define MESEMANTICO	"Se detecto un error semantico en el programa."
#define	MEDEFAULT	"Ha ocurrido un error desconocido"

char *error_mensaje (int codigo);

#endif /*ERROR_H*/


