//Declaracion de las funciones definidas en el archivo de cabezera
//Para el procesamiento de una imagen bmp de 24 bits 

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
void checkBMP(char *signature, int dataOffset, int sizeHeader, short int bitCount, int compression)
{

    if((*(signature) != 66) || (*(signature+1) != 77))
    {
        printf("\n\n\t.:Error de archivo. Error->[Firma incorrecta]");
        exit(1);
    }
    if(dataOffset != 54)
    {
        printf("\n\n\t.:Error de archivo. Error->[Header incompatible]");
        exit(1);
    }
    if(sizeHeader != 40)
    {
        printf("\n\n\t.:Error de archivo. Error->[Header incompatible]");
        exit(1);
    }
    if(bitCount != 24)
    {
        printf("\n\n\t.:Error de archivo. Error->[La imagen debe de ser de 24 bits]");
        exit(1);
    }
    if(compression != 0)
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
//Funcion para desplazar los bytes de color de la imagen bmp
void shiftColors(unsigned char **M, int height, int width, int padded, int shift)
{
    for(int i=0; i<height;i++)
        for(int j=0; j < 6*width-padded; j+=3)                //De esta manera se itera solo los bytes de pixel y no los bytes de relleno
        {
            *(*(M+i)+j) = (*(*(M+i)+j) + shift) % 256;        //M[i][j] = (M[i][j] + shift) % 256;
            *(*(M+i)+j+1) = (*(*(M+i)+j+1) + shift) % 256;    //M[i][j+1] = (M[i][j+1] + shift) % 256;
            *(*(M+i)+j+2) = (*(*(M+i)+j+2) + shift) % 256;    //M[i][j+2] = (M[i][j+2] + shift) % 256;
        }
}
//crear nueva imagen
void createBMP(BITMAPFILEHEADER headerFile, BITMAPINFOHEADERV3X headerInfo, unsigned char **M, int height, int padded, char *newName)
{
    FILE *newImage = fopen(newName, "wb");
    if(newImage == NULL)
    {
        printf("\n\n\t.:Error en creacion de imagen. Funcion->[createBMP()]\n\n");
        exit(1);
    }

    fwrite(&headerFile, sizeof(BITMAPFILEHEADER), 1, newImage);
    fwrite(&headerInfo, sizeof(BITMAPINFOHEADERV3X), 1, newImage);

    for(int i=0;i < headerInfo.height; i++)
        fwrite(M[i], padded, 1, newImage );
    fclose(newImage);
}

//Funcion que verifica el argumento parado mediante CLI para elegir cifrado o decifrado de imagen
int checkArg(int argc, char **argv)
{
    if(argc < 3 )
    {
        printf("\n\n\tError en argumentos CLI. Funcion->[checkArg()]\n\n.:");
        exit(1);
    }
    if(!((strcmp("-e", *(argv+1)) == 0) || (strcmp("-d", *(argv+1)) == 0)))      
    {
        printf("\n\n\tArgumento invalido. Argumentos posibles: [-e][-d]. Funcion->[checkArg()]\n\n.:");
        exit(1);
    }
    if(strcmp("-e", *(argv+1)) == 0)                                     //1 -> cifrar
        return 1;
    else
        return 2;                                                        //2 -> decifrar
}
//Funcion que ngenera el nuevo nombre der archivo procesado
char* createNewName(char *fileName, int typeOfOp)
{
    int i=0;
    char *newName = (char*)malloc(sizeof(char)*MAXLENSTRING);
    if(newName == NULL)
    {
        printf("\n\n\t.:Error en creacion de nombre nuevo. Funcion->[createNewName()]\n\n");
        exit(1);
    }
    if(typeOfOp == 2)   //Nuevo nombre para decifrado
    {
        while(fileName[i] != 46)
        {
            newName[i] = fileName[i];
            i++;
        }
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
        while(fileName[i] != 46)
        {
            newName[i] = fileName[i];
            i++;
        }
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