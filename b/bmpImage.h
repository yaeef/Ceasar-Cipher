//Definición de estructuras y funciones usadas para leer, almacenar y escribir
//en imagenes bmp de 24 bits, sin compresion y con un HEADER V3 Standard



#define FIRSTCOLOR 128    //Colores: Gris~[128][128][128] | Rojo~[0][0][255] | Blanco~[255][255][255]
#define MAXLENSTRING 100
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

int scanBMFH(BITMAPFILEHEADER*, char*);
int scanBMIH(BITMAPINFOHEADERV3X*, char*);
void checkBMP(char*, int, int, short int, int);
unsigned char **createMatrix(int, int);
void destroyMatrix(unsigned char**, int, int);
void scanColors(unsigned char**, int, int, int ,char*);
void shiftColors(unsigned char**, int, int, int, int);
void createBMP(BITMAPFILEHEADER, BITMAPINFOHEADERV3X, unsigned char**, int, int, char*);
int checkArg(int, char**);
char* createNewName(char*, int);

#include "bmpImage.c"