#ifndef PHASE_H
#define PHASE_H

/* Este tipo representa una fase en el programa.
 * Las fases pueden tomar diferentes valores
 * correspondientes a los introducidos con la
 * función 'phase_add'. (se extraen de manera circular).
 * La implementacion esta basada en el modulo de lista
 * circular de flotantes 'f_glist'.
 */
#include <list>

using namespace std;

class phase{

    private:
        unsigned int id;
        list<float> values;
	    unsigned int baseAddr;

    public:
        phase(unsigned int id);

        void phase_print (int times);

        unsigned int phase_id (void);

        void phase_add_value(float degrees);

        float phase_next_value(void);

        void phase_set_base_address(unsigned int BaseAddress);

        unsigned int phase_get_mem_address(unsigned int shift);

        unsigned int phase_count_values(void);

        /*REVISAR!!!!!!!!!!!!!!!*/
        unsigned int phase_nth_value(unsigned int n);
};
#endif

