// Copyright 2014 FaMAF

#include <stdlib.h>
#include "./error.h"

char error_message_map[][] = {
    "Se ha excedido el rango de frecuencia maxima.",
    "Se ha excedido el rango de frecuencia minima.",
    "Se ha excedido el rango de fase maxima.",
    "Se ha excedido el rango de fase minima.",
    "Se ha ingresado una posicion no valida.",
    "Se ha producido un fallo general en el sistema.",
    "No se ha ingresado un comando válido.",
    "No se pudo encontrar el numero de linea.",
    "El formato del numero de linea no es correcto.",
    "El campo 'patron' no es de tipo numérico.",
    "El campo 'patron' esta fuera del rango establecido.",
    "El campo 'dato' no es de tipo numérico.",
    "El campo 'dato' esta fuera del rango establecido.",
    "El campo 'demora' no es de tipo numérico.",
    "El campo 'demora' esta fuera del rango establecido.",
    "Se ha excedido el numero de lazos anidados posibles.",
    "Se intento acceder un archivo invalido.",
    "Se detecto un error sintactico en el programa.",
    "Se ejecuto una sentencia de lazo sin un retorno de lazo.",
    "Se detecto un error semantico en el programa.",
    "Ha ocurrido un error desconocido"
}

char *error_mensaje(r_mod_error codigo error_code) {
    return error_message_map[error_code]
}
