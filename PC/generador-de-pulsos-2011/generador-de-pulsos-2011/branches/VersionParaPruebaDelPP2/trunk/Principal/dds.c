#include "dds.h"
#include <stdio.h>
#include <assert.h>
#include "lpt_windows.h"
#include "f_glist.h"

unsigned int dds_load_ram_phase(unsigned char mem_address, float phase_value);    

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



unsigned int dds_load_ram_phase(unsigned char mem_address, float phase_value){

    unsigned char add = 1, value = 0;
    int cos = 0;
    
    cos = (360*45.508);
    phase_value = 45.508 * phase_value;
    value = ((int)phase_value) % cos;           
   
    
    printf("LSB del valor de la fase: %X\n", (int)value/45.508);

//printf("DEBUG:                Dir: %X  phase %X:\n", mem_address, phase_value);
//printf("DEBUG:                LSB: %X  MSB %X:\n", (unsigned char)phase_value, phase_value>>8);
    
    direccion(RAM_REG_COM1);
    escritura(mem_address);
    direccion(RAM_REG_WRITE);       //LSB
    escritura(phase_value);
    direccion(RAM_REG_COM1);
    escritura(mem_address+add);
    direccion(RAM_REG_WRITE);
    value = (int)phase_value>>8; 
    escritura(value);              //MSB
    

    printf("MSB del valor de la fase: %X\n",value/45.508);
    
    return 0;
    
}



/******************************************************************************/


bool dds_set_address(unsigned char address){
    bool result = true;
    
    result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
    /* Delay() */
    
    if (result)
        result = lpt_send_byte(LPT_DATA, address) == 0;

    delayN(40);
    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x05);  

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x07) == 0;
        /* Delay() */

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x05) == 0;
   
   if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
   
   return result;

}


/******************************************************************************/


