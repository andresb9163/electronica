#ifndef _LPT_H_
#define _LPT_H_

#define LPT1 0x378
#define LPT2 0x278

#define LPT_BASE    LPT1
#define LPT_DATA    LPT_BASE + 0
#define LPT_STATUS  LPT_BASE + 1
#define LPT_CONTROL LPT_BASE + 2

class lpt_windows{

private:
    void delayN(unsigned int n);
    void delay4(unsigned int n);

public:
    lpt_windows(void);
    
    unsigned int send_byte(unsigned int port_addr, unsigned int data);

    unsigned int recive_byte(unsigned int port_addr);

    bool is_busy(unsigned int port_addr);

    /*************************************************************************/

    /*  rutinas de Walter para comunicarse con el aparato                    */

    /*************************************************************************/


    unsigned int direccion(unsigned char address);  /* escribe una direccion en el latch address */

    unsigned int escritura(unsigned char data);   /* escritura de un byte de datos */

    void dds_word(unsigned int w);   /* transmite un word en serie  al dds */

    unsigned int startLeer(void);

    unsigned int endLeer(void);

    unsigned char leer(void) ;   /*** leer un byte en la direccion corriente ***/

    void setganancia(unsigned int ga, unsigned int gb);
};

#endif


 