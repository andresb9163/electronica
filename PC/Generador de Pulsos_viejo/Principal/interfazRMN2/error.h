#ifndef _ERROR_H_
#define _ERROR_H_

#define NO_ERROR 0
#define ERROR_TOO_MANY_SAMPLES 1
#define ERROR_NO_PULSE_SHEET_PATH 2
#define ERROR_DDS_FREQ_1 3
#define ERROR_DDS_FREQ_2 4
#define ERROR_NO_NUMBER_OF_SAMPLES 5
#define ERROR_NO_AD_BUFFER_SIZE 6
#define ERROR_NO_AD_MODE 7
#define ERROR_NO_CONFIG_PATH 8
#define ERROR_NO_SAVE_FILE_PATH 9
#define ERROR_NO_MPS 10
#define ERROR_DELAYS_MISSING 11

#define MERROR_TOO_MANY_SAMPLES "Demasiadas repeticiones para el experimento"
#define MERROR_NO_PULSE_SHEET_PATH "No hay ruta para la hoja de pulsos"
#define MERROR_DDS_FREQ_1 "No hay valor asignado a la frecuencia 1 del DDS"
#define MERROR_DDS_FREQ_2 "No hay valor asignado a la frecuencia 2 del DDS"
#define MERROR_NO_NUMBER_OF_SAMPLES "No hay valor asignado a la cantidad de experimentos"
#define MERROR_NO_AD_BUFFER_SIZE "No hay valor asignado para el tamaño de buffer de adquisicion"
#define MERROR_NO_AD_MODE "No hay valor asignado para el modo de operacion del AD"
#define MERROR_NO_CONFIG_PATH "No hay ruta para el archivo de configuracion"
#define MERROR_NO_SAVE_FILE_PATH "No hay ruta para guardar los resultados de las adquisiciones"
#define MERROR_NO_MPS "No hay valor asignado a las muestras por segundo del conversor AD"
#define MERROR_DELAYS_MISSING "Faltan asignar valores de delay a alguna/s instruccion/es"


#define MDEFAULT "Error desconocido"

char *getMessage(unsigned int error_code);

#endif