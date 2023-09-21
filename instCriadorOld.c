#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MAX_ENC 6
#define MAX_WEIGHT 8
#define MIN_WEIGHT 3
#define MAX_CARGO 10

int **paletes, *nEPaletes, *destinos, **itens, *nEItens, *pItens, *pesos, **centralizadoras, *nECent, *paleteCent, **prodItem, *nEProd;
int *prodPalete;
int qEncomenda, qItem, qDest, qCent, qProd, T, cPalete, qPalete, erro;
void (*f_1)(int **, int *, int);
void (*f_2)(int *, int *, int, int);

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
  
void bubbleSort(int *v, int n){ 
    if (n < 1)return; 
 
    for (int i=0; i<n; i++) 
        if (v[i] > v[i+1]) 
            swap(&v[i], &v[i+1]);  
    bubbleSort(v, n-1); 
} 


void prepPalete(int **ret, int *ls ,int sizeLs){
    for(int i= 1; i < sizeLs; i++){
        *ret[i-1] = ls[i] - ls[i-1];
        nEPaletes[i-1] = *ret[i-1];
        paletes[i-1] = malloc((*ret[i-1]) * sizeof(int));
    }

}

void montaPalete(int *ret, int *aux, int n, int m){
    shuffle(aux,m);
    int k = 0;
    for(int i = 0; i<n; i++){
        for(int j = 0; j < ret[i]; j++){
            paletes[i][j] = aux[k];
            k++;
        }

    }

}

void prepItem(int **ret, int *ls, int sizeLs){
    for(int i= 1; i < sizeLs; i++){
        *ret[i-1] = ls[i] - ls[i-1];
        nEItens[i-1] = *ret[i-1];
        itens[i-1] = malloc((*ret[i-1]) * sizeof(int));
    }
}

void montaItens(int *ret, int *aux, int n, int m){
    int k = 0;
    for(int i = 0; i<n; i++){
        for(int j = 0; j < ret[i]; j++){
            //printf("ahn %d, %d\n", j, k);
            itens[i][j] = aux[k];
            k++;
        }

    }
}

void geraP(int n, int m){
    int sizeLs, *ls, *ret, *aux;
    sizeLs = 1;
    ls = malloc((1+n) * sizeof(int));
    ret = malloc(n * sizeof(int));
    aux = malloc((m) * sizeof(int));

    while(sizeLs < n ){
        int c = (rand() % (m - 1)) + 1;
        int encontrado = 0;
        for(int i=1; i<sizeLs; i++){
            if(ls[i] == c){
                encontrado = 1;
                break;
            }
        }
        if (encontrado == 0){
            ls[sizeLs] = c;
            sizeLs++;
        }        
    }

    bubbleSort(ls, sizeLs-1);
    ls[sizeLs] = m;
    sizeLs++;

    for(int i= 1; i < sizeLs; i++){
        ret[i-1] = ls[i] - ls[i-1];
        nEPaletes[i-1] = ret[i-1];
        paletes[i-1] = malloc((ret[i-1]) * sizeof(int));
    }

    for(int i = 0; i< m; i++){
        aux[i] = i+1;
    }


    f_2(ret,aux,n,m);

    free(aux);
    free(ret);
    free(ls);
}

void geraI(int n, int m){
    erro = 0;
    int sizeLs, *ls, *ret, *aux;
    sizeLs = 1;
    ls = malloc((1+n) * sizeof(int));
    ret = malloc(n * sizeof(int));
    aux = malloc((m) * sizeof(int));

    while(sizeLs < n ){
        int c = (rand() % (m - 1)) + 1;
        int encontrado = 0;
        for(int i=1; i<sizeLs; i++){
            if(ls[i] == c){
                encontrado = 1;
                break;
            }
        }
        if (encontrado == 0){
            ls[sizeLs] = c;
            sizeLs++;
        }        
    }

    bubbleSort(ls, sizeLs-1);
    ls[sizeLs] = m;
    sizeLs++;

    for(int i= 1; i < sizeLs; i++){
        ret[i-1] = ls[i] - ls[i-1];
        if(ret[i-1] > m){
            fprintf(stderr,"Houve Erro.\n");
            
            free(aux);
            free(ret);
            free(ls);
            erro = 1;
            return;
        }
        nEItens[i-1] = ret[i-1];
        itens[i-1] = malloc((ret[i-1]) * sizeof(int));
    }

    for(int i = 0; i< m; i++){
        aux[i] = i+1;
    }


    f_2(ret,aux,n,m);

    free(aux);
    free(ret);
    free(ls);
}


