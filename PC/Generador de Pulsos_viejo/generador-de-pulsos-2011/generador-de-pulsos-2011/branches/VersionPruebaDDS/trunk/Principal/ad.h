#ifndef AD_H
#define AD_H

#include "stdbool.h"


#define AD_MODO_MODULADO 0
#define AD_MODO_CONTINUO 1


unsigned int ad_adquirir(unsigned int kpc, unsigned int adqm, unsigned int mps);

#endif

