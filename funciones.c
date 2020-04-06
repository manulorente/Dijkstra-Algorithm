/*
**	Fichero: funciones.c	
**	Autor:Manuel Lorente Almán	
**	Fecha: 07-04-2013
**	
**	
**	Descripcion: Fichero con funciones utilizadas en todo el programa
**      y cuya breve descripcion encontramos en el fichero de libreria
**      "ips.h".
*/

/*Includes del sistema*/

#include <stdio.h>//Contiene funciones de biblioteca y prototipos de C

#include <stdlib.h>//Libreria para la reserva dinámica de memoria

#include <string.h>//Libreria para el manejo de cadena de caracteres

/*Includes de la aplicación*/
#include "ips.h"

/*Función que segun el error recibido imprime un mensaje de error o no*/

void Gestiona_Errores(int error)
{
  switch(error)
    {
    case 1:
      fprintf(stderr,ERROR1);
      break;
    case 2:
      fprintf(stderr,ERROR2);
      break;
    case 3:
      fprintf(stderr,ERROR3);
      break;
    case 4:
      fprintf(stderr,ERROR4);
      break;
    case 5:
      fprintf(stderr,ERROR5);
      break;
    case 6:
      fprintf(stderr,ERROR6);
      break;
    default:
      break;
    }
}

/*Crean un nodo si es de tipo enlace con el campo peso
  con reserva dinámica en el caso de que 
  no sea creado correctamente la bandera "memoria" modifica su valor
  para posterior gestion del error*/

ENLACE * Crea_Enlaces(int peso, int * memoria)
{
  ENLACE * aux = NULL; 
  aux= (ENLACE *) malloc (sizeof (ENLACE)); 
  if (aux != NULL)
    {
      aux->peso = peso; 
      aux->sig = NULL; 
      *memoria = 0;
    }
  else
    {
      *memoria = 1;
    }
  return aux; 
}

NODO * Crea_Nodo(int * memoria)
{
  NODO * aux = NULL; 
  aux = (NODO *) malloc (sizeof(NODO)); 
  if (aux != NULL) 
    {
      aux->sig = NULL;  
      *memoria = 0;
    }
  else
    {
      *memoria = 1;
    }
  return aux; 
}

/*Funciones que enlazan un nodo simple en una lista del mismo tipo*/

void enlaza(ENLACE *p, ENLACE **pp) 
{
  ENLACE * aux = NULL; 
  aux = *pp;
  aux->sig = p;
  aux = p;
  *pp = aux;
}

void enlaza2 (NODO *q, NODO ** qq) 
{
  NODO * aux = NULL;
  aux = *qq; 
  aux->sig = q; 
  aux = q;
  *qq = aux;
}

/*Las dos siguientes funciones comparan uno y cada uno de los nodos
  de la lista de enlaces con la lista de nodos simples para no repetir
  ninguno, en el caso de que un nodo no sea igual, pasa al siguiente.
  Recibe el nodo en cuestión y la lista a comparar*/

int compara1 (ENLACE * aux, NODO * listafinal)  
{
  int control = 1;
  
  while (control != 0 && listafinal != NULL)
    {
      control = strcmp (aux->origen,listafinal->ip); 
      if ((control != 0) && (listafinal != NULL))
	{
	  listafinal= listafinal->sig;
	}
    }
  return control;
}

int compara2 (ENLACE * aux, NODO * listafinal)  
{
  int control = 1;
  
  while (control != 0 && listafinal != NULL)
    {
      control = strcmp (aux->destino, listafinal->ip); 
      if ((control != 0) && (listafinal != NULL))
	{
	  listafinal = listafinal->sig;
	}
    }
  return control;
}

/*En la inicializacion del conjunto de nodos se le asigna a cada nodo un 
  coste infinio salvo para ese nodo desde el que se calcula la ruta
  (nodo inicia) que tiene coste 0.Para cada nodo en la red de nodos el
  predecesir se inicializa a NINGUNO(NO).Recibe la lista y el numero de
  nodos totales.*/

void Inicializa_Nodos(NODO * listafinal, int numnodos)
{
  int i = 1;
  char string[10] = "NO";
  
  for (i=1; i<=numnodos; i++)
    {
      if (i == 1) 
	{
	  listafinal->coste = 0;
	  strcpy (listafinal->predecesor, string);
	  listafinal = listafinal ->sig;
	}
      else 
	{
	  listafinal->coste = INFINITO; 
	  strcpy (listafinal->predecesor, string);
	  listafinal = listafinal->sig;
	}
    }
}

/*Este algoritmo se un bucle que se repite el numero de nodos menos 1. En cada
 iteracion de recorren todos los enlaces revisando los costes calculados 
 previamente y se van modificando los campos coste en el caso de ser la
 ruta optima. Recibe ambas listas y el numero de nodos de cada lista.*/

