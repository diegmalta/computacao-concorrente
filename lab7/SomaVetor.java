import java.util.Arrays;
import java.util.Random;

//criar uma classe que implementa a interface Runnable
class S{

    //gerador de numeros aleatorios
    Random rand = new Random();
    private int tamVetor; //o tamanho do vetor eh multiplo de NTHREADS garantindo balanceamento de carga

    //inicializacao de vetores
    int[] vetor;
    private int somaTotal;

    //construtor
    public S(){
        Random rand = new Random();
        this.tamVetor = rand.nextInt(200);
        this.vetor = new int[this.tamVetor];
        for (int i = 0; i < tamVetor; i++) {
            int randInt = rand.nextInt(10); //preenche o vetor com numeros aleatorios de 0 ate 9
            vetor[i] = randInt;
        }
    }

    public synchronized void inc(int i) {
        somaTotal += vetor[i]; //soma numa variavel o numero armazenado na posicao i do vetor
    }

    public synchronized int getSomaTotal() {
        return this.somaTotal;
    }

    public synchronized int[] getVetor(){
        return this.vetor;
    }

    public synchronized int getValorVetor(int i){
        return this.vetor[i];
    }

    public synchronized int getTamVetor(){
        return this.tamVetor;
    }

}

class X extends Thread{
    //id da thread
    private int id;
    private int nthreads;
    //objeto compartilhado entre threads
    S s;

    int tamBloco;
    int inicial;
    int fim;


    //construtor
    public X(int id, S s, int nthreads){
        this.id = id;
        this.s = s;
        this.nthreads = nthreads;
        this.tamBloco = this.s.getTamVetor()/this.nthreads;
        this.inicial = this.id*tamBloco;
    }



    //funcao executada pelas threads
    public void run() {
        if(id == (this.nthreads-1)){
            fim = this.s.getTamVetor();
        }else{
            fim = inicial+tamBloco; //trata o resto, se houver
        }
        System.out.println("Thread " + this.id + " iniciou.");
        int aux = this.s.getTamVetor()/this.nthreads;

        for (int i = inicial; i < fim; i++){
                this.s.inc(i);
        }
        System.out.println("Thread " + this.id + " terminou!");
    }
    
}

class SomaVetor{

    static final int NTHREADS = 3;

    public static void main(String[] args) {
        //vetor de threads
        Thread[] threads = new Thread[NTHREADS];

        System.out.println();

        //cria uma instancia do recurso compartilhado pelas threads
        S s = new S();

        System.out.println("Tamanho do Vetor: " + s.getTamVetor());

        //Cria as threads
        for (int i = 0; i < threads.length; i++) {
            threads[i] = new X(i, s, NTHREADS);
        }

        //Inicia as threads
        for (int i = 0; i < threads.length; i++) {
            threads[i].start();
        }

        //Esperar pelo termino das Threads
        for (int i = 0; i < threads.length; i++) {
            try {threads[i].join();}catch (InterruptedException e){return;}
        }

        //confere se a soma realizada nas threads esta correta
        int confereSoma = 0;
        for (int i = 0; i < s.getTamVetor(); i++) {
            confereSoma += s.vetor[i];
        }

        System.out.println("Soma na main: " + confereSoma);
        System.out.println("Números dentro do vetor: " + Arrays.toString(s.getVetor()));
        System.out.println("A execucao terminou. Resultado: " + s.getSomaTotal());

    }

}

