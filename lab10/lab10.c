#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include<unistd.h>

#define tamBuffer 7

// Variaveis globais
sem_t slotCheio, slotVazio;//condicao
sem_t bufferCheio, bufferVazio; 
sem_t mutexProd, mutexCons;//exclusao mutua
int Buffer[tamBuffer];
int nthreads, numProdutor, numConsumidor;
int out = 0,  numSlot = 0, contador=0;
int maxContador;

//inicializa o buffer
void IniciaBuffer(int n) {
    int i;
    for(i=0; i<n; i++)
    Buffer[i] = 0;
}

//imprime o buffer
void ImprimeBuffer(int n) {
    int i;
    for(i=0; i<n; i++)
        printf("%d ", Buffer[i]);
    printf("\n\n");
}

void Insere (int i, int item) {
    //exclusao mutua entre produtores
    sem_wait(&mutexProd);

    printf("INSERINDO ITEM - Thread id: %d\n", item);
    Buffer[i] = item;

    sem_post(&bufferCheio);
    sem_post(&mutexProd);

    contador++;

    ImprimeBuffer(tamBuffer);
}

void Retira (int item) {
    //exclusao mutua entre consumidores
    sem_wait(&mutexCons);
    
    printf("RETIRANDO ITEM - Thread id: %d\n", item);

    Buffer[out] = 0;
    out++;

    if(out != tamBuffer){
        sem_post(&mutexCons);
    }

    contador++;

    ImprimeBuffer(tamBuffer);
}

//thread produtora
void * produtor(void * arg) {
    int *id = (int *) arg;
    printf("Sou a thread produtora %d\n", *id);
    //sleep(2);
    while(1) {
        //produzindo o item
        sem_wait(&bufferVazio);
        printf("PREENCHENDO O BUFFER INTEIRO\n");
        for(int i=0;i<tamBuffer;i++){
            Insere(i, *id);
        }
        sem_post(&mutexCons);
        if(contador>maxContador){
            free(arg);
            pthread_exit(NULL);
        }
    } 
    free(arg);
    pthread_exit(NULL);
}

//thread consumidora
void * consumidor(void * arg) {
    int *id = (int *) arg;
    printf("Sou a thread consumidora %d\n", *id);
    //sleep(2);
    while(1) {
        sem_wait(&bufferCheio);
        //printf("ESVAZIANDO O BUFFER INTEIRO\n");
        Retira(*id);
        if(out == tamBuffer){
            out = 0;
            sem_post(&bufferVazio);
        }
        if(contador>maxContador){
            free(arg);
            pthread_exit(NULL);
        }
    }
    free(arg);
    pthread_exit(NULL);
}

//funcao principal
int main(int argc, char* argv[]){
    if(argc != 4){
        printf("Executar programa da seguinte forma: %s <numero de threads produtoras> <numero de threads consumidoras> <numero de operacoes realizadas>\n", argv[0]);
        exit(-1);
    }
    numProdutor = atoi(argv[1]);
    numConsumidor = atoi(argv[2]);
    maxContador = atoi(argv[3]);
    nthreads = numConsumidor+numProdutor;

    int *id[nthreads];

    //aloca espaco para as threads
    for (int t=0; t<nthreads; t++) {
        if ((id[t] = malloc(sizeof(int))) == NULL) {
            pthread_exit(NULL);
            printf("Erro de alocacao de memoria na thread %d.\n", t); 
            return 1;
        }
        *id[t] = t+1;
    }

    //inicializa o Buffer
    IniciaBuffer(tamBuffer);  

    pthread_t tidP[numProdutor];
    pthread_t tidC[numConsumidor];

    //inicia os semáforos
    sem_init(&mutexCons, 0, 0);
    sem_init(&mutexProd, 0, 1);
    sem_init(&bufferCheio, 0, 0);
    sem_init(&bufferVazio, 0, 1);

    printf("---------------------------------------------------------------------\n");
    printf("THREADS PRODUTORAS: %d\nTHREADS CONSUMIDORAS: %d\nTAMANHO DO BUFFER: %d\n\n", numProdutor, numConsumidor, tamBuffer);

    //cria as threads produtoras
    for(int i=0; i<numProdutor; i++) {
        if(pthread_create(&tidP[i], NULL, produtor, (void *) id[i])) exit(-1);
    } 
    
    //cria as threads consumidoras
    for(int i=0; i<numConsumidor; i++) {
        if(pthread_create(&tidC[i+numProdutor], NULL, consumidor, (void *) id[i+numProdutor])) exit(-1);
    } 

    //espera todas as threads terminarem
    int i;
    for (i = 0; i < numProdutor; i++) {
        pthread_join(tidP[i], NULL);
    }
    for(int j=i; j<nthreads;j++){
        pthread_join(tidC[i], NULL);
    }

    sem_destroy(&mutexProd);
    sem_destroy(&mutexCons);
    sem_destroy(&bufferCheio);
    sem_destroy(&bufferVazio);
    
    printf("FIM DO PROGRAMA.\n\n\n\n\n\n\n");

    return 0;
}