bool dds_write(unsigned char data){

    bool result = true;

    result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
    /* Delay() */
    
    if (result)
        result = lpt_send_byte(LPT_DATA, data) == 0;

    delayN(40);
    if (!result)
        result = lpt_send_byte(LPT_CONTROL, 0x05);  

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


/******************************************************************************/


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


/******************************************************************************/


void dds_set_freq(long double frec_1, long double frec_2){
     long double calculo = 0;
     unsigned int w1_0, w1_1, w1_2, w1_3, w1_4, w1_5, cuenta;
     unsigned int w2_0, w2_1, w2_2, w2_3, w2_4, w2_5;
     long int h;
     
 
   // la ecuación para obtener el valor del registro de frecuencia
  // para una frecuencia deseada es:
  //
  //    valor = frec * 2^48 /  fMCLK.     fMCLK =  300MHZ ó 200MHZ

  // obtener los seis bytes a cargar en el registro de frecuencia

 // calculo =  frec_1 * 281474976710656 / 300000000;

   calculo =  frec_1 * 281474976710656 / 200000000;

   w1_5 = calculo / 1099511627776;  // divide por 2^40
   calculo = calculo - (w1_5 * 1099511627776 );

   w1_4 = calculo / 4294967296; // divide por 2^32
   calculo = calculo - (w1_4 * 4294967296 );

   w1_3 = calculo / 16777216; // divide por 2^24
   calculo = calculo - ( w1_3 * 16777216 );

   w1_2 = calculo / 65536; // divide por 2^16
   calculo = calculo - ( w1_2 * 65536 );

   w1_1 = calculo / 256; // divide por 2^8
   w1_0 = calculo - ( w1_1 * 256 );


 /************* transferir la frecuencia 1 al dds **************/

   direccion(0x71); // direcionar registro de comando
    escritura(0x00); //modo PC y cargar 6 registros de frec. del DDS

  // direccionar los registros de frecuencias y cargar con FTW1


    direccion(0x75); //direccionar registro 04 del DDS
     escritura(0x04);
      direccion(0x78); // cargar dato en FTW1_00
        escritura(w1_5);

      for(h=0; h<400000; h++);

     direccion(0x75); //direccionar registro 05 del DDS
     escritura(0x05);
      direccion(0x78); // cargar dato en FTW1_00
        escritura(w1_4);

      for(h=0; h<400000; h++);

     direccion(0x75); //direccionar registro 06 del DDS
     escritura(0x06);
      direccion(0x78); // cargar dato en FTW1_00
        escritura(w1_3);

      for(h=0; h<400000; h++);

    direccion(0x75); //direccionar registro 07 del DDS
     escritura(0x07);
      direccion(0x78); // cargar dato en FTW1_00
        escritura(w1_2);

       for(h=0; h<400000; h++);

    direccion(0x75); //direccionar registro 08 del DDS
     escritura(0x08);
      direccion(0x78); // cargar dato en FTW1_00
        escritura(w1_1);

     for(h=0; h<400000; h++);

    direccion(0x75); //direccionar registro 09 del DDS
     escritura(0x09);
      direccion(0x78); // cargar dato en FTW1_00
        escritura(w1_0);

   // actualizar los registros internos

  direccion(0x76); // direcionar registro de comando  UDCLK
   escritura(0x00);


  // la ecuación para obtener el valor del registro de frecuencia
  // para una frecuencia deseada es:
  //
  //    frec * 2^48 /  fMCLK.     fMCLK =  50MHZ

  // obtener los seis bytes a cargar en el registro de frecuencia

   // calculo =  frec_2 * 281474976710656 / 300000000;

   calculo =  frec_2 * 281474976710656 / 200000000;

   w2_5 = calculo / 1099511627776;  // divide por 2^40
   calculo = calculo - (w2_5 * 1099511627776 );

   w2_4 = calculo / 4294967296; // divide por 2^32
   calculo = calculo - (w2_4 * 4294967296 );

   w2_3 = calculo / 16777216; // divide por 2^24
   calculo = calculo - ( w2_3 * 16777216 );

   w2_2 = calculo / 65536; // divide por 2^16
   calculo = calculo - ( w2_2 * 65536 );

   w2_1 = calculo / 256; // divide por 2^8
   w2_0 = calculo - ( w2_1 * 256 );


 /************* transferir la frecuencia 2 al dds **************/

    direccion(0x71); // direcionar registro de comando
    escritura(0x00); //modo PC

  // direccionar los registros de frecuencias y cargar con FTW1


   direccion(0x75); // direcionar registro FTW2_05
    escritura(0x0a);
     direccion(0x78); // cargar dato en FTW2_05
        escritura(w2_5);

   direccion(0x75); // direcionar registro FTW2_04
    escritura(0x0b);
     direccion(0x78); // cargar dato en FTW2_04
        escritura(w2_4);

   direccion(0x75); // direcionar registro FTW2_03
    escritura(0x0c);
     direccion(0x78); // cargar dato en FTW2_03
        escritura(w2_3);

   direccion(0x75); // direcionar registro FTW2_02
    escritura(0x0d);
     direccion(0x78); // cargar dato en FTW2_02
        escritura(w2_2);

   direccion(0x75); // direcionar registro FTW2_01
    escritura(0x0e);
     direccion(0x78); // cargar dato en FTW2_01
        escritura(w2_1);

   direccion(0x75); // direcionar registro FTW2_00
    escritura(0x0f);
     direccion(0x78); // cargar dato en FTW2_00
        escritura(w2_0);

  // actualizar los registros internos

   direccion(0x76); // direcionar registro de comando  UDCLK
    escritura(0x00);


   
}

void dds_config(){
     
     long int t;
     
          
     // Seteamos valores por Default al LPT
     lpt_send_byte(LPT_CONTROL, 0x04);
     lpt_send_byte(LPT_DATA, 0x00);

     // resetear y desactivar el DDS

     direccion(0x71); // direcionar registro de comando
     escritura(0x00); //modo PC
     // Master RESET

     direccion(0x72);
     escritura(0x00);

    for(t=0; t<40000000; t++);

 // configurar el DDS

  direccion(0x75); //direccionar registro 1f
   escritura(0x1f);
    direccion(0x78);
     escritura(0x02);    // modo FSK, UDCLK externo
     //escritura(0x03);    // modo FSK, UDCLK interno


  direccion(0x75); //direccionar registro 1d del DDS
   escritura(0x1d);
    direccion(0x78);
      escritura(0x17); // 00010111, todo en PowerDown


  direccion(0x75); //direccionar registro 1e
   escritura(0x1e);
    direccion(0x78);
   //   escritura(0x46);  //PLL out. 0x44 = 200Mhz, 0x45 = 250Mhz, 0x46 = 300Mhz
        escritura(0x44);  //PLL out. 0x44 = 200Mhz, 0x45 = 250Mhz, 0x46 = 300Mhz

  direccion(0x75); //direccionar registro 20
   escritura(0x20);
    direccion(0x78);
     escritura(0x00);    // Shape keying deshabilitado
                          // filtro deshabilitado     
}





void dds_enable(){
     // inicializar los registros de control


  direccion(0x71); // direcionar registro de comando
     escritura(0x00); //modo PC



   direccion(0x75); //direccionar registro 1d del DDS
     escritura(0x1d);
      direccion(0x78);
        escritura(0x10);

     // activar el cargador de fase del ALTERA

         direccion(0x71); // direcionar registro de comando
          escritura(0x05); //modo DDS con fases     
}

void dds_close(){
     direccion(0x71); // direcionar registro de comando
     escritura(0x00); //modo PC

   direccion(0x75); //direccionar registro 1d del DDS
     escritura(0x1d);
      direccion(0x78);
        escritura(0x17); //todo el DDS en power down
}


void dds_disable(){
     
     direccion(0x71); // direcionar registro de comando
     escritura(0x00); //modo PC

   direccion(0x75); //direccionar registro 1d del DDS
     escritura(0x1d);
      direccion(0x78);
        escritura(0x17); //todo el DDS en power down

         // actualizar los registros internos

   direccion(0x76); // direcionar registro de comando  UDCLK
    escritura(0x00);
}




void dds_reset(){
     long int t = 0;
     
     
     // Seteamos valores por Default al LPT
     lpt_send_byte(LPT_CONTROL, 0x04);
     lpt_send_byte(LPT_DATA, 0x00);

    // resetear y desactivar el DDS

  direccion(0x71); // direcionar registro de comando
    escritura(0x00); //modo PC
 // Master RESET

   direccion(0x72);
   escritura(0x00);

    for(t=0; t<40000000; t++);

 // configurar el DDS

  direccion(0x75); //direccionar registro 1f
   escritura(0x1f);
    direccion(0x78);
    // escritura(0x03);    // modo FSK, UDCLK interno
        escritura(0x02);    // modo FSK, UDCLK externo

  direccion(0x75); //direccionar registro 1d del DDS
   escritura(0x1d);
    direccion(0x78);
      escritura(0x17); // 00010111, todo en PowerDown


  direccion(0x75); //direccionar registro 1e
   escritura(0x1e);
    direccion(0x78);
    // escritura(0x46);  //PLL out. 0x44 = 200Mhz, 0x45 = 250Mhz, 0x46 = 300Mhz
        escritura(0x44);  //PLL out. 0x44 = 200Mhz, 0x45 = 250Mhz, 0x46 = 300Mhz

  direccion(0x75); //direccionar registro 20
   escritura(0x20);
    direccion(0x78);
     escritura(0x00);    // Shape keying deshabilitado
                          // filtro deshabilitado
}

bool dds_load_phases_ram(instruction_sheet inst_sheet, unsigned int shift){
        unsigned int count_phases = 0,
                     count_phases_value = 0,
                     n = 0,
                     m = 0,
                     phase_value = 0,
                     next_address = 0;             
        phase p = NULL;
        bool result = true;
        f_glist phases = f_glist_create ();
        float f = 0;
        int i = 0, j = 0;

        assert(inst_sheet != NULL);
        
        count_phases = instruction_sheet_phase_count(inst_sheet);

        /* Primero armo la lista con todas los valores de fase que poseo: */
        for(i = 0; i < count_phases; i++){
            p = instruction_sheet_get_nth_phase(inst_sheet, n);
            for(j = 0; j < phase_count_values(p); j++){
                f = phase_nth_value(p,j);
                if(f_glist_find(phases, f) < 0)    
                    f_glist_add(phases, f);
            }
        }

        /* Ahora guardo cada uno de esos valores en la RAM: */

        for(i = 0; i < f_glist_length(phases); i++){
            dds_load_ram_phase(next_address, f_glist_nth(phases, i));
            next_address += 2;
        }

        /* Por último actualizamos cada phase con la posicion de sus valores 
        /* en la ram: */

        for(i = 0; i < count_phases; i++){
            p = instruction_sheet_get_nth_phase(inst_sheet, n);
            for(j = 0; j < phase_count_values(p); j++){
                f = phase_nth_value(p,j);
                assert( -1 != f_glist_find(phases,f));
                phase_set_address(p,j,2*f_glist_find(phases,f));
            }

        


        if (count_phases > RAM_SPACE_SIZE) result = false;
        else{
            activate_ram_write();          //Modo escritura de RAM  
            next_base_address = 0;
            for(n = 0; n < count_phases; n++){
                p = instruction_sheet_get_nth_phase(inst_sheet, n);
                assert(p != NULL);
                
                phase_set_base_address(p, next_base_address);
                /*printf("Direccion Base de la phase %i con shift %u es: %i\n",phase_id(p), shift, next_base_address);*/
                count_phases_value = phase_count_values(p);
                for(m = 0; m < count_phases_value; m++){
                    phase_value = phase_nth_value(p, m + shift);
                                                           
                    dds_load_ram_phase(next_base_address, phase_value);                           
                                        
                    next_base_address += 2;    
                }
            }
            
            desactivate_ram_write();       //Modo PC
            /*Habitlitar lectura de las fases desde la RAM*/
           // direccion(0x71);
           // escritura(0x04);
            
            direccion(0x71); // direcionar registro de comando
            escritura(0x05); //modo DDS con transferencia  de fases

        }
    return result;
}
