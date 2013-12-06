#include "ad.h"
#include "lpt_linux.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


/*PROTOTIPOS*/

bool ad_config (unsigned int adqm, unsigned int kpc, unsigned int mps);

bool ad_adquirir(short **canala, short **canalb, unsigned int modo, 
                    unsigned int kpc, unsigned int mps);
                    
bool ad_leer_buffers(short **canala, short **canalb, unsigned int kpc);

bool ad_reset (void);

bool ad_estado(void);                    

/******************************************************************************/



bool ad_config (unsigned int adqm, unsigned int kpc, unsigned int mps){

    bool result = false;
    unsigned char conf = 0;
    unsigned char N = 0;
    
    assert(kpc==1 || kpc==2 || kpc==4 || kpc==8 || kpc==16 || kpc==32 || kpc==64);
    
    /*Lo coloco en modo ad*/
    conf = 0x01;
    
    if(adqm == AD_MODO_CONTINUO)
        conf = conf | 0x02;
        
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
        result = 0;
    }
    
    if(result)
        result = direccion(0x0b);
    
    if(result)
        result = escritura(conf);
    
    
    /*Configuro las muestras por segundo, ¿Puedo hacerlo estando en modo a/d?*/    
    if(mps < 10000000){    
        N = 10000000/mps;
        result = direccion(0x0c);
        if(result)
            result = escritura(255-N);
        if(result)
            result = direccion(0x0d);
        if(result)
            result = escritura(0x00);
    }else 
        result = 0;
        
    return result;
}



/******************************************************************************/

bool ad_adquirir(short **canala, short **canalb, unsigned int modo, unsigned int kpc, unsigned int mps){

        unsigned int b = 0;
        bool result = false;
        
        result = ad_config(modo, kpc, mps);
        if(result){
            ad_reset();
            
            /*ADQUIERO*/
            direccion(0x0b);
            startLeer();
            do{
                    b=leer();
            }while(!(b&0x01));
            endLeer();
            
            /*OBTENGO EL RESULTADO*/
            
            if(result)
                result = direccion(0x0b);
            if(result)
                result = escritura(0x00);                           /*modo pc*/
            if(result)
                result = ad_leer_buffers(canala, canalb, kpc);
        }
            
        return result;
}



/******************************************************************************/


bool ad_leer_buffers(short **canala, short **canalb, unsigned int kpc){
    
    unsigned int i = 0, result = 0;
    char *canal_a = NULL;
    char *canal_b = NULL;
    char *canal_ab = NULL;
    char aux = 0;
    
    canal_a = calloc(1, kpc*1024);
    canal_b = calloc(1, kpc*1024);
    canal_ab = calloc(1, kpc*1024);    
    
    
    *canala = calloc(1, kpc*1024);
    *canalb = calloc(1, kpc*1024);
    
    ad_reset();
    
    direccion(0x08);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(kpc*1024); i++)
        canal_b[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
    
    ad_reset();
    
    direccion(0x09);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(kpc*1024); i++)
        canal_ab[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
        
    ad_reset();
    
    direccion(0x0a);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(kpc*1024); i++)
        canal_a[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
    

    /* Construyo el resultado final A ab B -> Aa bB*/

    for(i = 0; i<(kpc*1024);i++){
        (*canala)[i] = canal_a[i];
        (*canala)[i] = (*canala)[i]<<4;
        aux = canal_ab[i]>>4;
        (*canala)[i] = (*canala)[i] | aux;
    }

    for(i = 0; i<(kpc*1024);i++){
        (*canalb)[i] = canal_b[i];
        (*canalb)[i] = (*canalb)[i]<<4;
        aux = canal_ab[i]&0x0f;
        /*printf("AUX------- %X\n",aux);*/
        (*canalb)[i] = (*canalb)[i] | aux;
    }

    free(canal_a);
    free(canal_b);
    free(canal_ab);

    return result;
}



/******************************************************************************/


/*  Reset para poder adquiriro leer el buffer*/
bool ad_reset (void){

    bool result = false;

    result = direccion(0x0b);
    
    if(result)
        result = escritura(0x80);
    if(result)
        result = escritura(0x00);

    return result;
}



/******************************************************************************/


/*  True indica que se ha finalizado la adquicision o lectura del buffer.
    Debe estar en modo A/D para poder usar esta funcion.*/
bool ad_estado(void){

    bool result = false;
    
    result = direccion(0x0b);
    
    if(result)
        result = ((0x01 & leer()) == 1);

    return result;
}


