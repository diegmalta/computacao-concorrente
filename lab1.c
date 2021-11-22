/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: Atividade 5 */

/*
Na função main: 
inicializaremos o vetor --- CHECK
criaremos as duas threads --- CHECK
aguarderemos o término delas --- CHECK
verificaremos se os valores finais de cada vetor estão corretos --- CHECK
*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  10         //número total de threads para serem criadas
#define TAMANHO 10000       //número de posições do vetor a ser criado

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
    int idThread, inicio, fim;
} t_Args;

//vetor global de tamanho 10.000
int vetor[TAMANHO];

/*
Para elevar ao quadrado cada elemento de um vetor de 10.000 elementos, e escrever o resultado na mesma posição do elemento,
podemos testar se funciona, colocando os números 0 a 9.999 para serem calculados ao quadrado em cada vector de forma crescente.
Dividiremos a tarefa entre as threads colocando os vetores das posições 0 a 4.999 na primeira thread e das posições 5.000 a 9.999 para a segunda,
assim sendo 5.000 elementos para serem calculados para cada thread.
*/

//funcao executada pelas threads
void *calculaVetores(void *arg) {
    t_Args *args = (t_Args *) arg;

    printf("Thread #%d:    %d ate %d\n", args->idThread, args->inicio, args->fim);
    for(int i=args->inicio;i<args->fim;i++) {
        vetor[i]=i*i;
    }

    free(arg); //libera a alocação realizada na main
    pthread_exit(NULL);
}


/*
Na função main, inicializaremos o vetor, criaremos as duas threads, aguarderemos o término delas e verificaremos se os valores finais de
cada vetor estão corretos.
*/

//função principal
int main(void){
    pthread_t tid_sistema[NTHREADS]; //identifica as threads no sistema
    int thread; //variavel auxiliar
    t_Args *arg; //receberá os argumentos para a thread

    for(int i=0;i<TAMANHO;i++) {
        vetor[i]=0;
    }

    //Aloca os argumentos para as threads
    for(thread=0; thread<NTHREADS; thread++) {
        printf("Aloca argumentos para thread %d\n", thread);
        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("ERRO --- malloc()\n");
            exit(-1);
        }
        
    //separa o tamanho do vetor pelo número de threads totais

        arg->idThread = thread; 
        arg->inicio=0+(TAMANHO*thread)/NTHREADS;
        arg->fim=(TAMANHO*(thread+1))/NTHREADS;
        printf("thread:%d, INICIO:%d, FIM:%d", thread, arg->inicio, arg->fim);

        printf("--Cria a thread %d\n", thread);
        if (pthread_create(&tid_sistema[thread], NULL, calculaVetores, (void*) arg)) {
            printf("ERRO --- pthread_create()\n");
            exit(-1);
        }
    }

    //--espera todas as threads terminarem
    for (thread=0; thread<NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }

      //verificar se os valores finais estao corretos
    for(int i=0;i<TAMANHO;i++) {
        if(vetor[i]!=(i*i)) {
            printf("ERRO --- Vetor %d calculado incorretamente!\n", i);
            exit(1);
        }
    }
    printf("Vetores calculados com sucesso!\n");

    printf("--Thread principal terminou\n");

    pthread_exit(NULL);
}

