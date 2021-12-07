#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

int *matriz1; //matriz de entrada 1
int *matriz2; //matriz de entrada 2
int *saidaSequencial; //matriz de saida da multiplicacao na parte Sequencial
int *saidaConc; //matriz de saida da multiplicacao na parte concorrente
int nthreads; //numero de threads

typedef struct{
    int id; //identificador do elemento da thread
    int dim; //dimensao das estruturas de entrada
}tArgs;

//funcao executada pelas threads
void * tarefa(void *arg){
    tArgs *args = (tArgs*) arg;
    //printf("Thread %d\n", args->id);
    for(int i=args->id;i<args->dim;i+=nthreads){
        for(int j=0;j<args->dim;j++){
            int aux = 0;
            for(int k=0; k<(args->dim); k++){
                aux = aux + matriz1[i*(args->dim)+k]*matriz2[k*(args->dim)+j];
            }
            saidaConc[i*(args->dim)+j] = aux;
        }
    }
    pthread_exit(NULL);
}   

    
//fluxo principal
int main(int argc, char* argv[]){
    int dim; //variavel da dimensao das matrizes de entrada
    pthread_t *tid; //identificador de thread no sistema
    tArgs *args; //identificadores locais das threads de dimensao
    double inicioSequencial, fimSequencial, tempoDecorridoSequencial, inicioConcorrente, fimConcorrente, tempoDecorridoConcorrente;

    //leitura e avaliação dos parametros de entrada
    if(argc<3){
        printf("Digite: %s <dimensão da matriz> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if(nthreads>dim){
        nthreads=dim;
    }
    
    //alocacao de memoria para as estruturas de dados
    matriz1 = malloc(sizeof(int)*dim*dim);
    if(matriz1 == NULL){
        printf("ERRO --- malloc(matriz1)\n");
        return 2;
    };
    matriz2 = malloc(sizeof(int)*dim*dim);
    if(matriz2 == NULL){
        printf("ERRO --- malloc(matriz2)\n");
        return 2;
    };
    saidaSequencial = malloc(sizeof(int)*dim*dim);
    if(saidaSequencial == NULL){
        printf("ERRO --- malloc(saida Sequencial)\n");
        return 2;
    };
    saidaConc = malloc(sizeof(int)*dim*dim);
    if(saidaConc == NULL){
        printf("ERRO --- malloc(saida concorrente)\n");
        return 2;
    };

    srand(time(NULL));
    //inicializacao das estruturas de dados
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            matriz1[i*dim+j] = (rand() % 100); //equivalente a matriz[i][j] e rand() % 100 gera um numero aleatorio inteiro de 0 a 99
            matriz2[i*dim+j] = (rand() % 100);
            saidaSequencial[i*dim+j] = 0;
            saidaConc[i*dim+j] = 0;
        }
    }

    //multiplicacao na funcao main, como forma Sequencial de multiplicacao
    GET_TIME(inicioSequencial);
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            int aux = 0;
            for(int k=0; k<dim; k++){
                aux = aux + matriz1[i*dim+k]*matriz2[k*dim+j];
            }
            saidaSequencial[i*dim+j] = aux;
        }
    }
    GET_TIME(fimSequencial);
    tempoDecorridoSequencial = fimSequencial - inicioSequencial;
    printf("tempo de multiplicacao Sequencial: %lf\n", tempoDecorridoSequencial);

    //multiplicacao de matriz pelas threads
    //alocacao das estruturas 
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid == NULL){
        puts("ERRO --- malloc(tid)");
        return 2;
    }
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args == NULL){
        puts("ERRO --- malloc(args)");
        return 2;
    }

    GET_TIME(inicioConcorrente);
    //criacao das threads
    for(int i=0;i<nthreads;i++){
        (args+i)->id = i;
        (args+i)->dim = dim;
        if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
            puts("ERRO --- pthread_create");
            return 3;
        }
    }

    //espera pelo termino das threads
    for(int i=0;i<nthreads;i++){
        pthread_join(*(tid+i), NULL);
    }
    GET_TIME(fimConcorrente);
    tempoDecorridoConcorrente = fimConcorrente - inicioConcorrente;
    printf("tempo de execucao das threads: %lf\n", tempoDecorridoConcorrente);

    //conferimento dos resultados das multiplicacoes concorrentes e Sequencial

    //saidaSequencial[10] = 7; //TESTE PARA VERIFICAR SE VALORES ERRADOS SÃO CAPTADOS PELA FUNÇÃO ABAIXO

    for(int i=0;i<dim;i++){
        if(saidaSequencial[i] != saidaConc[i]){
                printf("ERRO --- saída %d da matriz Sequencial (valor: %d) diferente de saída %d da matriz concorrente (valor: %d)\n", i, saidaSequencial[i], 
                i, saidaConc[i]);
            return 4;
        }
    }
    printf("Matrizes calculadas com sucesso!\n");

    //printf("Tempo Sequencial / Tempo Concorrente = %lf\n", tempoDecorridoSequencial/tempoDecorridoConcorrente);


    //liberacao da memoria
    free(matriz1);
    free(matriz2);
    free(saidaSequencial);
    free(saidaConc);
    free(args);
    free(tid);


    return 0;
}
