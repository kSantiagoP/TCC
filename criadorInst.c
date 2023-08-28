#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


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


void geraS(int item, int enc){
    int sizeLs, *ls, *ret, *aux;
    sizeLs = 1;
    ls = malloc((1+item) * sizeof(int));
    ret = malloc(item * sizeof(int));
    aux = malloc((enc) * sizeof(int));

    while(sizeLs < item ){
        int c = (rand() % (enc - 1)) + 1;
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
    ls[sizeLs] = enc;
    sizeLs++;

    for(int i= 1; i < sizeLs; i++){
        ret[i-1] = ls[i] - ls[i-1];
    }

    for(int i = 0; i< enc; i++){
        aux[i] = i+1;
    }

    shuffle(aux,enc);
    int k = 0;
    for(int i = 0; i<item; i++){
        printf("{");

        for(int j = 0; j < ret[i]; j++){
            printf("%d",aux[k]);
            if(j != ret[i] - 1){
                printf(",");
            }
            k++;
        }
        printf("}");
        if(i != item-1){
            printf(",");
        }

    }

    free(aux);
    free(ret);
    free(ls);
}

void geraC(int cent){
    int *aux;
    aux = malloc(cent * sizeof(int));

    for (int i = 0; i < cent; i++){
        aux[i] = i+1;
    }

    shuffle(aux, cent);
    
    for (int i = 0; i< cent; i++){

        printf("{%d}", aux[i]);

        if(i != cent -1){
        printf(",");
        
        }
    }

    free(aux);
}

int main(){
    time_t t;
    srand((unsigned) time(&t));
    int qEncomenda, qItem, qDest, qCent, qProd, T, cPalete;

    scanf("%d", &qEncomenda);
    scanf("%d", &cPalete);
    scanf("%d", &qProd);

    qItem = (rand() % (qEncomenda)) + 1;
    qDest = (rand() % (qEncomenda)) + 1;
    T = qEncomenda;
    qCent = qDest;

    printf("qtd_Itens           = %d;\n",qItem);
    printf("qtd_Encomendas           = %d;\n",qEncomenda);
    printf("qtd_Destinatarios           = %d;\n",qDest);
    printf("qtd_Centralizadoras         = %d;\n",qCent);
    printf("qtd_Produtores           = %d;\n",qProd);
    printf("T           = %d;\n",T);
    printf("CapacidadePalete           = %d;\n",cPalete);

    printf("dest = [");
    for (int i = 0; i < qEncomenda; i++){
        printf("%d",(rand() % (qEncomenda)) + 1);
        if ( i < qEncomenda - 1){
            printf(",");
        }
    }
    printf("];\n");

    printf("peso = [");
    for (int i = 0; i < qEncomenda; i++){
        printf("%d",(rand() % (qEncomenda)) + 1);
        if ( i < qEncomenda - 1){
            printf(",");
        }
    }
    printf("];\n");

    printf("S = [");
    //Agrupa em subconjuntos
    geraS(qItem, qEncomenda);

    printf("];\n");

    printf("cent = [");
    geraC(qCent);
    printf("];\n");

    printf("qtd = [");
    //fixo, depende exclusivamente da quantidade de produtores e de itens
    // por ora fixo em 1 ou 0
    //PRODUTORES X ITENS ---> PRODUTORES LINHAS X ITENS COLUNAS

    for(int i = 0; i < qProd; i++){
        printf("|");
        for(int j = 0; j < qItem; j++){
            printf(" 1");
            if(j != qItem-1){
                printf(",");
            }else{
                printf(" ");
            }
        }
        if(i == qProd - 1){
            printf("|");
        }
    }
    printf("];\n");
}