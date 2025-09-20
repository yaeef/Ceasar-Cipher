//Declaracion de las funciones definidas en el archivo de cabezera
//Para el procesamiento de una imagen bmp de 24 bits 

//Función que verifica argumentos CLI
void checkCLI(int argc, char flag)
{
  if(argc != 3)
  {
    printf("\n\n\t.:Error en numero de argumentos CLI:.\n\n");
    exit(1);
  }
  if(flag == '?')
  {
    printf("\n\n\t.:Error en flag seleccionada [e,d]:.\n\n");
    exit(1);
  }
}


//Funcion para lectura del BITMAPFILEHEADER, i.e, los primeros 14 bytes del header
int scanBMFH(BITMAPFILEHEADER *header, char *imageName)
{
    FILE *bmpImage = fopen(imageName, "rb");

    if(bmpImage == NULL)
    {
        printf("\n\n\t.:Error en apertura de archivo. Funcion->[scanBMFH()]\n\n");
        return 1;
    }
    fseek(bmpImage, 0, SEEK_SET);                          
    fread(&header->signature, sizeof(char), 2, bmpImage);   
    fread(&header->fileSize, sizeof(int), 1, bmpImage);
    fread(&header->reserved, sizeof(int), 1, bmpImage);
    fread(&header->dataOffset, sizeof(int), 1, bmpImage);      
    fclose(bmpImage);
    return 0;
}
//Funcion para lectura del BITMAPINFOHEADER, i.e, los 40 bytes restantes del header. Uso exclusivo para BMP 3.X estandar
int scanBMIH(BITMAPINFOHEADERV3X *header, char *imageName)
{
    FILE *bmpImage = fopen(imageName, "rb");

    if(bmpImage == NULL)
    {
        printf("\n\n\t.:Error en apertura de archivo. Funcion->[scanBMIH()]\n\n");
        return 1;
    }

    fseek(bmpImage, 14, SEEK_SET);     
    fread(&header->sizeHeader, sizeof(int), 1, bmpImage);     //LECTURA DE BITMAPINFOHEADER | 40 BYTES
    fread(&header->width, sizeof(int), 1, bmpImage);
    fread(&header->height, sizeof(int), 1, bmpImage);
    fread(&header->planes, sizeof(short int), 1, bmpImage);
    fread(&header->bitCount, sizeof(short int), 1, bmpImage);
    fread(&header->compression, sizeof(int), 1, bmpImage);
    fread(&header->imageSize, sizeof(int), 1, bmpImage);
    fread(&header->Xppm, sizeof(int), 1, bmpImage);
    fread(&header->Yppm, sizeof(int), 1, bmpImage);
    fread(&header->colorsUsed, sizeof(int), 1, bmpImage);
    fread(&header->colorsImportant, sizeof(int), 1, bmpImage);
    fclose(bmpImage);
    return 0;
}
//Funcion para verificar que la imagen puede ser procesada.
void checkBMP(BITMAPINFOHEADERV3X headerInfo, BITMAPFILEHEADER headerFile)
{

    if((headerFile.signature[0] != 66) || (headerFile.signature[1] != 77))
    {
        printf("\n\n\t.:Error de archivo. Error->[Firma incorrecta]");
        exit(1);
    }
    if(headerFile.dataOffset != 54)
    {
        printf("\n\n\t.:Error de archivo. Error->[Header incompatible]");
        exit(1);
    }
    if(headerInfo.sizeHeader != 40)
    {
        printf("\n\n\t.:Error de archivo. Error->[Header incompatible]");
        exit(1);
    }
    if(headerInfo.bitCount != 24)
    {
        printf("\n\n\t.:Error de archivo. Error->[La imagen debe de ser de 24 bits]");
        exit(1);
    }
    if(headerInfo.compression != 0)
    {
        printf("\n\n\t.:Error de archivo. Error->[Imagen debe ser sin compresion]");
        exit(1);
    }
}

