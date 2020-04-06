#Makefile practica final estructuras de datos
#Manuel Lorente Almán (manloralm)
EJECUTABLE=proyecto
OBJETOS=proyecto.o funciones.o
OPCIONES= -g -W -Wall 
COMPILADOR=gcc

$(EJECUTABLE): $(OBJETOS)
	$(COMPILADOR) $(OPCIONES) -o $(EJECUTABLE) $(OBJETOS)

proyecto.o: proyecto.c ips.h
	$(COMPILADOR) $(OPCIONES) -c proyecto.c

funciones.o: funciones.c ips.h
	$(COMPILADOR) $(OPCIONES) -c funciones.c

clean:
	rm *.o

ejecuta:
	./proyecto