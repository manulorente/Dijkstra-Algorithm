/*
**	Fichero: ips.h	
**	Autor:Manuel Lorente Almán	
**	Fecha: 07-04-2013
**	
**	
**	Descripcion: Fichero de librería donde se exponen las estructuras
**      utilizadas a lo largo del programa con sus respectivos campos,
**      los mensajes de error, constantes utilizadas y el prototipo 
**      y una breve explicacion de las funciones que desarrollan 
**      el programa. 
*/

#ifndef IPS_H
#define IPS_H

/*Mensajes de error*/

#define ERROR1 "Error 1: argumentos incorrectos.\n"
#define ERROR2 "Error 2: el fichero de configuracion no se puede leer.\n"
#define ERROR3 "Error 3: el fichero de configuracion es incorrecto.\n"
#define ERROR4 "Error 4: el fichero de entrada no se puede leer.\n"
#define ERROR5 "Error 5: el fichero de salida no se puede abrir.\n"
#define ERROR6 "Error 6: error de memoria.\n"

/*Constantes del fichero*/

#define TAM 40
#define INFINITO 99999999

/*Estructura de la lista de enlaces*/

typedef struct enlace 
{
  char origen[TAM];
  char destino[TAM];
  int peso; 
  struct enlace * sig; 
}ENLACE;

/*Estructura de la lista de nodos simples*/

typedef struct nodo
{
  char ip[TAM];
  char predecesor [TAM];
  int coste;
  struct nodo *sig;
}NODO;

/*Funciones que crean un nodo de la lista de enlaces y de
  la lista de nodos simples,una recibe un peso de enlace y
  ambas reciben un puntero a una variable de control para 
  gestionar el uso de la memoria*/

ENLACE * Crea_Enlaces(int peso, int * memoria);
NODO * Crea_Nodo(int * memoria);

/*Funciones que enlazan nodos simples en sus respectivas listas,
  reciben el nodo a enlazar y la lista*/

void enlaza(ENLACE *p, ENLACE **pp);
void enlaza2 (NODO *q, NODO ** qq);

/*Función que compara cada nodo de la lista de enlaces
  con la lista final para no repetir ninguno a la hora de crear
  la lista de nodos simples.Recibe la lista de enlace auxiliar y la
  de nodos simples*/
int compara1 (ENLACE * aux, NODO * listafinal); 
int compara2 (ENLACE * aux, NODO * listafinal); 

/*Conjunto de funciones que comprueban y chequean las listas dentro 
  de los límites de cadenas y pesos establecidos, o bien
  las procesan para un uso mas facil de las funciones que calculan el 
  algoritmo.*/

int comprueba (char * cadena, char * cadena2, int peso, int control2) ;
int comprueba2 (char * cadena, char * cadena2, ENLACE * aux);
int filtro(NODO *listafinal, int numnodos);
int encontrado (NODO *listafinal, char * ip, int * numnodos);
NODO * ultima(NODO * listafinal, char * b);

/*Funciones que se encargan de inicializar lista de nodos simples, 
  calcular la ruta mínima en la red y de imprimir el camino correcto
  en el fichero de salida.*/

void Inicializa_Nodos(NODO * listafinal, int numnodos);
void Encuentra_Ruta(ENLACE * enlaces, NODO * listafinal, int numenlaces, int numnodos);
int Imprime_Camino(NODO * listafinal, int numnodos, FILE * fsal, char * origen, char * cadenaauxiliar);

/*Equipo de funciones que se encargan del mantenimiento del programa, 
  destruyendo nodos o borrando listas enteras y gestiona los errores
  en el caso de que los hubiese*/

void Destruye_Nodo(NODO **pp);
void Destruye_Enlace(ENLACE **pp);
void Libera_Enlace(ENLACE ** primero, int numenlaces); 
void Libera_Nodo(NODO ** primero, int numnodos, NODO * ultimo);
void Gestiona_Errores(int error);

#endif





