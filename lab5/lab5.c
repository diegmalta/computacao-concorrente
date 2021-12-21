/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */

#include <pthread.h>
#include <stdio.h>

#define NTHREADS  5

/* Variaveis globais */
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
pthread_cond_t x_cond_A;
int contador = 0;

/* Thread A */
void *A (void *t) {
    pthread_mutex_lock(&x_mutex);
    if(contador < 3){
        pthread_cond_wait(&x_cond_A, &x_mutex);
    }
    printf("Volte sempre!\n");
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {
    pthread_mutex_lock(&x_mutex);
    pthread_cond_wait(&x_cond, &x_mutex);
    printf("Fique a vontade.\n");
    contador++;
    if(contador == 3){
        pthread_cond_signal(&x_cond_A);
    }
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

/* Thread C */
void *C (void *t) {
    pthread_mutex_lock(&x_mutex);
    pthread_cond_wait(&x_cond, &x_mutex);
    printf("Sente-se por favor.\n");
    contador++;
    if(contador == 3){
        pthread_cond_signal(&x_cond_A);
    }
    pthread_mutex_unlock(&x_mutex);
    pthread_cond_signal(&x_cond_A);
    pthread_exit(NULL);
}

/* Thread D */
void *D (void *t) {
    pthread_mutex_lock(&x_mutex);
    pthread_cond_wait(&x_cond, &x_mutex);
    printf("Aceita um copo d’água?\n");
    contador++;
    if(contador == 3){
        pthread_cond_signal(&x_cond_A);
    }
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

/* Thread E */
void *E (void *t) {
    printf("Seja bem-vindo!\n");
    pthread_cond_broadcast(&x_cond);
    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    int i; 
    pthread_t threads[NTHREADS];

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);
    pthread_cond_init (&x_cond_A, NULL);

    /* Cria as threads */
    pthread_create(&threads[0], NULL, A, NULL);
    pthread_create(&threads[1], NULL, B, NULL);
    pthread_create(&threads[2], NULL, C, NULL);
    pthread_create(&threads[3], NULL, D, NULL);
    pthread_create(&threads[4], NULL, E, NULL);

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf ("\nFIM\n");

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    return 0;
}
