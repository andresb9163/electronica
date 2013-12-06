#ifndef _DDS_H
#define _DDS_H


#include "lpt_windows.h"
#include "instruction_sheet.h"

#define RAM_REG_COM1   0x70
#define RAM_REG_WRITE  0x74

#define DDS_REG_COM1   0x71
#define DDS_REG_COM2   0x72
#define DDS_REG_WRITE  0x78
#define DDS_REG_DIR    0x75
#define DDS_REG_TRANSF 0x76
#define DDS_REG_COD    0x77

#define RAM_SPACE_SIZE 16

bool dds_set_address(unsigned char address);
bool dds_write(unsigned char data);
bool dds_send_word(unsigned int word);

void dds_enable();
void dds_disable();
void dds_reset();

void dds_config();
void dds_set_freq(long double frec_1, long double frec_2);
bool dds_load_phases_ram(instruction_sheet inst_sheet, unsigned int shift);
unsigned int dds_activate_ram_write (void);
unsigned int dds_desactivate_ram_write (void);

#endif


