//Ceasar Cipher para texto

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Estandar POSIX para lectura de flags :)
#include "extras.h"
#define FIRST_CHAR 4


int main(int argc, char **argv)
{
  int opt = getopt(argc, argv, "e:d:");     //Leer flag (con optarg se obtiene nombre)
  checkCLI(argc, opt);
  long fileSize = getFileSize(optarg);      //Tamaño del archivo
  char *fileRAM = createString(fileSize+1); //Reservar memoria archivo + '\0'
  scanFile(fileRAM, fileSize, optarg);      //Vaciado de txt en RAM
  
  endec(fileRAM, opt);
  createNewFile(fileRAM, optarg, opt, fileSize);

  deleteString(fileRAM);
  return 0; 
}

