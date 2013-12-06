#include "phase.h"
#include <assert.h>

        phase::phase(unsigned int i){
            id = i;
        }

        void phase::phase_print (int times){
            /*AGREGAR CODIGO PARA IMPRIMIR*/
        }

        unsigned int phase::phase_id (void){
            return id;
        }

        void phase::phase_add_value(float degrees){
            values.push_back(degrees);
        }

        float phase::phase_next_value(void){
            /*AGREGARO CODIGO PARA DEVOLVER EL SIGUIENTE VALOR*/
            return 0.;
        }

        void phase::phase_set_base_address(unsigned int BaseAddress){
            baseAddr = BaseAddress;
        }

        unsigned int phase::phase_get_mem_address(unsigned int shift){
            unsigned int result = 0, index = 0, len = 0;
            len = values.size();

            assert(len != 0);

	        index = shift % len;
            result = baseAddr + index;

	        return result;
        }

        unsigned int phase::phase_count_values(void){
            return values.size();
        }

        /*REVISAR!!!!!!!!!!!!!!!*/
        unsigned int phase::phase_nth_value(unsigned int n){
            n = n % this->phase_count_values();
            list<float>::iterator i = values.begin();
            advance(i, n);
            return *i;
        }
