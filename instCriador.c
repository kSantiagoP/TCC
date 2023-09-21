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
int *prodPalete, *itemProd;
int qEncomenda, qItem, qDest, qCent, qProd, T, cPalete, qPalete, erro;


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

void printPalete(){
    for(int i = 0; i < qPalete; i++){
        fprintf(stderr,"palete %d: ", i);
        for(int j = 0; j < nEPaletes[i]; j++){
            fprintf(stderr,"%d ", paletes[i][j]);
        }
        fprintf(stderr,"\n");
    }

}

void printOutput(){
    printPalete();
    printf("qtd_Itens           = %d;\n",qItem);
    printf("qtd_Encomendas           = %d;\n",qEncomenda);
    printf("qtd_Destinatarios           = %d;\n",qDest);
    printf("qtd_Centralizadoras         = %d;\n",qCent);
    printf("qtd_Produtores           = %d;\n",qProd);
    printf("T           = %d;\n",qEncomenda);
    printf("CapacidadePalete           = %d;\n",MAX_CARGO);

    printf("dest = [");
    for (int i = 0; i < qEncomenda; i++){
        printf("%d", destinos[i]+1);
        if(i < qEncomenda - 1){
            printf(",");
        }
    }
    printf("];\n");

    printf("peso = [");
    for(int i = 0; i < qEncomenda; i++){
        printf("%d",pesos[i]);
        if(i < qEncomenda - 1){
            printf(",");
        }
    }
    printf("];\n");

    printf("S = [");
    for(int j = 0; j < qItem; j++){
        printf("{");
        for(int k = 0; k < nEItens[j]; k++){
            printf("%d", itens[j][k]);
            if ( k < nEItens[j] - 1){
                printf(",");
            }
        }
        printf("}");
        if(j < qItem - 1){
            printf(",");
        }

    }
    printf("];\n");

    printf("cent = [");
    for(int j = 0; j < qCent; j++){
        printf("{");
        for(int k = 0; k < nECent[j]; k++){
            printf("%d", centralizadoras[j][k]+1);
            if ( k < nECent[j] - 1){
                printf(",");
            }
        }
        printf("}");
        if(j < qCent - 1){
            printf(",");
        }

    }
    printf("];\n");

    printf("qtd = [");
    for(int i = 0; i < qProd; i++){
        printf("|");
        for (int j = 0; j < qItem; j++){
            if( i == itemProd[j])
                printf("1");
            else
                printf("0");
            
            if(j < qItem - 1)
                printf(",");
        }
        if(i == qProd -1)
            printf("|");
    }

    printf("];\n");

    //imprimir centralizadoras
    //imprimir qtd
}

/*
Alterações: 
- Receber n paletes e n produtores de entrada
- Adicionar pesos de forma incremental a cada palete
- Paletes de capacidade randomizada entre duas constantes
- Cada palete eh construido necessariamente pelo mesmo produtor
- Uma vez enviado o palete deve ser entregue para centralizadora que necessariamente engloba os destinos apropriados
- Garantias: Um palete deve conter destinos de uma mesma centralizadora e membros de uma mesma produtora (check).
*/

int geraPalete(){
    int auxPalete[MAX_CARGO];
    int totalW;
    int numEnc;
    int newW;
    qEncomenda = 0;

    paletes = malloc( qPalete * sizeof(int *));
    if (!paletes)
        return 0;

    nEPaletes = malloc(qPalete * sizeof(int));
    if (!nEPaletes)
        return 0;
    //aloca o ponteiro de ponteiros referente aos paletes e seus respectivos membros
    //Se a alocaçao falhar retorne 0


    for (int i = 0; i < qPalete; i++){
        //gera um produto de peso entre 3 ~ 8 e joga no palete
        numEnc = 0;
        totalW = 0;
        while(totalW < MAX_CARGO - MIN_WEIGHT + 1){
            //gere um peso e atribua ao palete até sobrar no maximo 2 de espaço
            newW = MIN_WEIGHT + rand() % (MAX_WEIGHT - MIN_WEIGHT + 1);
            if (!((newW + totalW) > MAX_CARGO)){
                auxPalete[numEnc] = newW;
                totalW += newW;
                #ifdef DEBUG
                printf("%d ", auxPalete[numEnc]);
                #endif
                numEnc++;
            }
        }
        #ifdef DEBUG
        printf("\n");
       #endif
        //quando o palete estiver montado, aloque espaço para seus membros, atribua os valores e gere o proximo
        //se alocaçao falhar retorne zero
        paletes[i] = malloc( numEnc * sizeof(int));
        if(!paletes[i])
            return 0;
        
        nEPaletes[i] = numEnc;
        for(int j = 0; j < numEnc; j++){
            paletes[i][j] = auxPalete[j];
        }

        qEncomenda += numEnc;
    }

    #ifdef DEBUG
    printf("qEncomenda: %d \n", qEncomenda);
    #endif

    return 1;
}

