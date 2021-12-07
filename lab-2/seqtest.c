#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

float *matriz; //matriz de entrada
float *vetor; //vetor de entrada
float *saida; //vetor de saida


int main(int argc, char* argv[]){
    int dim; //variavel da dimensao das matrizes de entrada

    //leitura e avaliação dos parametros de entrada
    if(argc<2){
        printf("Digite: %s <dimensão da matriz>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    
    //alocacao de memoria para as estruturas de dados
    matriz = malloc(sizeof(float)*dim*dim);
    if(matriz == NULL){
        printf("ERRO --- malloc(matriz)\n");
        return 2;
    }
    vetor = malloc(sizeof(float)*dim);
    if(vetor == NULL){
        printf("ERRO --- malloc(vetor)\n");
        return 2;
    };
    saida = malloc(sizeof(float)*dim);
    if(saida == NULL){
        printf("ERRO --- malloc(saida)\n");
        return 2;
    };

    //inicializacao das estruturas de dados
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            matriz[i*dim+j] = 1; //equivalente a matriz[i][j]
        }
        vetor[i] = 1;
        saida[i] = 0;
    }

    //multiplicacao de matriz pelo vetor
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            saida[i] += matriz[i*dim+j]*vetor[j];
        }
    }

    //exibicao dos resultados
    puts("Matriz de entrada: ");
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            printf("%.1f ", matriz[i*dim+j]);
        }
        puts("");
    }
    puts("Vetor de entrada: ");
    for(int i=0;i<dim;i++){
            printf("%.1f ", vetor[i]);
    }
    puts("");
    puts("Vetor de saída: ");
    for(int i=0;i<dim;i++){
            printf("%.1f ", saida[i]);
    }
    puts("");

    //liberacao da memoria
    free(matriz);
    free(vetor);
    free(saida);

    return 0;
}