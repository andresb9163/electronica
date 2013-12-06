#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define LOOP_INST_CODE 0
#define ACQUIRE_INST_CODE 1
#define PULSE_INST_CODE 2
#define DELAY_INST_CODE 3
#define ENDLOOP_INST_CODE 4
#define END_INST_CODE 5

class instruction{
    private:
            unsigned int id;
            unsigned int type;
            int data;
            unsigned int duration;
            unsigned int number;
            unsigned int phase_shift;

    public:
            instruction(unsigned int i, int t, int d);

            void set_duration(unsigned int d);

            unsigned int get_duration(void);

            void std_out_print(void);

            int get_data(void);

            unsigned int get_type(void);

            unsigned int get_id(void);

            bool is_phase_shifted(void);

            void phase_add_shift(int shift);

            void set_phase_shift(bool shifted);

            int phase_get_shift(void);

            unsigned int get_number(void);

            void set_number(unsigned int n);

            instruction *clone(void);

            void set_data(unsigned int n);

            unsigned int get_instruction_code(void);
};

#endif
