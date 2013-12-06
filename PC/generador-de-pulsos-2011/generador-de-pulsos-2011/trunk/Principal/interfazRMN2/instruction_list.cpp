#include "instruction_list.h"

instruction_list::instruction_list(void){
    count = 0;
}

void instruction_list::add (instruction *i){
    lista.push_back(i);
    count++;
}

unsigned int instruction_list::length (void){
	return count;
}

void instruction_list::std_out_print (unsigned int times){
    int i = 0, j = 0;
    for(j=0;j<times;j++){
        for(i=0;i<this->length();i++){
            this->nth_item(i)->std_out_print();
        }
    }
}

instruction* instruction_list::nth_item(unsigned int n){
    list<instruction*>::iterator i = lista.begin();
    advance(i, n);
	return *i;
}


list<instruction*> instruction_list::get_list(void){
	return lista;
}
 