#include "dds.h"
#include <stdio.h>

    

unsigned int activate_ram_write (void){

    dds_set_address(DDS_REG_COM1);
    dds_write(0x02);

    return 0;
}


unsigned int desactivate_ram_write (void){

    dds_set_address(DDS_REG_COM1);
    dds_write(0x00);

    return 0;
}



unsigned int load_ram_phase(unsigned char mem_address, unsigned int phase_value){

    unsigned char add = 1, value = 0;
    
    value = phase_value;
    printf("llllllll: %X\n", value);
    
    dds_set_address(RAM_REG_COM1);
    dds_write(mem_address+add);
    dds_set_address(RAM_REG_WRITE);
    dds_write(value);
    dds_set_address(RAM_REG_COM1);
    dds_write(mem_address);
    dds_set_address(RAM_REG_WRITE);
    value = phase_value>>8;
    dds_write(value);

    printf("load_ram_phase value2:%X\n",value);
    
    return 0;
    
}



/* Cambiar a CHAR el campo data de 'lpt_send_byte' */
bool dds_set_address(unsigned char address){
    bool result = true;
    
    result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
    /* Delay() */
    
    if (result)
        result = lpt_send_byte(LPT_DATA, address) == 0;

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x07) == 0;
        /* Delay() */

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x05) == 0;
   
   if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
   
   return result;

}



bool dds_write(unsigned char data){

    bool result = true;

    result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
    /* Delay() */
    
    if (result)
        result = lpt_send_byte(LPT_DATA, data) == 0;

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x0d) == 0;
        /* Delay() */

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x05) == 0;    
    
    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
        /* Delay() */    

    return result;
   
}



bool dds_send_word(unsigned int word){
  unsigned char i = 0,
                x = 0;
  bool result = true;

  for(i = 0; i <= 15 && result; i++){
        x = 1;
        
	if(word & 0x8000) 
	    x = 3;

	result = dds_write(x);

    if (result){
	    x = x & 2;
	    result = dds_write(x);
    }
	
	x = x | 1;
	
	if (result)
	    result = dds_write(x);

	word = word << 1;  /* desplazar a la izquierda un bit */

  }

  if (result) result = dds_write(7);

  return result;

}