//quando será definido os ids de cada encomenda no palete?
    //assim que soubermos quantas elas sao
//quando serão definidos os destinos de cada encomenda no palete?
    //quando definirmos as centralizadoras. cada palete tem que ir para uma delas

int geraId(){
    
    int *auxId, n;
    auxId = malloc(qEncomenda * sizeof(int));
    if (!auxId)
        return 0;
    
    pesos = malloc(qEncomenda * sizeof(int));
    if (!pesos)
        return 0;

    for (int i = 0; i < qEncomenda; i++){
        auxId[i] = i + 1;
    }

    shuffle(auxId, qEncomenda);

    n = 0;
    for(int i = 0; i < qPalete; i++){
        for(int j = 0; j < nEPaletes[i]; j++){
            pesos[auxId[n]-1] = paletes[i][j];
            paletes[i][j] = auxId[n];
            n++;
        }
    }

    #ifdef DEBUG
   printf("Paletes: \n");
   for(int i = 0; i < qPalete; i++){
        printf("palete %d: ", i);
        for(int j = 0; j < nEPaletes[i]; j++){
            printf("%d ", paletes[i][j]);
        }
        printf("\n");
    }

    printf("Pesos: \n");
    for (int i = 0; i < qEncomenda; i++){
        printf("%d ", pesos[i]);
    }
    printf("\n");
    #endif

    free(auxId);

    return 1;
}

int geraC(){
    //Primeiro definimos os destinos contemplados por cada centralizadora
    //quantidade de destinos deve ser maior ou igual ao de centralizadoras
    //shuffle os ids dos destinos, distribua entre centralizadoras

    int *auxId;

    centralizadoras = malloc(qCent * sizeof(int *));
    if( !centralizadoras)
        return 0;
    
    nECent = malloc(qCent * sizeof(int));
    if(!nECent)
        return 0;
    memset(nECent,0,qCent);

    int maxDestinos = ceil((float) qDest / (float) qCent);

    for (int i = 0; i < qCent; i++){
        centralizadoras[i] = malloc(maxDestinos * sizeof(int));
        if(!centralizadoras[i])
            return 0;
        memset(centralizadoras[i],0,maxDestinos);
    }

    auxId = malloc(qDest * sizeof(int));
    if(!auxId)
        return 0;
    
    for (int i = 0; i < qDest ; i++){
        auxId[i] = i;
    }

    shuffle(auxId,qDest);

    int n = 0;
    int m = 0;
    for(int i = 0; i < qDest; i++){
        centralizadoras[n][m] = auxId[i];
        nECent[n]++;
        n = (n + 1) % qCent;
        if(n == 0)
            m = (m + 1) % maxDestinos; //nao eh pra acontecer, mas por seguranca
    }


    free(auxId);

    return 1;
}

