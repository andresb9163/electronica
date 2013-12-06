#include "error.h"
#include <stdlib.h>

char *error_mensaje (int codigo) {
	char *mensaje = NULL;   

	switch (codigo) {
        	case -EFREQMAX: mensaje = MEFREQMAX; break;
		case -EFREQMIN: mensaje = MEFREQMIN; break;
                case -EFASEMAX: mensaje = MEFASEMAX; break;
                case -EFASEMIN: mensaje = MEFASEMIN; break;
                case -EPOSINUM: mensaje = MEPOSINUM; break;
                case -EFAILURE: mensaje = MEFAILURE; break;
                case -ENOCOMANDO: mensaje = MENOCOMANDO; break;
                case -ENOVALLINEA: mensaje = MENOVALLINEA; break;
                case -ENONUMLINEA: mensaje = MENONUMLINEA; break;
                case -ENOVALPATRON: mensaje = MENOVALPATRON; break;
                case -ENOPATRON: mensaje = MENOPATRON; break;
                case -ENOVALDATO: mensaje = MENOVALDATO; break;
                case -ENODATO: mensaje = MENODATO; break;
                case -ENOVALTIEMPO: mensaje = MENOVALTIEMPO; break;
                case -ENOTIEMPO: mensaje = MENOTIEMPO; break;
                case -EMAXLAZOS: mensaje = MEMAXLAZOS; break;
		case -ENOARCHIVO: mensaje = MENOARCHIVO; break;
                case -ESINTAXIS: mensaje = MESINTAXIS; break;
                case -ESEMANTICO: mensaje = MESEMANTICO; break;
                default: mensaje = MEDEFAULT;
        }

	return mensaje;
}
