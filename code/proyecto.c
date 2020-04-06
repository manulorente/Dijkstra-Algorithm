/*
**	Fichero: proyecto.c	
**	Autor:Manuel Lorente Almán	
**	Fecha: 07-04-2013
**	
**	
**	Descripcion: Fichero principal de un programa que calcula el camino
**      (o ruta) de distancia mínima en una red de ordenadores, donde cada una
**      de las maquinas es un nodo.Para ello se utiliza un algoritmo que realiza
**      el cálculo del camino mínimo desde un nodo dado hasta cualquier otro
**      de la red de nodos.
*/

/*Includes del sistema*/

#include <stdio.h>//Contiene funciones de biblioteca y prototipos de C

#include <stdlib.h>//Libreria para la reserva dinámica de memoria

#include <string.h>//Libreria para el manejo de cadena de caracteres

/*Includes de la aplicación*/
#include "ips.h"

int main (int argc, char **argv)
{
  //Ficheros a utilizar para leer y escribir
  FILE * fent = NULL;  
  FILE * camino = NULL; 
  FILE * fsal = NULL;
  //String con la que almacenamos cada línea leida del fich conf
  char linea[TAM];
  //Cadenas auxiliares para el tratado de los datos
  char destino[TAM];
  char destino2 [TAM];
  char origen [TAM];
  char cadenaauxiliar [TAM];
  char * bandera = NULL;
  //Peso entre nodos de cada enlace
  int peso = 0;
  int i = 0; 
  //Contadores de enlaces y nodos simples
  int numenlaces = 1;
  int numnodos = 1;
  //Variables enteras para el control del flujo del programa y errores
  int control = 0;
  int error = 0; 
  int control2 = 0;
  int memoria = 0;
  int fallo = 0;
  //Esctructuras de enlaces y nodos para desarrollar el algoritmo
  ENLACE * enlaces = NULL;
  ENLACE * enlacesauxiliar = NULL;
  ENLACE * enlacesaux = NULL; 
  ENLACE *anterior = NULL; 
  NODO * listafinal = NULL;
  NODO * nodos = NULL; 
  NODO * enlacesaux2 = NULL; 
  NODO * ultimo = NULL;
  
  if (argc == 4)
    {
      fent = fopen (argv [1], "r");
      camino = fopen (argv [2], "r"); 
      fsal = fopen (argv [3], "w"); 
      if (fent != NULL)
	{
	  if (camino != NULL) 
	    {
	      if (fsal != NULL)
		{
		  /*Antes de crear ningún enlace con la línea leida comprobaremos siempre que sea correcta
		    en caso de no ser correcta el programa determinará el error correspondiente y no seguirá 
		  ejecutándose.*/
		  fgets (linea, TAM, fent); 
		  control2 = sscanf (linea, "%s %s %d", destino2, destino, &peso); 
		  error = comprueba (destino2, destino, peso, control2); 
		  if (error == 0)
		    {
		      enlaces = Crea_Enlaces (peso, &memoria);
		      /*En el caso de no haber error de memoria ni de fichero procedemos a rellenar el 
			enlace creado con los campos de información leidos y procesados.*/
		      if (memoria == 0)
			{
			  fscanf(camino, "%s", origen);  
			  fscanf(camino, "%s", cadenaauxiliar);
			  strcpy (enlaces->origen, destino2); 
			  strcpy (enlaces->destino, destino);
			  /*Establecemos "enlaces" como lista principal de enlaces y copiamos la referencia 
			    a punteros auxiliares para tratarlos y nunca perder la referencia de la lista*/
			  enlacesaux = enlaces;
			  anterior = enlaces;
			  
			  while (!feof (fent) && (error == 0) && (memoria == 0))
			    {
			      /*Con este bucle leemos el fichero de configuración entero, comprobando línea
				a línea de forma que si alguna linea es incorrecta se detiene el programa*/
			      fgets (linea, TAM, fent); 
			      control2 = sscanf (linea, "%s %s %d", destino2, destino, &peso);
			      if (control2 != -1) 
				{
				  /*Esta ejecución comprueba que la línea sea correcta y tenga un caracter
				    de nueva linea*/
				  bandera = strchr (linea, '\n');
				  error = comprueba (destino2, destino, peso, control2);
				  enlacesauxiliar = Crea_Enlaces (peso, &memoria);
				  strcpy (enlacesauxiliar->origen, destino2);
				  strcpy (enlacesauxiliar->destino, destino);
				  /*Siempre vamos enlazando y creando auxiliares que van modificando la lista
				    original pero no perderemos nunca la referencia del primer nodo*/
				  enlaza (enlacesauxiliar, &enlacesaux); 
				  if ( i!= 0) 
				    {
				      anterior = anterior->sig;
				    }
				  numenlaces++;
				  i++;
				}
			    }
			  
			  if (memoria == 0) 
			    {
			      /*Procedemos a borrar los nodos de la lista de enlaces que no nos sirvan
				en el caso del algoritmo*/
			      if ((bandera != NULL) && ((strcmp(anterior->origen,enlacesaux->origen)) == 0) 
				  &&  ((strcmp(anterior->destino,enlacesaux->destino)) == 0))
				{
				  Destruye_Enlace(&(anterior->sig));
				  numenlaces--;
				}
			      enlacesaux = enlaces;
			      /*Realizamos un segundo chequeo de la lista de enlaces para comprobar
				que todo esta correcto*/
			      while ( error == 0 && enlacesaux != NULL) 
				{
				  error = comprueba2 (enlacesaux->origen, enlacesaux->destino, enlacesaux);
				  enlacesaux = enlacesaux->sig; 
				}
			      
			      if (error == 0) 
				{
				  enlacesaux = enlaces;  
				  /*A partir de la lista de enlaces, en el caso de que no hubiese error,
				    crearemos una lista de nodos simples con los nodos individuales que forman
				    la red la cual trataremos posteriormente para calcular la ruta correcta*/
				  listafinal = Crea_Nodo (&memoria); 
				  if (memoria == 0) 
				    {
				      strcpy(listafinal->ip, origen); 
				      enlacesaux2 = listafinal; 
				      /*Recorreremos toda la lista de enlaces comprobando que no haya errores
					de memoria, comparando cada nodo de enlace con los nodos a creados
					para no repetir ninguno y crearemos nuestra lista de nodos simples definitiva*/
				      for (i=1; ((i<numenlaces) && (enlacesaux != NULL) && (memoria == 0)); i++) 
					{
					  control = compara1 (enlacesaux, listafinal); 
					  
					  if (control != 0) 
					    {
					      nodos = Crea_Nodo (&memoria); 
					      strcpy (nodos->ip, enlacesaux->origen); 
					      enlaza2 (nodos, &enlacesaux2); 
					      numnodos++; 
					    }
					  
					  control = compara2 (enlacesaux, listafinal); 
					  if (control != 0) 
					    {
					      nodos = Crea_Nodo (&memoria); 
					      strcpy (nodos->ip, enlacesaux->destino);
					      enlaza2 (nodos, &enlacesaux2); 
					      numnodos++; 
					    }
					  enlacesaux = enlacesaux->sig; 
					}
				      /*Una vez creada la lista de nodos simples, la recorremos hasta llegar
					al nodo origen de la ruta dada en el fichero de entrada*/
				      ultimo = ultima (listafinal, cadenaauxiliar);
				      
				      if (memoria == 0) 
					{ 
					  /*Una vez comprobado que todo está ok y que no hay errores procederemos a
					    preparar la lista para que el algoritmo cálcule la ruta y la imprima en el
					    fichero de salida*/
					  
					  Inicializa_Nodos (listafinal, numnodos);
					  Encuentra_Ruta(enlaces, listafinal, numenlaces, numnodos); 
					  numnodos = Imprime_Camino(listafinal, numnodos, fsal, origen, cadenaauxiliar);
					  
					  /*Liberamos las listas creadas anteriormente y cerramos ficheros ya procesados*/
					  Libera_Enlace (&enlaces, numenlaces);  
					  Libera_Nodo (&listafinal, numnodos, ultimo); 
					  
					  fclose (fent); 
					  fclose (camino); 
					  fclose (fsal); 
					}
				      /*Si se ha producido algun error, tenemos una funcion que se encarga
					de gestionarlo con su correspondiente mensaje.*/
				      else 
					{
					  Gestiona_Errores(6);
					  fallo = 1;
					}
				    }
				  else 
				    {
				      Gestiona_Errores(6);
				      fallo = 1;
				    }
				}
			      else 
				{
				  /*En el caso de haber error en el fichero de configuracion 
				    liberamos la memoria previamente reservada al ser inutil*/
				  Gestiona_Errores(3);
				  Libera_Enlace (&enlaces, numenlaces);
				}
			    }
			  else 
			    {
			      Gestiona_Errores(6);
			      fallo = 1;
			    }
			}
		      else 
			{
			  Gestiona_Errores(6);
			  fallo = 1;
			}
		    }
		  else
		    {
		      Gestiona_Errores(3);
		      fallo = 1;
		    }
		}
	      else
		{
		  Gestiona_Errores(5);
		  fallo = 1;
		}
	    }
	  else
	    {
	      Gestiona_Errores(4);
	      fallo = 1;
	    }
	}
      else 
	{
	  Gestiona_Errores(2); 
	  fallo = 1;
	}
    }
  else 
    {
      Gestiona_Errores(1); 
      fallo = 1;
    }
  /*Valor de retorno del programa, 0 si el programa se ha ejecutado correctamente
    y 1 si se ha producido algun error*/

  return fallo;
}