//A partir de agora, distribuimos paletes entre as centralizadoras, de forma a atribuir destinos para cada encomenda nela contemplada.
int distribuiPalete(){
    
    int *auxId;
    auxId = malloc(qPalete * sizeof(int));
    if(!auxId)
        return 0;
    
    for(int i = 0; i < qPalete; i++){
        auxId[i] = i;
    }
    
    shuffle(auxId,qPalete);

    paleteCent = malloc(qPalete * sizeof(int));
    if(!paleteCent)
        return 0;
    
    int n = 0;
    for(int i = 0; i < qPalete; i++){
        paleteCent[auxId[i]] = n;
        n = (n + 1) % qCent;
    }

    #ifdef DEBUG
    printf("Centralizadoras de cada Palete: \n");
    for(int i = 0; i < qPalete; i++){
        printf("%d ", paleteCent[i] + 1);
    }
    printf("\n");
    #endif

    destinos = malloc(qEncomenda * sizeof(int));
    if(!destinos)
        return 0;

    for(int i = 0; i < qPalete; i++){
        //percorra todos os paletes
        //para cada encomenda nele atribua um destino possivel dentro da centralizadora que cuida do palete

        for(int j = 0; j < nEPaletes[i]; j++){
            destinos[paletes[i][j] - 1] = centralizadoras[paleteCent[i]][rand() % nECent[paleteCent[i]]]; //tem algo errado nessa linha
        }

    }

    #ifdef DEBUG
    printf("Destinos: \n");
    for(int i = 0; i < qEncomenda; i++){
        printf("%d ", destinos[i] + 1);
    }
    printf("\n");
    #endif

    free(auxId);
    return 1;
}
//Ainda precisamos definir as produtoras e seus respectivos itens

int decideProdutora();
int atribuiItem();

//agora definimos os produtores, seus itens e quais encomendas pertencem a qual conjunto
int geraPI(){

    //cada palete tem exclusivamente 1 produtora e 1 centralizadora
    // Cada item é produzido por uma produtora (em uma relacao de sobrejetividade)

    int *auxId;
    auxId = malloc(qItem * sizeof(int));
    if(!auxId)
        return 0;

    prodItem = malloc(qProd * sizeof(int *));
    if(!prodItem)
        return 0;

    int maxItemProd;
    maxItemProd = ceil((float) qItem / (float) qProd);

    for (int i = 0; i < qProd; i++){
        prodItem[i] = malloc(maxItemProd * sizeof(int));
        if(!prodItem[i])
            return 0;
    }

    itemProd = malloc(qItem * sizeof(int));
    if(!itemProd)
        return 0;

    nEProd = malloc(qProd * sizeof(int));
    if(!nEProd)
        return 0;
    memset(nEProd,0,qProd);

    for (int i = 0; i < qItem; i++){
        auxId[i] = i;
    }

    shuffle(auxId, qItem);

    int n = 0;
    int m = 0;


    for(int i = 0; i < qItem; i++){
        prodItem[n][m] = auxId[i];
        nEProd[n]++;
        itemProd[auxId[i]] = n;
        n = (n + 1) % qProd;
        if(n == 0)
            m = (m + 1) % maxItemProd;
    }

    #ifdef DEBUG
    printf("Produtoras e seus respectivos itens \n");
    for(int i = 0; i < qProd; i++){
        printf("Produtora %d: ",i + 1);
        for(int j = 0; j < nEProd[i]; j++){
            printf("%d ",prodItem[i][j] + 1);
        }
        printf("\n");
    }
    #endif

    free(auxId);
    return decideProdutora();
}

int decideProdutora(){

    prodPalete = malloc(qPalete * sizeof(int));
    if(!prodPalete)
        return 0;
    
    int n = rand() % qProd;
    for(int i = 0; i < qPalete; i++){
        prodPalete[i] = n;
        n = (n + 1) % qProd;
    }

    #ifdef DEBUG
    printf("Produtoras de cada palete:\n");
    for(int i = 0; i < qPalete; i++){
        printf("Palete %d: %d\n", i + 1, prodPalete[i] + 1);
    }
    #endif
    
    return atribuiItem();
}

