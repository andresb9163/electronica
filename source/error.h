// Copyright 2014 FaMAF

#ifndef ERROR_H
#define ERROR_H


typedef enum r_mod_error {
    EFREQMAX,      // Frecuencia maxima excedida
    EFREQMIN,      // Frecuencia minima excedida
    EFASEMAX,      // Fase maxima excedida
    EFASEMIN,      // Fase minima excedida
    EPOSINUM,      // Posicion de frecuencia o fase incorrecta
    EFAILURE,      // Falla general
    ENOCOMANDO,    // No se ingreso comando valido
    ENOVALLINEA,   // No se encontro numero de linea
    ENONUMLINEA,   // Formato de numero de linea incorrecto
    ENOVALPATRON,  // No se encontro un valor de patron
    ENOPATRON,     // Formato de patron incorrecto
    ENOVALDATO,    // No se encontro un valor de dato
    ENODATO,       // Formato de dato incorrecto
    ENOVALTIEMPO,  // No se encontro un valor de demora
    ENOTIEMPO,     // Formato de demora incorrecto
    EMAXLAZOS,     // Numero maximo de lazos excedido
    ENOARCHIVO,    // Se intento acceder algo que no es un archivo
    ELAZO,         // Se abrio un lazo que nunca fue cerrado.
    ESINTAXIS,     // Error sintactico en el programa
    ESEMANTICO,    // Error semantico en el programa
    EDEFAULT,      // Error mostrado por defecto
} r_mod_error;

#endif  // ERROR_H


