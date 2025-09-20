//Definición de estructuras y funciones usadas para leer, almacenar y escribir
//en imagenes bmp de 24 bits, sin compresion y con un HEADER V3 Standard


#define MAXLENSTRING 256
#pragma pack(push, 1)
typedef struct
{   
    char signature[2];           //Firma 'BM'
    int fileSize;                //Tamaño del archivo
    int reserved;                //Reservado; Sin uso; (=0)
    int dataOffset; 
}BITMAPFILEHEADER;

typedef struct
{
    int sizeHeader;             //Tamaño del BITMAPINFOHEADER
    int width;                  //Ancho en pixeles de la imagen
    int height;                 //Alto en pixeles de la imagen
    short int planes;                 
    short int bitCount;         //Cantidad de bits por pixel; (=24)
    int compression;            //Bandera de compresion; (=0) BMP NO COMPRIMIDA
    int imageSize;              //Tamaño de la imagen
    int Xppm;                   //Resolucion horizontal [Pixels/meter]
    int Yppm;                   //Resolucion vertical   [Pixels/meter]
    int colorsUsed;             //Cantidad de colores usados en la imagen
    int colorsImportant;        //Cantidad de colores importantes; (0=all)
}BITMAPINFOHEADERV3X;
#pragma pack(pop)

void checkCLI(int , char );
int scanBMFH(BITMAPFILEHEADER*, char*);
int scanBMIH(BITMAPINFOHEADERV3X*, char*);
void checkBMP(BITMAPINFOHEADERV3X, BITMAPFILEHEADER);
unsigned char **createMatrix(int, int);
void destroyMatrix(unsigned char**, int, int);
void scanColors(unsigned char**, int, int, int ,char*);
int scanShifting();
int scanInverse(int );
void ceasarCipher(unsigned char **, int , int , int , int );
void endec(unsigned char **, int , int , int , int );
char* createNewName(char*, int);
void createNewBMP(unsigned char **, BITMAPFILEHEADER , BITMAPINFOHEADERV3X , int , char *, int );

#include "bmpImage.c"
