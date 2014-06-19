#include "USB.h"
#include <stdio.h>

char *dll = "mpusbapi";


void PuertoUSB::LoadDLL(void) {
  libHandle = NULL;
  libHandle = LoadLibrary(dll);
  if (libHandle == NULL) {
	printf("Error loading %s.dll\n", dll);
    exit(-1);
  } else {
    MPUSBGetDLLVersion = (DWORD(*)(void))\
                        GetProcAddress(libHandle, "_MPUSBGetDLLVersion");
    MPUSBGetDeviceCount = (DWORD(*)(PCHAR))\
                        GetProcAddress(libHandle, "_MPUSBGetDeviceCount");
    MPUSBOpen = (HANDLE(*)(DWORD, PCHAR, PCHAR, DWORD, DWORD))\
                        GetProcAddress(libHandle, "_MPUSBOpen");
    MPUSBWrite = (DWORD(*)(HANDLE, PVOID, DWORD, PDWORD, DWORD))\
                        GetProcAddress(libHandle, "_MPUSBWrite");
    MPUSBRead = (DWORD(*)(HANDLE, PVOID, DWORD, PDWORD, DWORD))\
                        GetProcAddress(libHandle, "_MPUSBRead");
    MPUSBReadInt = (DWORD(*)(HANDLE, PVOID, DWORD, PDWORD, DWORD))\
                        GetProcAddress(libHandle, "_MPUSBReadInt");
    MPUSBClose = (BOOL(*)(HANDLE)) GetProcAddress(libHandle, "_MPUSBClose");

    if ((MPUSBGetDeviceCount == NULL) || (MPUSBOpen == NULL) ||
                 (MPUSBWrite == NULL) || (MPUSBRead == NULL) ||
                 (MPUSBClose == NULL) || (MPUSBGetDLLVersion == NULL) ||
                 (MPUSBReadInt == NULL)) {
      printf("Something went wrong, can't load lib %s.dll\n", dll);
      exit(-1);
    }
  }
}


PuertoUSB::PuertoUSB() {
  payLoadCounter = 2;
  LoadDLL();
}


void PuertoUSB::SetComando(int comando) {
  TxBuffer[0] = comando;
}


void PuertoUSB::direccion(int direccion) {
  TxBuffer[1] = direccion;
}


void PuertoUSB::parametro(int parametro) {
  TxBuffer[payLoadCounter] =  parametro;
  payLoadCounter++;
}


void PuertoUSB::EnviarDatos_Respuesta() {
  DWORD selection;
  DWORD SendLength = 64;
  DWORD ReceiveLength = 0;
  DWORD SentDataLength = 0;
  DWORD ExpectedReceiveLength = 64;
  UINT SendDelay = 10000;
  UINT ReceiveDelay = 15000;
  // BYTE receive_buf[1024 * 128]={0};

  myOutPipe = MPUSBOpen(0, vid_pid, out_pipe, MP_WRITE, 0);
  myInPipe = MPUSBOpen(0, vid_pid, out_pipe, MP_READ, 0);

  if (myOutPipe == INVALID_HANDLE_VALUE || myInPipe == INVALID_HANDLE_VALUE) {
    return;
  }

  if (MPUSBWrite(myOutPipe, TxBuffer, SendLength, &SentDataLength, SendDelay)) {
    if (MPUSBRead(myInPipe, RxBuffer, ExpectedReceiveLength, &ReceiveLength,
                                                                ReceiveDelay)) {
      ;
    } else {
      CheckInvalidHandle();
    }
  }

  MPUSBClose(myOutPipe);
  MPUSBClose(myInPipe);
  myOutPipe = myInPipe = INVALID_HANDLE_VALUE;

  payLoadCounter = 2;

  return;
}


DWORD PuertoUSB::GetSummary(void) {
  HANDLE tempPipe = INVALID_HANDLE_VALUE;
  DWORD count = 0;
  DWORD max_count;
  DWORD retvalue = 256;

  max_count = MPUSBGetDeviceCount(vid_pid);
  // Note:
  // The total number of devices using the generic driver could be
  // bigger than max_count. They could have different vid & pid numbers.
  // This means if max_count is 2, the valid instance index do not
  // necessary have to be '0' and '1'.
  //
  // Below is a sample code for searching for all valid instance indexes.
  // MAX_NUM_MPUSB_DEV is defined in _mpusbapi.h

  count = 0;
  for (int i = 0; i < MAX_NUM_MPUSB_DEV; i++) {
    tempPipe = MPUSBOpen(i, vid_pid, NULL, MP_READ, 0);
    if (tempPipe != INVALID_HANDLE_VALUE) {
      MPUSBClose(tempPipe);
      count++;
      retvalue = i;
    }
    if (count == max_count)
      break;
  }
  return retvalue;
}


void PuertoUSB::CheckInvalidHandle(void) {
  if (GetLastError() == ERROR_INVALID_HANDLE) {
    // Most likely cause of the error is the board was disconnected.
    MPUSBClose(myOutPipe);
    MPUSBClose(myInPipe);
    myOutPipe = myInPipe = INVALID_HANDLE_VALUE;
  }
}


void PuertoUSB::soloEnviarDatos() {
  DWORD selection;
  DWORD SendLength = 64;
  DWORD SentDataLength = 0;
  UINT SendDelay = 10000;

  myOutPipe = MPUSBOpen(0, vid_pid, out_pipe, MP_WRITE, 0);

  if (myOutPipe == INVALID_HANDLE_VALUE) {
    return;
  }

  if (MPUSBWrite(myOutPipe, TxBuffer, SendLength, &SentDataLength, SendDelay)) {
    ;
  } else {
    CheckInvalidHandle();
  }

  MPUSBClose(myOutPipe);
  myOutPipe = INVALID_HANDLE_VALUE;

  return;
}


void PuertoUSB::soloEsperarRespuesta(unsigned int ExpectedLenght,
                                     char *puntero) {
  DWORD selection;
  DWORD ReceiveLength = 0;
  DWORD ExpectedReceiveLength = ExpectedLenght;
  UINT ReceiveDelay = 30000;
  // ----   cambiar este una vez aclarod el trabajo de los canales
  char * receive_buf = puntero;

  myInPipe = MPUSBOpen(0, vid_pid, out_pipe, MP_READ, 0);

  if (myInPipe == INVALID_HANDLE_VALUE) {
    return;
  }

  if (MPUSBRead(myInPipe, receive_buf, ExpectedReceiveLength, &ReceiveLength,
                                                                ReceiveDelay)) {
    ;
  } else {
    CheckInvalidHandle();
  }

  MPUSBClose(myInPipe);
  myInPipe = INVALID_HANDLE_VALUE;

  return;
}






