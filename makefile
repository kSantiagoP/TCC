all: purge criaInst 

debug: instCriador.c purge
	gcc instCriador.c -o criaInst -lm -DDEBUG

englobado: instCriador.c purge
	gcc instCriador.c -o criaInst -lm -DGLOB

criaInst: instCriador.c
	gcc instCriador.c -o criaInst -lm

clean: 
	rm -f *.o

purge: clean
	rm -f criaInst