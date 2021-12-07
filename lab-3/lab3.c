//encontrar a quantidade de valores de um vetor de numeros reais que
//estao dentro de uma faixa de valores dada

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include<time.h>

long long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
float *vetor; //vetor de entrada com dimensao dim
int limInferior; //variavel limite inferior
int limSuperior; //variavel limite superior


//fluxo das threads
void * tarefa(void * arg){
    long int id = (long int) arg; //identificador da thread
    long int *contadorLocal = 0; //variavel local de contagem de elementos
    long int tamBloco = dim/nthreads; //tamanho do bloco de cada thread
    long int inicial = id * tamBloco; //elemento inicial do bloco da thread
    long int final; //elemento final (nao-processado) do bloco da thread
    
    contadorLocal = (long int*) malloc(sizeof(long int));
    if(contadorLocal == NULL){
        fprintf(stderr, "ERRO --- malloc(contadorLocal)\n");
        exit(1);
    }
    
    if(id == nthreads-1){
        final = dim;
    }else{
        final = inicial+tamBloco; //trata o resto, se houver
    }

    //conta os elementos do bloco da thread
    for(long int i=inicial;i<final;i++){
        if(limInferior<vetor[i] && vetor[i]<limSuperior){
            (*contadorLocal)++;
        }
    }
    //retorna o resultado da contagem local de elementos
    pthread_exit((void *)contadorLocal);
}



//fluxo principal
int main(int argc, char *argv[]){
    double inicio, fim; //variaveis para calculo da duracao do programa sequencial e concorrente
    pthread_t *tid; //identificadores de thread do sistema
    int *retorno; //valor de retorno das threads
    srand((unsigned)time(NULL));
    int contadorSequencial = 0;
    int contadorConcorrente = 0;

    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc<5){
        fprintf(stderr, "Digite: %s <dimensao do vetor> <limiar inferior> <limiar superior> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoll(argv[1]);
    limInferior = atoi(argv[2]);
    limSuperior = atoi(argv[3]);
    nthreads = atoi(argv[4]);

    //aloca o vetor de entrada
    vetor = (float*) malloc(sizeof(float)*dim);
    if(vetor == NULL){
        fprintf(stderr, "ERRO --- malloc(vetor)\n");
        return 2;
    }
    //inicializacao do vetor
    for(int i=0;i<dim;i++){
        vetor[i] = (rand()%1001 + (rand()%1000)/1000.0);
    }
    puts("");
    

    //busca sequencial dos elementos
    GET_TIME(inicio);
    for(int i=0; i<dim; i++){
        if(limInferior<vetor[i] && vetor[i]<limSuperior){
            contadorSequencial++;
        }
    }
    GET_TIME(fim);
    printf("tempo sequencial: %lf\n", fim-inicio);

    GET_TIME(inicio);
    //soma concorrente dos elementos
    tid = (pthread_t *) malloc(sizeof(pthread_t) *nthreads);
    if(tid == NULL){
        fprintf(stderr, "ERRO --- malloc(tid)\n");
        return 2;
    }
    //criar as threads
    for(long int i=0;i<nthreads;i++){
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO --- pthread_create\n");
            return 3;
        }
    }

    //aguardar o término das threads
    for(long int i=0; i<nthreads; i++){
        if(pthread_join(*(tid+i), (void**) &retorno)){
            fprintf(stderr, "ERRO --- pthread_join\n");
            return 3;
        }
        contadorConcorrente += *retorno; //contador global da concorrencia
        //free(retorno);
    }
    GET_TIME(fim);
    printf("tempo concorrente: %lf\n", fim-inicio);


    //computar o valor final

    //exibir os resultados
    puts("");
    printf("Contador Sequencial: %d\n", contadorSequencial);
    printf("Contador Concorrente: %d\n", contadorConcorrente);
    puts("");
    if(contadorConcorrente == contadorSequencial){
        puts("Contadores calculados igualmente!\n");
    }else{
        puts("Erro no cálculo de algum dos contadores!\n");
    }

    //libera as memorias alocadas
    free(vetor);
    free(tid);
    

    return 0;
}


