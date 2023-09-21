all: criaInst

criaInst: instCriador.c
	gcc instCriador.c -o criaInst -lm

clean: 
	rm -f *.o

purge: clean
	rm -f criaInst