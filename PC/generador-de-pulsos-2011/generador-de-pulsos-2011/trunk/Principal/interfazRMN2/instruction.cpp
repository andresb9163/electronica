#include "instruction.h"
#include "pp2.h"
#include <iostream.h>


instruction::instruction(unsigned int i, int t, int d){
    id = i;
    type = t;
    data = d;
    duration = 0;
    number = 0;
    phase_shift = 0;
}

void instruction::set_duration(unsigned int d){
    duration = d;
}

unsigned int instruction::get_duration(void){
    return duration;
}

void instruction::std_out_print(void){
    cout << "Instruction:\n" << endl;
    cout << "            id: " << id << endl;

    switch(type){
        case LOOP_INST_CODE:{  cout << "            type: LOOP" << endl;
                    break;}
        case ACQUIRE_INST_CODE:{  cout << "            type: ACQUIERE" << endl;
                    break;}
        case PULSE_INST_CODE:{  cout << "            type: PULSE" << endl;
                    break;}
        case DELAY_INST_CODE:{  cout << "            type: DELAY" << endl;
                    break;}
        case ENDLOOP_INST_CODE:{  cout << "            type: ENDLOOP" << endl;
                    break;}
        case END_INST_CODE:{  cout << "            type: END" << endl;
                    break;}
        default:{   cout << "Error valor de type no valido" << endl;
                }
    }

    cout << "            data: " <<  data << endl;
    cout << "            duration: " << duration << endl;
    cout << "            number: " << number << endl;
    cout << "            phase_shift: " << phase_shift << endl;
    cout << endl;
}

int instruction::get_data(void){
    return data;
}

unsigned int instruction::get_type(void){
    return type;
}

unsigned int instruction::get_id(void){
    return id;
}

bool instruction::is_phase_shifted(void){
    return (data < 0);
}

void instruction::phase_add_shift(int shift){
    phase_shift += shift;
}

void instruction::set_phase_shift(bool shifted){
    if(shifted){
		if(data >= 0){
			data = (data)*(-1);
		}
	}else if(data < 0){
		data = (data)*(-1);
	}
}

int instruction::phase_get_shift(void){
    return phase_shift;
}

unsigned int instruction::get_number(void){
    return number;
}

void instruction::set_number(unsigned int n){
    number = n;
}

instruction *instruction::clone(void){
    instruction *inst = new instruction(id, type, data);
    return inst;
}

void instruction::set_data(unsigned int n){
    data = n;
}

unsigned int instruction::get_instruction_code(void){
    unsigned int result = 0;

    switch(type){
		case LOOP_INST_CODE:{ result = LAZO_PP2_CODE;
				break;}
		case ACQUIRE_INST_CODE:{result = 0x00;
								/*En este caso no interesa la instruccion ya /
								 * que se debe disparar el conversor AD*/
				break;}
		case PULSE_INST_CODE:{result = CONTINUE_PP2_CODE;
				break;}
		case DELAY_INST_CODE:{result = CONTINUE_PP2_CODE;
				break;}
		case ENDLOOP_INST_CODE:{result = RETL_PP2_CODE;
				break;}
		case END_INST_CODE:{result = FIN_PP2_CODE;
				break;}
	}
    return  result;
}
