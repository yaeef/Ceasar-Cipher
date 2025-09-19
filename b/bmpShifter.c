/*Codigo para desplazar el color en imagenes bmp de 24 bits con bitmapinfoheaderv3 estandar*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmpImage.h"



int main(int argc, char **argv)
{
    int typeOfOp = checkArg(argc, argv);
    char *fileName = *(argv+2);
    int shift;
    BITMAPFILEHEADER headerFile;                                          //Header del archivo .bmp
    BITMAPINFOHEADERV3X headerInfo;                                       //Header que contiene información sobre la imagen
    unsigned char **M;
    char *newName;

    scanBMFH(&headerFile, fileName);
    scanBMIH(&headerInfo, fileName);
    checkBMP(headerFile.signature, headerFile.dataOffset, headerInfo.sizeHeader, headerInfo.bitCount, headerInfo.compression);
    int padded = ((headerInfo.width*3+3)/4)*4;                            //Alineacion a 4 bytes 
    M = createMatrix(headerInfo.height, padded);                          //Creacion de matriz de bytes
    scanColors(M, headerInfo.height, headerFile.dataOffset, padded, fileName);//Lectura de colores
    newName = createNewName(fileName, typeOfOp);

    if(typeOfOp == 2)   //Decifrado
    {
        if(((int)(M[0][0])) -171 >= 0)
            shift = (256 - (((int)(M[0][0])) - 171)) % 256;
        else
            shift = (171 - ((int)(M[0][0]))) % 256;
        shiftColors(M,headerInfo.height,headerInfo.width, padded, shift);
    }
    else     //Cifrado
    {  
        system("clear");
        printf("\n\n\tIngrese la cantidad a desplazar: ");
        scanf("%d", &shift);
        shiftColors(M,headerInfo.height,headerInfo.width, padded, shift);
    }
    createBMP(headerFile, headerInfo, M, headerInfo.height, padded, newName);
    free(newName);
    destroyMatrix(M, headerInfo.height, padded);
    return 0;
}
