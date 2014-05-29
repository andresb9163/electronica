//----------------------------------------------------------------------------
#ifndef USB_H
#define USB_H

#define   MAXRXBUFF   128*1024
#include "windows.h"
#include "mpusbapi.h"

    char vid_pid[]= "vid_04d8&pid_000c";    // Default Demo Application Firmware
    char out_pipe[]= "\\MCHP_EP1";
    char in_pipe[]= "\\MCHP_EP1";
    static HINSTANCE libHandle;
    static HANDLE myOutPipe;
    static HANDLE myInPipe;


class PuertoUSB {
  private:
    // Global Vars

    unsigned char TxBuffer[64];
    
    DWORD temp;
    unsigned char payLoadCounter;

   public:
    unsigned char RxBuffer[MAXRXBUFF];

    unsigned char Cantidad_de_paquetes;
    unsigned char Canal2[MAXRXBUFF];     // Canal B del AD        0x08
    unsigned char Canal1[MAXRXBUFF];     // Canal A del AD        0x0A
    unsigned char Canal3[MAXRXBUFF];     // Canal AB del AD       0x09
    unsigned char Canal4[MAXRXBUFF];     // Canal recepcion puerto serie

    unsigned int CanalB[MAXRXBUFF];     // Canal B del AD
    unsigned int CanalA[MAXRXBUFF];     // Canal A del AD


    /*
    char  RxBuffer[MAXRXBUFF];          // Buffer para leer datos desde el puerto USB
    char  TxBuffer[MAXTXBUFF];          // Buffer para enviar datos por el puerto USB
    unsigned int Canal2[MAX_CANAL];     // Canal B del AD
    unsigned int Canal1[MAX_CANAL];     // Canal A del AD
    unsigned int Canal3[MAX_CANAL];     // Canal AB del AD
    unsigned int Canal4[MAXBUFFPS];     // Canal recepcion puerto serie
    int   cant_datos;                   // Cantidad de datos leidos del AD
    int   PosBuffTx;                    // Entero para la posicion del buffer TxBuffer
    DWORD RxBytes;                      // Cantidad de datos leidos del puerto
    */
  
    PuertoUSB();
    DWORD GetSummary(void);
    void CheckInvalidHandle(void);
    void SetComando(int);
    void direccion (int);

    void parametro(int);
    void EnviarDatos_Respuesta();
    void LoadDLL(void);
    void soloEnviarDatos();
    void soloEsperarRespuesta(unsigned int, char * );

};


#endif