void Encuentra_Ruta(ENLACE * enlaces, NODO * listafinal, int numenlaces, int numnodos)
{
  /*Contadores para los bucles*/
  int i = 0;
  int j = 0; 
  int control = 0;
  /*Punteros auxiliares para el manejo de la listas*/
  NODO * aux1 = NULL;
  NODO * aux2 = NULL;
  ENLACE * aux3 = NULL; 
  
  /*Siempre utilizamos punteros auxiliares para no perder la referencia
    de la lista*/
  
  aux3 = enlaces;
  
  for (i=1; i<numnodos; i++) 
    {
      enlaces = aux3;
      for (j =1; j<=numenlaces; j++)
	{ 
	  aux1 = listafinal;
	  aux2 = listafinal; 
	  control = strcmp (enlaces->origen, aux1->ip);
	  
	  while (control != 0 && aux1 != NULL) 
	    {
	      aux1 = aux1->sig; 
	      control = strcmp (enlaces->origen, aux1->ip);
	    }
	  
	  control = strcmp (enlaces->destino, aux2->ip);
	  while (control !=0 && aux2 != NULL)
	    {
	      aux2 = aux2->sig;
	      control = strcmp (enlaces->destino, aux2->ip);
	    }
	  
	  if ((aux1->coste + enlaces->peso) < (aux2->coste))
	    {
	      aux2->coste = aux1->coste + enlaces->peso; 
	      strcpy(aux2->predecesor, aux1->ip); 
	    }
	  enlaces = enlaces->sig; 
	}
    }
}
/*Funcion que recibe la lista con la ruta calculada,el numero de nodos,
  el fichero a escribir y las cadenas origen y destino de la red. 
  Se encarga de imprimir la lista final con el camino entre el origen
  y destino con el menor coste posible*/

int Imprime_Camino(NODO * listafinal, int numnodos, FILE * fsal, char * origen, char * cadenaauxiliar) 
{
  
  NODO * aux = NULL; 
  NODO * aux2 = NULL; 
  NODO * prim = NULL; 
  NODO * ant = NULL; 
  char string[40];
  char id[40];
  
  /*Puntero que modifica el valor del numero de nodos
   cuando se borra alguno.*/
  
  int * contador = &numnodos;
  int j = 1;
  int control = 0; 
  int control2 = 0;

  /*Variable para controlar el resultado de la comparacion entre
    el origen y la lista para comenzar a imprimir*/
  
  int stop = 0; 
  
  /*Coste total del camino calculado*/
  
  int total = -1;   
  
  /*Borra los nodos inservibles de la lista final*/
  
  numnodos = filtro(listafinal, numnodos);
  
  /*Copia el origen de la ruta a una cadena auxiliar para compararla
    con la lista.*/
  
  strcpy (string, cadenaauxiliar); 
  
  /*Siempre utilizaremos punteros auxiliares para el manejo de listas*/
  
  prim = listafinal; 
  
  /*Obtenemos el coste del último nodo, cuyo campo coste es el mas alto de
    todos y es el coste total del camino*/
  
  while ((prim != NULL)&& (total == -1))
    {
      if ((strcmp (string, prim->ip)) == 0)
	total = prim->coste;
      else
	prim = prim->sig; 
    }
  
  prim = listafinal;
  
  /*Borramos los nodos de la lista que no vayamos a imprimir*/
  
  while (prim != NULL) 
    {
      ant = prim; 
      prim = prim->sig;
      if ((prim != NULL) && (strcmp(string, prim->ip) != 0))
	{
	  strcpy (id, prim->ip);
	  control2 = encontrado (listafinal, id,contador);
	  if (control2 == 0)
	    {
	      Destruye_Nodo(&(ant->sig));
	    }
	}
    }
  
  aux = listafinal; 
  aux2 = listafinal->sig;
  fprintf (fsal, "%s\t%s\t%d\n\nCamino\n", origen, string, total);

  /*En el caso de que el nodo origen y destino sea el mismo*/
  
  if ((strcmp (origen, string)) == 0)
    {
      fprintf (fsal, "%s\t0\t0\n", origen); 
    }
  else 
    {
      fprintf (fsal, "%s\t%d\t%d\n", aux->ip, aux->coste, aux->coste); 
      
      /*Vamos recorriendo la lista e imprimiendo los nodos que correspondan 
	con el predecesor*/
      
      while (j<numnodos)
	{
	  control = strcmp (aux->ip, aux2->predecesor); 
	  if (control == 0) 
	    {
	      fprintf (fsal, "%s\t%d\t%d\n", aux2->ip, 
		       ((aux2->coste)-(aux->coste)), aux2->coste);
	      j++;
	      stop = strcmp (aux2->ip, string);
	      if (stop == 0) 
		{
		  j = numnodos;
		}
	      
	      aux = aux2; 
	      aux2 = aux->sig;
	    }
	  
	  else 
	    {
	      aux2 = aux2->sig;
	    }
	  
	  if (aux2 == NULL)  
	    {
	      aux2 = listafinal; 
	    }
	  if (aux == NULL) 
	    {
	      aux = listafinal; 
	    }
	}
    }
  return numnodos;
}

