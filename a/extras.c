#define FIRST_CHAR 4 

//Declaración de extras para Ceasar Cipher

//Función para comprobar los argumentos CLI
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
  }
}

//Función para obtener el tamaño del archivo
long getFileSize(char *file)
{
  FILE *f = fopen(file, "r");
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fclose(f);
  return size;
}

//Función para crear una cadena de forma dinámica
char* createString(long n)
{
  char *s = (char*)malloc(n*sizeof(char));
  if(s == NULL)
  {
    printf("\n\n\t.:Memoria no encontrada:.\n\n");
    exit(1);
  }
  return s;
}

//Función para liberar memoria dinámica
void deleteString(char *s)
{
  free(s);
}

//Función para vaciar el archivo txt en memoria RAM
void scanFile(char *RAM, long size, char *fileName)
{
  FILE *myFile = fopen(fileName, "r");
  fread(RAM, 1, size, myFile);
  *(RAM + size) = '\0';  //Se agrega caracter de fin de cadena
  fclose(myFile);
}

//Función que hace el corrimiento de caracteres V1
/*void ceasarCipher(char *fileRAM, int shift)
{
  int i=0;
  char c;
  if(shift < 0 || shift > 26)
  {
    printf("\n\n\t.:Corriemiento invalido:.\n\n");
    exit(1);
  }
  while(*(fileRAM+i) != '\0')
  {
    c = *(fileRAM + i);
    if(65 <= c && c <= 90)  //MAYUSCULAS
    {
      *(fileRAM + i) = (((c + shift) % 91) < 65) ? (((c + shift) % 91) + 65) : ((c + shift) % 91);
    }
    if(97 <= c && c <= 122) //minusculas
    {
      *(fileRAM + i) = (((c + shift) % 123) < 97) ? ((c + shift) % 123) + 97 : ((c + shift) % 123);
    }
    i++;
  }
}*/

//Función que hace el corrrimiento de caracteres
void ceasarCipher(char *fileRAM, int B)
{
  int i=0;
  char c;
  while(*(fileRAM + i) != '\0')
  {
    c = *(fileRAM +i);
    if(65 <= c && c <= 90)   //MAYUSCULAS
    {
      c -= 65;
      c = (c+B) % 26;
      c += 65;
    }
    if(97 <= c && c <= 122)  //Minusculas
    {
      c -= 97;
      c = (c+B) % 26;
      c += 97;
    }
    *(fileRAM + i) = c;
    i++;
  }
}

//Función que pide la cantidad d a desplazar
int scanShifting()
{
  int s;
  system("clear");
  printf("\n\n\tIngresa desplazamiento: ");
  scanf("%d", &s);
  while(0 > s || s > 26)
  {
    system("clear");
    printf("\n\n\tDesplazamiento incorrecto  d->[0-26]");
    printf("\n\tIngresa desplazamiento: ");
    scanf("%d", &s);
  }
  return s;
}

//Función que encuentra el inverso aditivo en aritmética modular
int scanInverse(int c)
{
  int fcf;
  if(65 <= c && c <= 90) fcf = c - 65; //MAYUSCULAS
  if(97 <= c && c <= 122)fcf = c - 97; //minusculas
  return (FIRST_CHAR - fcf >= 0) ? (FIRST_CHAR - fcf): (26 + FIRST_CHAR -fcf); 
}

//Función que crea el nuevo nombre del archivo
char *createNewName(char *originalName, int opt) //Falta evitar desborde
{
  int i=0;

  char *newName = createString(256);
  while(*(originalName + i) != '.')
  {
    *(newName +i ) = *(originalName + i);
    i++;
  }
  if(opt == 'd') //Decifrar
  {
    *(newName + i) = '_';
    *(newName + i + 1) = 'd';
    *(newName + i + 2) = '.';
    *(newName + i + 3) = 't';
    *(newName + i + 4) = 'x';
    *(newName + i + 5) = 't';
    *(newName + i + 6) = '\0';
  }
  if(opt == 'e') //Cifrar
  {
    *(newName + i) = '_';
    *(newName + i + 1) = 'e';
    *(newName + i + 2) = '.';
    *(newName + i + 3) = 't';
    *(newName + i + 4) = 'x';
    *(newName + i + 5) = 't';
    *(newName + i + 6) = '\0';
  }
  return newName;
}

//Función que hace el corrimiento de B o  (-B) a cada caracter
void endec(char *fileRAM, int opt)
{
  if(opt == 'e') //Cifrado
    ceasarCipher(fileRAM, scanShifting());
  else           //Decifrado
    ceasarCipher(fileRAM, scanInverse(*(fileRAM+0)));
}

//Función que crea el nuevo archivo y vacia el contenido de la ram
void createNewFile(char *RAM, char *originalName, int opt, long fileSize)
{
  char *newName = createNewName(originalName, opt);
  FILE *newFile = fopen(newName, "w");
  fwrite(RAM, 1, fileSize, newFile);
  fclose(newFile);
  deleteString(newName);
}


