#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define MAX_ENC 6

int **paletes, *nEPaletes, *destinos;
int qEncomenda, qItem, qDest, qCent, qProd, T, cPalete, qPalete;


void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

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

void geraS(int n, int m){
    int sizeLs, *ls, *ret, *aux;
    sizeLs = 1;
    ls = malloc((1+n) * sizeof(int));
    ret = malloc(n * sizeof(int));
    nEPaletes = malloc(n * sizeof(int));
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

  shuffle(aux,m);
    int k = 0;
    for(int i = 0; i<n; i++){
    //    printf("{");

        for(int j = 0; j < ret[i]; j++){
        //    printf("%d",aux[k]);
            paletes[i][j] = aux[k];
            if(j != ret[i] - 1){
        //        printf(",");
            }
            k++;
        }
        //printf("}");
        if(i != n-1){
        //    printf(",");
        }

    }

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
    if (mode == 1){
        cPalete = pesoMax();
    }
    else{
        cPalete = pesoRand();
    }
    return mode;
}

int main(){
    int mode;

    //gere uma solução válida ( numero de encomendas, numero de paletes usados, quais encomendas vão aonde) GERAS pode fazer isso
    //a partir disso retornar até solução
        //gerar numero de destinatarios bem como seus respectivos paletes, centralizadoras = destinatarios, e por consequencia os destinos de cada encomenda
        //gerar peso de encomendas, criar algum limite superior
        //Gerar capacidade do palete baseado na solucao final de forma que uma solução mais otimizada não seja possivel
        //Gerar quantidade de itens, produtoras e tempo disponível de forma coerente com a solução final - etapa mais dificil ---
    time_t t;
    srand((unsigned) time(&t));

    qEncomenda = (rand() % (MAX_ENC)) + 3;
    qPalete = (rand() % (qEncomenda)) + 1;

    paletes = malloc((qPalete) * sizeof(int *));
    destinos = malloc((qPalete) * sizeof(int));


    geraS(qPalete,qEncomenda);
    //GERAR DESTINATARIO DE CADA PALETE E A QUANTIDADE DE DESTINATARIOS
    geraDest();
    //GERAR PESO DE CADA ENCOMENDA (Por enquanto, todos pesam 1)
    //GERAR MAX PESO DE CADA PALETE (versao 1)
    mode = pesoPalete(1);
    //GERAR ITENS, PRODUTORAS E TEMPO HÁBILA (versao 2)
    if(mode == 1){
        // apenas 1 item, 1 produtora, tempo hábil calculado de forma que tudo pode ser entregue
        qProd = 1;
        qItem = 1;
        T = qEncomenda;
        qCent = 1;
    }
    else{
        //aleatorização de item, produtora e tempo hábil de forma que tudo pode ser entregue

    }

    #ifdef DEBUG
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
    #endif


    for(int i = 0; i < qPalete; i++){
        free(paletes[i]);
    }
    free(paletes);
    free(destinos);
    free(nEPaletes);
}