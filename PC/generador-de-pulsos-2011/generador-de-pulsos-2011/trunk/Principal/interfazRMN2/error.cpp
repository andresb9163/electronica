#include "error.h"
#include <stdlib.h>

char *getMessage(unsigned int error_code){
        char *result = NULL;

        if(error_code == ERROR_TOO_MANY_SAMPLES)
            result =MERROR_TOO_MANY_SAMPLES;
        else if(error_code == ERROR_NO_PULSE_SHEET_PATH)
            result =MERROR_NO_PULSE_SHEET_PATH;
        else if(error_code == ERROR_DDS_FREQ_1)
            result =MERROR_DDS_FREQ_1;
        else if(error_code == ERROR_DDS_FREQ_2)
            result =MERROR_DDS_FREQ_2;
        else if(error_code == ERROR_NO_NUMBER_OF_SAMPLES)
            result =MERROR_NO_NUMBER_OF_SAMPLES;
        else if(error_code == ERROR_NO_AD_BUFFER_SIZE)
            result =MERROR_NO_AD_BUFFER_SIZE;
        else if(error_code == ERROR_NO_AD_MODE)
            result =MERROR_NO_AD_MODE;
        else if(error_code == ERROR_NO_CONFIG_PATH)
            result =MERROR_NO_CONFIG_PATH;
        else if(error_code == ERROR_NO_SAVE_FILE_PATH)
            result =MERROR_NO_SAVE_FILE_PATH;
        else if(error_code == ERROR_NO_MPS)
            result =MERROR_NO_MPS;
        else if(error_code == ERROR_DELAYS_MISSING)
            result =MERROR_DELAYS_MISSING;
        else
            result = MDEFAULT;
        return result;
}

