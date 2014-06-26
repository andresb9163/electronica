// Copyright 2014 FaMAF

#include "./pp2.h"

using namespace std;

void pp2_inst::cargar_inst(word numl, word patron, word dato, word nivel,
                            word cmd, word demora) {
    nl = numl;
    inst[0] =  demora & 0x000000FF;
    inst[1] = (demora & 0x0000FF00) / 0x100;
    inst[2] = (demora & 0x00FF0000) / 0x10000;
    inst[3] = (demora & 0xFF000000) / 0x1000000;
    inst[4] =  cmd + nivel * 8 + (dato & 0x07) * 32;
    inst[5] = (dato & 0x7f8) / 8;
    inst[6] =  patron & 0x00ff;
    inst[7] = (patron & 0xff00) / 256;
}

byte pp2_inst::val_byte(int i) {
    byte b = 0x00;
    if (0 <= i && i <= 7) {
        b = inst[i];
    }
    return b;
}

byte pp2_inst::val_comando(void) {
    byte b;
    b = val_byte(4);
    b = b & 0x07;
    return b;
}

word pp2_inst::val_dato(void) {
    word w;
    w = val_byte(5) * 8 + (val_byte(4) & 0xE0) / 32;
    return w;
}

word pp2_inst::val_nl(void) {
    return nl;
}


/*******************************************
 * Funciones auxiliares internas al modulo *
 *******************************************/

/**
 * es_linea_valida: Chequea que la linea de codigo es contigua.
 * @nl: Numero de linea a analizar.
 * @nl_esp: Numero de linea que se espera.
 */
bool es_linea_valida(word nl, word nl_esp) {
    return (nl == nl_esp);
}

/**
 * es_patron_valido: Chequea que el patron este en el rango establecido.
 * @patron: Valor a analizar.
 */
bool es_patron_valido(word patron) {
    return (0 <= patron && patron <= 65536);
}

/**
 * es_dato_valido: Chequea que el dato este en el rango establecido.
 * @dato: Valor a analizar.
 */
bool es_dato_valido(word dato) {
    return (0 <= dato && dato <= 2048);
}

/**
 * es_nivel_valido: Chequea que el nivel este en el rango establecido.
 * @nivel: Valor a analizar.
 */
bool es_nivel_valido(word nivel) {
    return (0 <= nivel && nivel <= 3);
}

/**
 * es_demora_valida: Chequea que la demora este en el rango establecido.
 * @demora: Valor a analizar.
 */
bool es_demora_valida(word demora) {
    return (0 <= demora && demora <= 0xfffffffe);
}

/**
 * a_word: Convierte una cadena de caracteres en un word.
 * @valor: Cadena a convertir.
 */
word a_word(const char* valor) {
    AnsiString numero = valor;
    word res = numero.ToIntDef(-1);
    return res;
}

/**
 * es_numero: Analiza si el formato de una cadena corresponde a un valor
 *        numerico.
 * @valor: Expresion a analizar.
 */
bool es_numero(const char *valor) {
    int retval = true;

    AnsiString numero = valor;
    word valor_posible = numero.ToIntDef(-1);
    if (valor_posible == -1) {
        retval = false;
    }
    return retval;
}

/**
 * tiene_comando: Revisa si la la cadena @linea contiene el comando @cmd.
 * @linea: Cadena a analizar.
 * @cmd: Comando que se esta buscando.
 */
bool tiene_comando(const char* linea, const char* cmd) {
    char comando[TAM_BUFFER];
    char basura[TAM_BUFFER];

    memset(comando, '\0', TAM_BUFFER);

    sscanf(linea, "%s %s", basura, comando);
    return strcmpi(comando, cmd) == 0;
}

/**
 * analizar_numero_linea: Revisa que el numero de linea de una sentencia sea
 *            valido.
 * @linea: Cadena de caractres a analizar.
 * @nl_esp: Numero de linea de condigo que se espera.
 * Devuelve: 0 en caso de exito o valor de error apropiado en caso de ocurrir
 *       alguno.
 */
int analizar_numero_linea(const char* linea, word nl_esp) {
    int retcode = 0;
    char numero[TAM_BUFFER];

    memset(numero, '\0', TAM_BUFFER);

    sscanf(linea, "%s", numero);
    if (es_numero(numero)) {
        if (!es_linea_valida(a_word(numero), nl_esp)) {
        retcode = -ENOVALLINEA;
            }
    } else {
        retcode = -ENONUMLINEA;
    }
    return retcode;
}

/**
 * analizar_patron: Revisa que el patron de una sentencia sea valido.
 * @linea: Cadena de caractres a analizar.
 * Devuelve: 0 en caso de exito o valor de error apropiado en caso de ocurrir
 *       alguno.
 */
int analizar_patron(const char* linea) {
    int retcode = 0;
    char basura[TAM_BUFFER], patron[TAM_BUFFER];

    memset(patron, '\0', TAM_BUFFER);

    sscanf(linea, "%s %s %s", basura, basura, patron);
    if (es_numero(patron)) {
        if (!es_patron_valido(a_word(patron))) {
            retcode = -ENOVALPATRON;
        }
    } else {
        retcode = -ENOPATRON;
    }
    return retcode;
}

