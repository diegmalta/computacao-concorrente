#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>


//variaveis globais
int dim;
int contador;
int *vetor;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

//funcao barreira
void barreira(int nthreads){
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    contador--;
    if (contador > 0){
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    else{
        contador = dim;
        pthread_cond_broadcast(&x_cond);
    }
    pthread_mutex_unlock(&x_mutex);
}

void *tarefa(void *arg){
    int id = *(int*) arg;
    long int somatorio = 0;

    for(int i=0; i<dim; i++){
        for(int j = 0; j<dim; j++){
            printf("Thread %d: passo=%d\n", id, i+1);
            somatorio += vetor[i];
        }
        //sincronizacao condicional
        barreira(dim);
        printf("Somatorio parcial Thread %d: somatorio=%ld\n", id, somatorio);
        vetor[id] = rand() % 10;
        barreira(dim);
    }
    printf("RESULTADO Thread %d: somatorio final = %ld\n", id, somatorio);

    pthread_exit((void *)somatorio);
}

int main(int argc, char *argv[]){
    //Inicializa a funcao srand para gerar numero inteiros aleatorios
    srand((unsigned)time(NULL));

    //declaracao de variaveis
    long int *vetorResultante;

    //recebe e valida os parametros de entrada (dimensao do vetor)
    if(argc != 2){
        fprintf(stderr, "Digite: %s <dimensao do vetor(numero de threads)>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);

    //Aloca estruturas
    vetor = (int*) malloc(sizeof(int)*dim);
    vetorResultante = (long int*) malloc(sizeof(long int)*dim);

    //inicializa contador e o vetor
    contador = dim;
    for(int i=0;i<dim;i++){
        vetor[i] = rand() % 10;
    }

    pthread_t threads[dim];
    int id[dim];
    /*  Inicializa o mutex e variavel de condicao   */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    //Cria threads
    for(int i=0; i<dim; i++){
        id[i] = i;
        pthread_create(&threads[i], NULL, tarefa, (void *) &id[i]);
        printf("CRIO SIM\n");
    }

    long int somatorio;
    //Espera o término de todas as threads
    for(int i=0; i<dim; i++){
        pthread_join(threads[i],(void **)&somatorio);
        vetorResultante[i] = somatorio;
    }
    printf("FIM\n");

    //verifica se os valores encontrados sao iguais
    for(int i=0; i<dim;i++){
        printf("%d - Thread: %ld\n", i+1, vetorResultante[i]);
        if(vetorResultante[i+1]!=vetorResultante[i] && i>0 && i<3){
            printf("Os valores encontrados sao diferentes!\n");
            return 2;
        }
    }
    printf("Os valores sao iguais!\n");

    //Libera o espaço alocado
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    return 0;

}