/*Funcion para crear la matriz dinamica para almacenar los bytes de color*/
unsigned char **createMatrix(int height, int width)
{
    unsigned char **M;
    M = (unsigned char**)malloc(height*sizeof(unsigned char*));
    if(M == NULL)
    {
        printf("\n\n\t.:Error de memoria. Error->[No se pudo crear la matriz]");
        exit(1);
    }
    for(int i=0; i<height; i++)
    {
        *(M + i) = (unsigned char*)calloc(width, sizeof(unsigned char));
        if(*(M + i) == NULL)
        {
            printf("\n\n\t.:Error de memoria. Error->[No se pudo crear la matriz]");
            exit(1);
        }
    }
    return M;
}
//Funcion para liberar memoria usada de forma dinamica por la matriz de colores
void destroyMatrix(unsigned char **M, int height, int width)
{
    for(int i=0; i<height; i++)
        free(*(M + i));
    free(M);
}
//Funcion para obtener los valores rgb de la imagen bmp
void scanColors(unsigned char **M, int height, int dataOffset, int padded ,char *imageName)
{
    FILE *bmpImage = fopen(imageName, "rb");

    if(bmpImage == NULL)
    {
        printf("\n\n\t.:Error en apertura de archivo. Funcion->[scanColors()]\n\n");
        exit(1);
    }
    fseek(bmpImage, dataOffset, SEEK_SET);
    for(int i=0; i < height; i++)
    {
        fread(*(M+i), padded, 1, bmpImage);
    }
    fclose(bmpImage);
}

//Función que lee la cantidad a desplazar la imagen
int scanShifting()
{
  int s;
  system("clear");
  printf("\n\n\tIngresa desplazamiento: ");
  scanf("%d", &s);
  while(0 > s || s > 257)
  {
    system("clear");
    printf("\n\n\tDesplazamiento incorrecto d->[0-256]");
    printf("\n\tIngresa desplazamiento: ");
    scanf("%d", &s);
  }
  return s;
}

//Función que calcula el inverso en aritmética modular para restaurar la imagen
int scanInverse(int c)
{
  return ((c - 171) >= 0) ? ((256 - (c - 171)%256)) : ((171 -c)%256);
  //return ((FIRST_COLOR - c) >= 0) ? ((FIRST_COLOR - c)%256) : ((256 + FIRST_COLOR - c)%256);
}

//Función que lleva a cabo el desplazamiento en la matriz de colores
void ceasarCipher(unsigned char **M, int height, int width, int padding, int shift)
{
  for(int i=0; i<height; i++)
    for(int j=0; j < 6*width-padding; j+=3)                //De esta manera se itera solo los bytes de pixel y no los bytes de relleno
    {
        *(*(M+i)+j) = (*(*(M+i)+j) + shift) % 256;        //M[i][j] = (M[i][j] + shift) % 256;
        *(*(M+i)+j+1) = (*(*(M+i)+j+1) + shift) % 256;    //M[i][j+1] = (M[i][j+1] + shift) % 256;
        *(*(M+i)+j+2) = (*(*(M+i)+j+2) + shift) % 256;    //M[i][j+2] = (M[i][j+2] + shift) % 256;
    }
}

//Función de cifrado y decifrado
void endec(unsigned char **M, int opt, int height, int width, int padding)
{
  if(opt == 'e')
    ceasarCipher(M, height, width, padding, scanShifting());
  else
    ceasarCipher(M, height, width, padding, scanInverse(*(*(M+0)+0)));         //Se pasa el primer color de la imagen
}

//Funcion que ngenera el nuevo nombre der archivo procesado
char* createNewName(char *originalName, int opt)
{
    int i=0;
    char *newName = (char*)malloc(sizeof(char)*MAXLENSTRING);
    if(newName == NULL)
    {
        printf("\n\n\t.:Error en creacion de nombre nuevo. Funcion->[createNewName()]\n\n");
        exit(1);
    }
    while(*(originalName +i) != '.')
    {
      *(newName + i) = *(originalName + i);
      i++;
    }
    if(opt == 'd')   //Nuevo nombre para decifrado
    {
        newName[i] = '_';
        newName[i+1] = 'd';
        newName[i+2] = '.';
        newName[i+3] = 'b';
        newName[i+4] = 'm';
        newName[i+5] = 'p';
        newName[i+6] = '\0';
    }
    else         //Nuevo nombre para cifrado 
    {
        newName[i] = '_';
        newName[i+1] = 'e';
        newName[i+2] = '.';
        newName[i+3] = 'b';
        newName[i+4] = 'm';
        newName[i+5] = 'p';
        newName[i+6] = '\0';
    }
    return newName;
}

//Función que escribe la nueva imagen, tanto el header como la matriz de colores
void createNewBMP(unsigned char **M, BITMAPFILEHEADER headerFile, BITMAPINFOHEADERV3X headerInfo, int padding, char *originalName, int opt)
{
  char *newName = createNewName(originalName, opt);
  FILE *newImage = fopen(newName, "wb"); //Falta comprobación de creación del archivo
  fwrite(&headerFile, sizeof(BITMAPFILEHEADER), 1, newImage);
  fwrite(&headerInfo, sizeof(BITMAPINFOHEADERV3X), 1, newImage);
  for(int i=0; i<headerInfo.height; i++)
    fwrite(M[i], padding, 1, newImage);
  fclose(newImage);
  free(newName);
}