/**
 * analizar_dato: Revisa que el dato de una sentencia sea valido.
 * @linea: Cadena de caractres a analizar.
 * Devuelve: 0 en caso de exito o valor de error apropiado en caso de ocurrir
 *       alguno.
 */
int analizar_dato(const char* linea) {
    int retcode = 0;
    char basura[TAM_BUFFER], dato[TAM_BUFFER];

    memset(dato, '\0', TAM_BUFFER);

    sscanf(linea, "%s %s %s %s", basura, basura, basura, dato);
    if (es_numero(dato)) {
        if (!es_dato_valido(a_word(dato))) {
            retcode = -ENOVALDATO;
        }
    } else {
        retcode = -ENODATO;
    }
    return retcode;
}

/**
 * analizar_tiempo: Revisa que la demora de una sentencia sea valido.
 * @linea: Cadena de caractres a analizar.
 * @pos: Posicion del campo en donde se espera encontrar la expresion de tiempo.
 * Devuelve: 0 en caso de exito o valor de error apropiado en caso de ocurrir
 *       alguno.
 * Nota: No se efectua el control de que @pos sea un valor valido, porque
 *   confiamos en la inteligencia del programador.
 */
int analizar_tiempo(const char* linea, int pos) {
    int retcode = 0;
    char basura[TAM_BUFFER], tiempo[TAM_BUFFER];

    memset(tiempo, '\0', TAM_BUFFER);

    if (pos == 4) {
        sscanf(linea, "%s %s %s %s", basura, basura,
                basura, tiempo);
    } else if (pos == 5) {
        sscanf(linea, "%s %s %s %s %s", basura, basura,
                basura, basura, tiempo);
    }
    if (es_numero(tiempo)) {
        if (!es_demora_valida(a_word(tiempo))) {
            retcode = -ENOVALTIEMPO;
        }
    } else {
        retcode = -ENOTIEMPO;
    }
    return retcode;
}

/**
 * comando: Analiza una linea de codigo en busca de comnados.
 * @llinea: Cadena de caracteres a analizar.
 * @nl_esp: Numero de linea esperado.
 * @inst: Puntero a instruccion del programador de pulsos, en donde se almacena
 *    la instruccion de 64 bits asociada al comando encontrado.
 * Devuelve: 0 en caso de exito y la instruccion de 64 bits almacenada en @inst.
 *           En caso de fallo, devuelve el codigo de error apropiado y no debe
 *       tenerse en cuenta cualquier contenido de @inst.
 */
int comando(char* linea, word nl_esp, word num_lazo, pp2_inst *inst) {
    int retcode = 0;
        char nl[TAM_BUFFER],   comando[TAM_BUFFER], patron[TAM_BUFFER],
             dato[TAM_BUFFER], tiempo[TAM_BUFFER],  resto[TAM_BUFFER];
        word w_nl = 0, w_patron = 0, w_dato = 0, w_nivel = 0,
             w_tiempo = 0, w_cmd = 0;

        memset(nl, '\0', TAM_BUFFER);
        memset(comando, '\0', TAM_BUFFER);
        memset(patron, '\0', TAM_BUFFER);
        memset(dato, '\0', TAM_BUFFER);
        memset(tiempo, '\0', TAM_BUFFER);
        memset(resto, '\0', TAM_BUFFER);
        nl[0] = comando[0] = patron[0] = dato[0] = tiempo[0] = '0';

        retcode = analizar_numero_linea(linea, nl_esp);
        if (!retcode) {
            if (tiene_comando(linea, CMD_CONTINUE)) {
                retcode = analizar_patron(linea);
                if (!retcode) {
                    retcode = analizar_tiempo(linea, 4);
                    if (!retcode) {
                        sscanf(linea, "%s %s %s %s %[^DELIMITADOR]",
                        nl, comando, patron, tiempo, resto);
                        w_cmd = CMD_CONTINUE_CODE;
                    }
                }
            } else if (tiene_comando(linea, CMD_LAZO)) {
                retcode = analizar_patron(linea);
                if (!retcode) {
                    retcode = analizar_dato(linea);
                    if (!retcode) {
                        retcode = analizar_tiempo(linea, 5);
                        if (!retcode) {
                            sscanf(linea, "%s %s %s %s %s %[^DELIMITADOR]",
                            nl, comando, patron, dato, tiempo, resto);
                            w_cmd = CMD_LAZO_CODE;
                            w_nivel = num_lazo;
                        }
                    }
                }
            } else if (tiene_comando(linea, CMD_RETL)) {
                retcode = analizar_patron(linea);
                if (!retcode) {
                    retcode = analizar_dato(linea);
                    if (!retcode) {
                        retcode = analizar_tiempo(linea, 5);
                        if (!retcode) {
                            sscanf(linea, "%s %s %s %s %s %[^DELIMITADOR]",
                            nl, comando, patron, dato, tiempo, resto);
                            w_cmd = CMD_RETL_CODE;
                            w_nivel = num_lazo - 1;
                        }
                    }
                }
            } else if (tiene_comando(linea, CMD_FIN)) {
                retcode = analizar_patron(linea);
                if (!retcode) {
                    sscanf(linea, "%s %s %s %[^DELIMITADOR]",
                        nl, comando, patron, resto);
                }
                w_cmd = CMD_FIN_CODE;
            } else {
                retcode = -ENOCOMANDO;
            }
            if (!retcode) {
                if (resto[0] != '\0') {
                    retcode = -ESINTAXIS;
                } else {
                    w_nl = a_word(nl);
                    w_patron = a_word(patron);
                    w_dato = a_word(dato);
                    w_tiempo = a_word(tiempo);
                    (*inst).cargar_inst(w_nl, w_patron, w_dato,
                                        w_nivel, w_cmd, w_tiempo);
                }
            }
        }
    return retcode;
}

