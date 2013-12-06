#ifndef _PP2_H_
#define _PP2_H_
#include "lpt_windows.h" 
/*#include "instruction_sheet.h"*/

/*Direcciones de los registros de trabajo*/

#define COMMAND_REG_ADDR 0x50
#define STATUS_REG_ADDR 0x50
#define INSTRUCTION_REG_ADDR 0x51
#define TRANSFER_REG_ADDR 0x52

/*Patrones de configuracion*/
#define FULL_RESET 0x02
#define CHARGE_MODE 0x03
#define MICRO_MODE 0x00
#define LAUNCH_PULSE_SEQ 0x80

/*Codigos de instruccion*/
#define CONTINUE_PP2_CODE 0x01
#define LAZO_PP2_CODE 0x02
#define RETL_PP2_CODE 0x03
#define FIN_PP2_CODE 0x07

/*Numero de bytes de las instrucciones aceptadas por el pp2 */
#define INST_LENGTH 8
/*Numero de bucles anidados (comenzando desde 0)aceptadas por el pp2 */
#define MAX_LOOP_LEVEL 3

#define NO_ERROR_CODE 0
#define BUSY_ERROR_CODE 1
#define WRONG_INST_ERROR_CODE 2

class pp2{
private:
lpt_windows *lpt;
unsigned int pp2_build_instruction (unsigned char *instruction, 
                                unsigned int pattern, unsigned int data, 
                                int loop_level, unsigned int delay, 
                                unsigned int inst_code);
bool is_working(void);


public:
pp2(void);

unsigned int write_instruction( unsigned int pattern, unsigned int data,
                                    int loop_level, unsigned int delay,
                                    unsigned int inst_code);

unsigned int full_reset(void);

unsigned int charge_mode_enabled(void);

unsigned int transfer_instruction(void);

unsigned int microprocessor_mode_enabled(void);


unsigned int launch_pulse_sequence(void);

/*unsigned int load_program (instruction_sheet is);*/
};
#endif

