#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

int *matriz; //matriz de entrada
int *vetor; //vetor de entrada
int *saidaSequencial; //vetor de saida da multiplicacao na parte Sequencial
int *saidaConc; //vetor de saida da multiplicacao na parte concorrente
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
            saidaConc[i] += matriz[i*(args->dim)+j]*vetor[j];
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
    matriz = malloc(sizeof(int)*dim*dim);
    if(matriz == NULL){
        printf("ERRO --- malloc(matriz)\n");
        return 2;
    }
    vetor = malloc(sizeof(int)*dim);
    if(vetor == NULL){
        printf("ERRO --- malloc(vetor)\n");
        return 2;
    };
    saidaSequencial = malloc(sizeof(int)*dim);
    if(saidaSequencial == NULL){
        printf("ERRO --- malloc(saida Sequencial)\n");
        return 2;
    };    
    saidaConc = malloc(sizeof(int)*dim);
    if(saidaConc == NULL){
        printf("ERRO --- malloc(saida concorrente)\n");
        return 2;
    };

    srand(time(NULL));
    //inicializacao das estruturas de dados
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            matriz[i*dim+j] = (rand() % 100); //equivalente a matriz[i][j] e rand() % 100 gera um numero aleatorio inteiro de 0 a 99
        }
        vetor[i] = (rand() % 100);
        saidaSequencial[i] = 0;
        saidaConc[i] = 0;
    }

    //multiplicacao na funcao main, como forma Sequencial de multiplicacao
    GET_TIME(inicioSequencial);
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            saidaSequencial[i] += matriz[i*dim+j]*vetor[j];
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

    
    //exibicao dos resultados
    /*
    puts("Matriz de entrada: ");
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            printf("%d ", matriz[i*dim+j]);
        }
        puts("");
    }
    puts("Vetor de entrada: ");
    for(int i=0;i<dim;i++){
            printf("%d ", vetor[i]);
    }
    puts("");
    puts("Vetor de saída: ");
    for(int i=0;i<dim;i++){
            printf("%d ", saidaSequencial[i]);
    }
    puts("");
    puts("Vetor de saída concorrente: ");
        for(int i=0;i<dim;i++){
            printf("%d ", saidaConc[i]);
    }
    puts("");
    */

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

    printf("Tempo Sequencial / Tempo Concorrente = %lf\n", tempoDecorridoSequencial/tempoDecorridoConcorrente);


    //liberacao da memoria
    free(matriz);
    free(vetor);
    free(saidaSequencial);
    free(saidaConc);
    free(args);
    free(tid);


    return 0;
}