void pp2::reset_micro(void) {}
void pp2::modo_carga(void) {}
void pp2::modo_micro(void) {}
void pp2::inic_secuencia(void) {}

int pp2::compilar(char *nombre_archivo, int *nl, char **e_linea) {
    int  retcode = 0;
    word nl_esp = 0;
    word num_lazos = 0;
    char *ptr = NULL;
    char *codigo = NULL;
    char linea[TAM_BUFFER];
    FILE *archivo = NULL;
    byte cmd = 0x00;
    pp2_inst instruccion, inst_a;
    stack<pp2_inst> pila;

    codigo = static_cast<char *>(malloc(sizeof(*codigo)));
    codigo[0] = '\0';
    archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        retcode = -ENOARCHIVO;
    } else {
        while (fgets(linea, TAM_BUFFER, archivo) != NULL) {
            codigo = static_cast<char *>(realloc(codigo,
                       (strlen(codigo) + strlen(linea) + 2) * sizeof(*codigo)));
            codigo = strncat(codigo, linea, strlen(linea));
        }
        fclose(archivo);
        ptr = codigo;
    }

    *nl = 0;
    programa.clear();
    while (!retcode && strchr(ptr, DELIMITADOR) != NULL) {
        sscanf(ptr, "%[^;];", linea);
        ptr = (strchr(ptr, DELIMITADOR)) + 1;
        if (strcmp(linea, "") != 0) {
            retcode = comando(linea, nl_esp, num_lazos, &instruccion);
            if (!retcode) {
                programa.push_back(instruccion);
                nl_esp++;
                cmd = instruccion.val_comando();
                if (cmd == CMD_LAZO_CODE) {
                    num_lazos++;
                    if (num_lazos > MAX_NUM_LAZOS) {
                    retcode = -EMAXLAZOS;
                    } else {
                    pila.push(instruccion);
                    }
                } else if (cmd == CMD_RETL_CODE) {
                    inst_a = pila.top();
                    if ((inst_a.val_comando() == CMD_LAZO_CODE)
                    && (instruccion.val_dato() == inst_a.val_nl())) {
                        pila.pop();
                        num_lazos--;
                    } else {
                        retcode = -ESEMANTICO;
                    }
                }
                if (!retcode) {
                *nl = *nl + 1;
                }
            }
        }
    }
    if (ptr[0] != '\0') {
    retcode = -ESINTAXIS;
    }
    if (!retcode && !pila.empty()) {
    inst_a = pila.top();
    *nl = inst_a.val_nl();
    retcode = -ESEMANTICO;
    }
    strncpy(*e_linea, linea, strlen(linea));
    return retcode;
}


void pp2::transferir_programa(void) {
    int i = 0, j = 0;
    int cant = programa.size();
    byte valor;
    /** Cargamos el programa **/
    for (i = 0; i < cant; i++) {
        // usb.setComando(almacenar);
        // usb.direccion(PP2_INSTRUC);
        usb.SetComando('A');
        usb.direccion(0x51);
        for (j = 0; j < 8 ; j++) {
            valor = programa.front().val_byte(j);
            usb.parametro(valor);
        }
       usb.EnviarDatos_Respuesta();

         // T 52 00
//  usb.EnviarDatos(transf,0x52,0x00);
//  usb.PP2Transferir();
       usb.SetComando('T');  // transferirla a la RAM
       usb.direccion(0x52);
       usb.parametro(0x00);
       usb.EnviarDatos_Respuesta();


        programa.push_back(programa.front());
        programa.pop_front();
    }
}

void pp2::mostrar_programa(void) {
    int i = 0;
        int cant = programa.size();

    for (i = 0; i < cant; i++) {
            printf("%X %X %X %X %X %X %X %x \n",
                    programa.front().val_byte(7),
            programa.front().val_byte(6),
            programa.front().val_byte(5),
            programa.front().val_byte(4),
            programa.front().val_byte(3),
            programa.front().val_byte(2),
            programa.front().val_byte(1),
            programa.front().val_byte(0));
                        programa.push_back(programa.front());
            programa.pop_front();
        }
}




