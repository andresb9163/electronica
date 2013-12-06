#include "ad.h"
#include "lpt_windows.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


struct t_ad{
    unsigned char conf; //configuracion actual del ad
    unsigned int modo;  //modulado o continuo
    unsigned int kpc;   //Kbytes de adquisicion por ciclo
    unsigned int *canala;
    unsigned int *canalb;
    unsigned int adq_count;
};


/* FUNCIONES INTERNAS*/
                 
void ad_resetear_contador(ad_t ad);

void ad_set_modo_pc(ad_t ad);

void ad_set_modo_ad(ad_t ad);

bool ad_leer_buffers(ad_t ad);
 

/******************************************************************************/

ad_t ad_create(void){
    
    ad_t ad = NULL;

    ad = calloc(1,sizeof(struct t_ad));
    assert(ad!=NULL);
    ad->conf = 0x00;
    ad->modo = AD_MODO_MODULADO;
    ad->adq_count = 0;
    return ad;
}


ad_t ad_destroy(ad_t ad){
    return NULL;
}


ad_t ad_config (unsigned int mps, unsigned int kpc, unsigned int adqm){

    bool result = false;  
    unsigned char N = 0;
    ad_t ad = NULL;

    ad = ad_create();
    assert(ad!=NULL);

    /* Modo PC para configurarlo */
    ad_set_modo_pc(ad);
    
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
        return NULL;
    }
        
    assert(kpc==1 || kpc==2 || kpc==4 || kpc==8 || kpc==16 || kpc==32 || kpc==64);
        
    /* modo continuo -> b1 = 1. Modo modulado -> b1 = 0 */
    if(adqm == AD_MODO_CONTINUO){
        ad->conf = ad->conf | 0x02;
        ad->modo = AD_MODO_CONTINUO;
    }        
        
    /* Kpc = tamano del buffer de adquisicion en Kb: */    
    if(kpc==64){
            ad->conf = ad->conf|0x70;
    }else if(kpc==32){
            ad->conf = ad->conf|0x60;
    }else if(kpc==16){
            ad->conf = ad->conf|0x50;
    }else if(kpc==8){
            ad->conf = ad->conf|0x40;
    }else if(kpc==4){
            ad->conf = ad->conf|0x30;
    }else if(kpc==2){
            ad->conf = ad->conf|0x20;
    }else if(kpc==1){
            ad->conf = ad->conf|0x10;
    }else if(kpc != 0){
        result = 1;
    }
    
    ad->canala = calloc(kpc*1024, sizeof(int));
    ad->canalb = calloc(kpc*1024, sizeof(int));    
    ad->kpc = kpc;

    if(!result)
        result = direccion(0x0b);   
    if(!result){
        result = escritura(ad->conf);
    } 
    return ad;
}

/******************************************************************************/


void ad_resetear_contador(ad_t ad){

    assert(ad != NULL);
    direccion(0x0b);
    escritura(ad->conf | 0x80);
    ad->conf = ad->conf&0x7F;
    escritura(ad->conf);

}

/******************************************************************************/

void ad_set_modo_pc(ad_t ad){

    assert(ad!= NULL);

    ad->conf = ad->conf & 0xFE;
    direccion(0x0b);
    escritura(ad->conf);

}

/******************************************************************************/

void ad_set_modo_ad(ad_t ad){

    assert(ad!= NULL);

    ad->conf = ad->conf | 0x01;
    direccion(0x0b);
    escritura(ad->conf);

}

/******************************************************************************/

unsigned int ad_adquirir(ad_t ad){
    unsigned int result = 0;
    unsigned int b = 0;

    //printf("ENTRA\n");

    //RERET Y MODO AD:
    ad_resetear_contador(ad);
    ad_set_modo_ad(ad);
    
    if(!result)          
        result = direccion(0x0b);
    if (!result)
        result = startLeer();    
    if (!result){
        do{
            //printf("ENTRA\n");
            b=leer();
        }while(!(b&0x01));
        result = endLeer();
        printf("leyo b = %u\n",b);
     }
    ad->adq_count++;    
          
    ad_leer_buffers(ad);
                      
    return result;
}



/******************************************************************************/


bool ad_leer_buffers(ad_t ad){
    
    unsigned int i = 0, result = 0, punto = 0, h = 0;
    unsigned char *canal_a = NULL, *canal_b = NULL, *canal_ab = NULL,  aux = 0;
    
    canal_a = malloc(sizeof(unsigned char)*ad->kpc*1024);
    canal_b = malloc(sizeof(unsigned char)*ad->kpc*1024);
    canal_ab = malloc(sizeof(unsigned char)*ad->kpc*1024);
  
    ad_resetear_contador(ad);
    ad_set_modo_pc(ad);

    direccion(0x08);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(ad->kpc*1024); i++)
        canal_b[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
    
    ad_resetear_contador(ad);
    ad_set_modo_pc(ad);

    direccion(0x09);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(ad->kpc*1024); i++)
        canal_ab[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
        
    ad_resetear_contador(ad);
    ad_set_modo_pc(ad);
    
    direccion(0x0a);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(ad->kpc*1024); i++)
        canal_a[i] = leer();  /* leeyendo los datos */

    endLeer();                                  /* bus de datos salida */
   
    ad_resetear_contador(ad);

    /* Construyo el resultado final A ab B -> Aa bB*/

    for(i = 0; i<(ad->kpc*1024);i++){
        punto = 0;
        punto = (canal_ab[i]/16)&0x0f;
        punto= punto+canal_a[i]*16;
        ad->canala[i]= punto;
        /*ad->canala[i] = 0;
        ad->canala[i] = (canal_ab[i]/16)&0x0f;
        ad->canala[i] += canal_a[i]*16;*/
    }

    for(i = 0; i<(ad->kpc*1024);i++){
        punto= canal_ab[i]&0x0f;
        punto= punto+canal_b[i]*16;
        ad->canalb[i]=punto;
        /*ad->canalb[i] = 0;
        ad->canalb[i] = canal_b[i];
        ad->canalb[i] = ad->canalb[i]<<4;
        aux = canal_ab[i]&0x0f;
        ad->canalb[i] = ad->canalb[i] | aux;*/
    }

    free(canal_a);
    free(canal_b);
    free(canal_ab);

    return result;
}




/******************************************************************************/


void ad_to_file(ad_t ad, char *output_file){

    FILE *output_ad = NULL;
    int i = 0;
    char adq_count[4];
    char file[150];
    char dateTime[200];
    char canalabuff[32] = {0};
    char canalbbuff[32] = {0};    
    

    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    sprintf(adq_count, "%d", ad->adq_count);

    file[0] = '\0';
    strcat(file,"adoutput/");
    if(output_file != NULL){
        assert(strlen(output_file)<100);
        strcat(file, output_file);
    }else{
        strcat(file,"ad_");
        strftime(dateTime,128,"%d-%m-%y_h%Hm%Ms%S_", tlocal);
        strcat(file,dateTime);
        strcat(file,"output");
    //    printf("%s\n",file);
    }    
    strcat(file,adq_count);
    strcat(file,".txt");

    output_ad = fopen(file, "w");
    assert(ad->canala != NULL && ad->canalb != NULL);
    for(i = 0; i < ad->kpc*1024; i++){
          fprintf(output_ad, "%i\t%i\n" , ad->canala[i], ad->canala[i]);
    }

    fprintf(output_ad, "\n\t\t<< Fin de la muestra >>\n");
    
    fclose(output_ad);
}
