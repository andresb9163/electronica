#ifndef AD_H
#define AD_H




#define AD_MODO_MODULADO 0
#define AD_MODO_CONTINUO 1
#define AD_HABILITADO 0
#define AD_DESHABILITADO 1
#define AD_RESETEADO 2

typedef struct t_ad * ad_t;


ad_t ad_create(void);

ad_t ad_destroy(ad_t ad);

ad_t ad_config (unsigned int mps, unsigned int kpc, unsigned int adqm);

unsigned int ad_adquirir(ad_t ad);

void ad_to_file(ad_t ad, char *output_file);    

#endif