/*Funciones de filtro que elimina los nodos inservibles de la lista final 
  y que devuelve la posicion en la lista de una determinada cadena */

int filtro(NODO *listafinal, int numnodos) 
{
  NODO * prim = listafinal; 
  NODO * aux1 = listafinal;
  
  int i = 1;
  
  char string[10] = "NO"; 
  
  
  while (prim != NULL)
    {
      if ((prim != NULL) && (strcmp (prim->predecesor, string) == 0) && (i != 1)) 
	{
	  Destruye_Nodo(&(aux1->sig));
	  aux1 = listafinal->sig;
	  prim = aux1->sig;
	  numnodos--;
	}
      else
	{
	  prim = prim->sig;
	}
      i++;
    }
  
  return numnodos; 
}

NODO * ultima(NODO * listafinal, char * b) 
{
  NODO * aux = NULL; 
  aux = listafinal;
  
  while ((strcmp (aux->ip, b)) != 0) 
    {
      listafinal = listafinal->sig; 
      aux = listafinal; 
    }
  
  return aux; 
}

/*Funciones que destruyen nodos simples de la lista*/

void Destruye_Nodo(NODO **pp)
{
  NODO *aux = *pp;
  if (aux != NULL)
    {
      *pp = aux->sig;
      free(aux);
    }
}

void Destruye_Enlace(ENLACE **pp)
{
  ENLACE *aux = *pp;
  if (aux != NULL)
    {
      *pp = aux->sig;
      free(aux);
    }
}

/*Funcion que devuelve 1 si encuentra algún nodo con el campo predecesor
  igual que la cadena recibida.*/

int encontrado (NODO *listafinal, char * ip, int * numnodos)
{
  int found= 0;
  
  while (listafinal != NULL && !found)
    {
      if ((strcmp (listafinal->predecesor, ip)) == 0)
	found = 1;
      else
	listafinal = listafinal->sig;
    }
  if (found == 0) 
    {
      (*numnodos)--;
    }
  return found;
}

/*Funcion que comprueba si todas las cadenas y pesos se encuentran en 
  los limites establecidos.*/

int comprueba (char * origen, char * destino, int peso, int control2) 
{
  int error = 0;
  int num1 = 0;
  int num2 = 0;
  int num3 = 0;
  int num4 = 0;
  int num5 = 0;
  int control1 = 0; 
  
  if (control2 != 3) 
    {
      error = 3; 
    }
  else 
    {
      
      if ((strcmp (origen, destino)) != 0)
	{
	  control1 = sscanf (origen, "%d.%d.%d.%d.%d", &num1, &num2, &num3, &num4, &num5); 
	  if (control1 != 4)
	    {
	      error = 3;
	    }
	  else 
	    {
	      if ((num1 >=0) && (num2 <= 255)&&(num2 >=0) && (num2 <= 255)&&
		  (num3 >=0) && (num3 <= 255)&& (num4 >=0) && (num4 <= 255))
		{
		  error = 0;
		}
	      else 
		error = 3; 
	    }
	}
      if (error == 0) 
	{
	  control1 = sscanf (destino, "%d.%d.%d.%d.%d", &num1, &num2, &num3, &num4, &num5); 
	  if (control1 != 4)
	    {
	      error = 3;
	    }
	  
	  else 
	    {
	      if ((num1 >=0) && (num2 <= 255)&&(num2 >=0) && (num2 <= 255)&&
		  (num3 >=0) && (num3 <= 255)&& (num4 >=0) && (num4 <= 255))
		{
		  error = 0;
		}
	      else 
		error = 3; 
	    }
	  if ((peso < 0) || (peso > 1024)) 
	    {
	      error = 3;
	    }
	}
    }
  return error; 
}

int comprueba2 (char * origen, char * destino, ENLACE * aux)
{
  
  int error = 0; 
  ENLACE * aux2 = NULL; 
  
  aux2 = aux->sig;  
  
  while (aux2 != NULL && error == 0) 
    {
      if (((strcmp (origen, aux2->origen)) == 0) && ((strcmp (destino, aux2->destino)) == 0))
	{
	  error = 3; 
	}
      else
	{
	  aux2 = aux2->sig; 
	}
    }
  return error; 
}

/*Funciones que liberan listas enteras de nodos simples o enlaces.*/

void Libera_Enlace(ENLACE ** primero, int numenlaces) 
{
  ENLACE  *aux = NULL; 
  int i = 1;
  
  while (i <= numenlaces) 
    {
      aux = *primero;
      *primero = aux->sig;
      free(aux);
      i++;
    } 
}

void Libera_Nodo(NODO ** primero, int numnodos, NODO * ultimo) 
{
  NODO *aux = NULL; 
  NODO *aux2 = NULL; 
  int i = 1;
  
  if (numnodos == 2)
    {  
      aux2 = *primero;  
      free (aux2); 
      free (ultimo); 
    }
  else 
    {
      while (i <= numnodos) 
	{
	  aux = *primero;
	  *primero = aux->sig;
	  free(aux);
	  i++;
	}
    }
}