void geraDest(){

    int nDest = 1;
    for(int i = 0; i < qPalete; i++){
        destinos[i] = (rand() % (nDest)) + 1;
        if(destinos[i] == nDest){
            nDest++;
        }
    }
    qDest = nDest - 1;
}

void geraProd(){
    int nProd = 1;
    for(int i = 0; i < qItem; i++){
        pItens[i] = (rand() % (nProd)) + 1;
        if(pItens[i] == nProd){
            nProd++;
        }
    }
    qProd = nProd - 1;
}

int pesoMax(){
    //Soma os pesos de todas as encomendas em cada palete e define a capacidade do palete como o maior peso entre eles
    int peso = 0;
    for (int i = 0; i < qPalete; i++){
        if (nEPaletes[i] > peso){
            peso = nEPaletes[i];
        }
    }
    return peso;
}

int pesoRand(){
    //soma os pesos de todas as encomendas em cada palete e define a capacidade do palete como no minimo o maior peso entre eles
    int peso;
    peso = (rand() % (10)) + pesoMax();
    return peso;
}

int pesoPalete(int mode){
    //if (mode == 1){
        cPalete = pesoMax();
    //}
    //else{
    //    cPalete = pesoRand();
    //}
    return mode;
}

void debugPrint(int mode){

    //Alterar essa function
    printf("qEncomenda = %d\n",qEncomenda);
    printf("qPalete = %d\n",qPalete);
    printf("Encomendas em cada palete: \n");
    for(int i = 0; i < qPalete; i++){
        printf("Palete %d:\n", i+1);
        for(int j = 0; j < nEPaletes[i]; j++){
            printf("%d\n",paletes[i][j]);
        }
        printf("-----------------\n");
    }
    printf("Numero de Destinos: %d\n", qDest);
    printf("Destinos de cada palete: \n");
    for(int i = 0; i < qPalete; i++){
        printf("Palete %d: %d\n", i+1, destinos[i]);
    }
    printf("----------------\n");
    printf("Capacidade do Palete: %d\n", cPalete);
    printf("Modo: %d\n", mode);
    printf("Numero de Itens: %d\n", qItem);
}

void oldProc(int argc, char *argv[]){
    int mode;
    mode = atoi(argv[1]);
    time_t t;
    srand((unsigned) time(&t));

    qEncomenda = (rand() % (MAX_ENC)) + 3;
    qPalete = (rand() % (qEncomenda)) + 1;

    paletes = malloc((qPalete) * sizeof(int *));
    destinos = malloc((qPalete) * sizeof(int));

    nEPaletes = malloc(qPalete * sizeof(int));
    f_1 = prepPalete;
    f_2 = montaPalete;
    geraP(qPalete,qEncomenda);

    geraDest();

    mode = pesoPalete(mode);

    if(mode == 1){
        // apenas 1 item, 1 produtora, tempo hábil calculado de forma que tudo pode ser entregue
        qProd = 1;
        qItem = 1;
        qCent = 1;
    }
    else if(mode == 2){
        //aleatorização de item, produtora e tempo hábil de forma que tudo pode ser entregue
        //Separar as encomendas em itens
        qItem = (rand() % (qEncomenda)) + 1;
        itens = malloc((qItem) * sizeof(int *));
        nEItens = malloc((qItem) * sizeof(int));
        f_1 = prepItem;
        f_2 = montaItens;
        geraI(qItem,qEncomenda);
        if(erro){
            for(int i = 0; i < qPalete; i++){
                free(paletes[i]);
            }
            free(paletes);
            free(destinos);
            free(nEPaletes);
            if(mode == 2){
                for (int i =0; i < qItem; i++){
                    free(itens[i]);
                }
                free(itens);
                free(nEItens);
            }
            exit(1);            
        }
        pItens = malloc((qItem) * sizeof(int));
        qCent = qDest;
        geraProd();
    }

    T = qEncomenda;

    #ifdef DEBUG
    debugPrint(mode);
    #endif

//  validar instancia antes de imprimir

    printOutput();

    for(int i = 0; i < qPalete; i++){
        free(paletes[i]);
    }
    free(paletes);
    free(destinos);
    free(nEPaletes);
    if(mode == 2){
        for (int i =0; i < qItem; i++){
            free(itens[i]);
        }
        free(itens);
        free(nEItens);
        free(pItens);
    }
}