int atribuiItem(){

    itens = malloc(qItem * sizeof(int *));
    if(!itens)
        return 0;

    for (int i = 0; i < qItem; i++){
        itens[i] = malloc(qEncomenda * sizeof(int));
        if(!itens[i])
            return 0;
    }

    nEItens = malloc(qItem * sizeof(int));
    if(!nEItens)
        return 0;

    memset(nEItens,0,qItem);    

    for (int i = 0; i < qPalete; i++){
        for (int j = 0; j < nEPaletes[i]; j++){
            int chaveItem = prodItem[prodPalete[i]][ rand() % nEProd[prodPalete[i]]];
            itens[chaveItem][nEItens[chaveItem]] = paletes[i][j];
            nEItens[chaveItem]++;
        }
    }

    #ifdef DEBUG
    printf("Itens e encomendas: \n");
    for(int i = 0; i < qItem; i++){
        printf("Item %d: ", i+1);
        for(int j = 0; j < nEItens[i]; j++){
            printf("%d ", itens[i][j]);
        }
        printf("\n");
    }
    #endif

    #ifdef DEBUG
    printf("Produtoras e seus respectivos itens \n");
    for(int i = 0; i < qProd; i++){
        printf("Produtora %d: ",i + 1);
        for(int j = 0; j < nEProd[i]; j++){
            printf("%d ",prodItem[i][j] + 1);
        }
        printf("\n");
    }
    #endif

    return 1;
}


int validaEntrada(){
    if( qPalete < 1 || qProd < 1 || qItem < 1 || qCent  < 1 || qDest < 1){
        fprintf(stderr, "Insira valores inteiros \n");
        return 0;
    }

    if(qDest < qCent){
        fprintf(stderr,"Numero de Destinos deve ser maior ou igual ao numero de centralizadoras \n");
        return 0;
    }

    if(qPalete < qProd){
        fprintf(stderr, "Numero de Paletes deve ser maior ou igual ao numero de produtoras \n");
        return 0;
    }

    return 1;
}

void limpaDados(){
    if(paletes){
        for(int i = 0; i < qPalete; i++){
            if(paletes[i])
                free(paletes[i]);
        }
        free(paletes);
    }

    if(nEPaletes)
        free(nEPaletes);

    if(centralizadoras){
        for(int i = 0; i < qCent; i++){
            if(centralizadoras[i])
                free(centralizadoras[i]);
        }
        free(centralizadoras);
    }

    if(nECent)
        free(nECent);

    if(paleteCent)
        free(paleteCent);
    
    if(destinos)
        free(destinos);

    if(itens){
        for(int i = 0; i < qItem; i++)
            if(itens[i])
                free(itens[i]);
        free(itens);
    }

    if(nEItens)
        free(nEItens);

    if(prodItem){
        for(int i = 0; i < qProd; i++)
            if(prodItem[i])
                free(prodItem[i]);
        free(prodItem);
    }    

    if(nEProd)
        free(nEProd);

    if(prodPalete)
        free(prodPalete);

    if(itemProd)
        free(itemProd);

    if(pesos)
        free(pesos);

}


//até completar o numero de paletes, aleatoriamente atribuir a cada um deles uma quantidade de encomendas até que não haja mais espaço 
    //- encomendas possuem um peso minimo diferente de 1 (podemos definir um limite minimo)
// para cada palete, definir uma centralizadora e uma produtora (?)
    //Cada centralizadora dá conta de um ou mais destinos diferentes de outras centralizadoras
    //portanto em um palete deve haver apenas encomendas que sejam de uma mesma produtora e cujos destinos sejam englobados pela mesma centralizadora
//Tempo e capacidade fixos
//qtd de Itens, destinos, qtd de centralizadoras randomizadas
int main(int argc, char *argv[]){

    time_t t;
    srand((unsigned) time(&t));

    if(argc < 6 || argc > 6){
        fprintf(stderr, "Usage: ./prog numPalete numProdutores numItens numCentralizadoras numDest\n");
        exit(1);
    }
    qPalete = atoi(argv[1]);
    qProd = atoi(argv[2]);
    qItem = atoi(argv[3]);
    qCent = atoi(argv[4]);
    qDest = atoi(argv[5]);

    if(!validaEntrada()){

        exit(1);
    }
    


    if(geraPalete() && geraId() && geraC() && distribuiPalete() && geraPI())
        printOutput();
    else
        fprintf(stderr,"Erro de Alocacao \n");

    limpaDados();

    return 0;
}