#include "ad.h"
#include "lpt_windows.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
                   
bool ad_leer_buffers(unsigned int kpc);

bool ad_reset_modo_pc (void);

void ad_to_file(short *canala, short *canalb, unsigned int kpc);    

bool ad_config_muestrasps (unsigned int mps);     

/******************************************************************************/



bool ad_config_muestrasps (unsigned int mps){

    bool result = false;  
    unsigned char N = 0;

    /* Modo PC para configurarlo */
    direccion(0x0b);
    escritura(0x00);
    
    /* Configuracion de la cantidad de muestras por segundo. */

    if(mps < 10000000 && mps > 39215){
        N = 10000000/mps;
        result = direccion(0x0c);
        if(!result)
            result = escritura(255-N);
        if(!result)
            result = direccion(0x0d);
        if(!result)
            result = escritura(0x00);
    }else {
        printf("Error en la cantidad de muestras por segundo\n");        
        result = 1;
    }
        
    return result;
}



/******************************************************************************/

unsigned int ad_adquirir(unsigned int kpc, unsigned int adqm, unsigned int mps){

    unsigned int b = 0, result = 0;
    unsigned char conf = 0x00;
        
    assert(kpc==1 || kpc==2 || kpc==4 || kpc==8 || kpc==16 || kpc==32 || kpc==64);
        
    /* modo continuo -> b1 = 1. Modo modulado -> b1 = 0 */
    if(adqm == AD_MODO_CONTINUO)
        conf = conf | 0x02;        
        
    /* Kpc = tamano del buffer de adquisicion en Kb: */    
    if(kpc==64){
            conf=conf|0x70;
    }else if(kpc==32){
            conf=conf|0x60;
    }else if(kpc==16){
            conf=conf|0x50;
    }else if(kpc==8){
            conf=conf|0x40;
    }else if(kpc==4){
            conf=conf|0x30;
    }else if(kpc==2){
            conf=conf|0x20;
    }else if(kpc==1){
            conf=conf|0x10;
    }else{
        result = 1;
    }
    
    if(!result)
        result = direccion(0x0b);   
    if(!result)
        result = escritura(conf);
      
    /* Configaracion de las muestras por segundo */
    ad_config_muestrasps(mps);    
        
    /*Listo la configuracion, nos ponemos en espera del pulso para adquirir*/
    if(!result){
        /* Reset antes de adquirir */
        direccion(0x0b);
        escritura(0x80|conf);
        escritura(0x00|conf);
    }
    if(!result)          
        result = direccion(0x0b);
    if (!result)
        result = startLeer();    
    if (!result){
        do{
            b=leer();
            /*printf("leyo b = %u\n",b);*/
        }while(!(b&0x01));
        result = endLeer();
     }
                             
     /*OBTENGO EL RESULTADO*/
       
    if(!result)
        result = ad_leer_buffers(kpc);

    return result;
}



/******************************************************************************/


bool ad_leer_buffers(unsigned int kpc){
    
    unsigned int i = 0, result = 0;
    char *canal_a = NULL, *canal_b = NULL, *canal_ab = NULL,  aux = 0;
    short *canala = NULL, *canalb = NULL;
    
    canal_a = calloc(1, kpc*1024);
    canal_b = calloc(1, kpc*1024);
    canal_ab = calloc(1, kpc*1024);    
    canala = calloc(1, sizeof(short)*kpc*1024);
    canalb = calloc(1, sizeof(short)*kpc*1024);  
  
    ad_reset_modo_pc();

    direccion(0x08);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(kpc*1024); i++)
        canal_b[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
    
    ad_reset_modo_pc();
    
    direccion(0x09);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(kpc*1024); i++)
        canal_ab[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
        
    ad_reset_modo_pc();
    
    direccion(0x0a);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(kpc*1024); i++)
        canal_a[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
    
   
    /* Construyo el resultado final A ab B -> Aa bB*/

    for(i = 0; i<(kpc*1024);i++){
        canala[i] = canal_a[i];
        canala[i] = canala[i]<<4;
        aux = canal_ab[i]>>4;
        canala[i] = canala[i] | aux;
    }

    for(i = 0; i<(kpc*1024);i++){
        canalb[i] = canal_b[i];
        canalb[i] = canalb[i]<<4;
        aux = canal_ab[i]&0x0f;
        /*printf("AUX------- %X\n",aux);*/
        canalb[i] = canalb[i] | aux;
    }

    free(canal_a);
    free(canal_b);
    free(canal_ab);

    ad_to_file(canala, canalb, kpc); 
    free(canala);  
    free(canalb);

    return result;
}



/******************************************************************************/


/*  Reset para poder adquiriro leer el buffer*/
bool ad_reset_modo_pc (void){

    bool result = false;

    result = direccion(0x0b);
    if(result)
        result = escritura(0x80);
    if(result)
        result = escritura(0x00);

    return result;
}



/******************************************************************************/


void ad_to_file(short *canala, short *canalb, unsigned int kpc){

    FILE *output_ad = NULL;
    int i = 0;

    output_ad = fopen("output_ad.txt", "a");
    assert(canala != NULL && canalb != NULL);
    for(i = 0; i < kpc*1024; i++){
          fprintf(output_ad, "canala %u\tcanalb %u\n" , canala[i], canalb[i]);
    }

    fprintf(output_ad, "\n\t\t<< Fin de la muestra >>\n");
    
    fclose(output_ad);
}
