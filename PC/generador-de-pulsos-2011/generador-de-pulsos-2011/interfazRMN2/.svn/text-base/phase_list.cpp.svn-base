#include "phase_list.h"

phase_list::phase_list(void){
    count = 0;
}

void phase_list::add (phase *i){
    lista.push_back(i);
    count++;
}

unsigned int phase_list::length (void){
	return count;
}

void phase_list::std_out_print (unsigned int times){
    int i = 0, j = 0;
    for(j=0;j<times;j++){
        for(i=0;i<this->length();i++){
            this->nth_item(i)->phase_print(1);
        }
    }
}

phase* phase_list::nth_item(unsigned int n){
    list<phase*>::iterator i = lista.begin();
    advance(i, n);
	return *i;
}


list<phase*> phase_list::get_list(void){
	return lista;
}

