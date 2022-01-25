// Monitor que implementa a logica do padrao leitores/escritores
class M {
    private int leitor, escritor;
    public static int var;

    // Construtor
    M() {
        this.leitor = 0; //leitores lendo (0 ou mais)
        this.escritor = 0; //escritor escrevendo (0 ou 1)
        var = 0;
    }

    // Entrada para leitores
    public synchronized void EntraLeitor (int id) {
        try {
            while (this.escritor > 0) {
                //if (this.escr > 0) {
                System.out.println ("le.leitorBloqueado("+id+")");
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.leitor++;  //registra que ha mais um leitor lendo
            System.out.println ("le.leitorLendo("+id+")");
        } catch (InterruptedException e) { }
    }

    // Saida para leitores
    public synchronized void SaiLeitor (int id) {
        this.leitor--; //registra que um leitor saiu
        if (this.leitor == 0)
            this.notify(); //libera escritor (caso exista escritor bloqueado)
        System.out.println ("le.leitorSaindo("+id+")");
    }

    // Entrada para escritores
    public synchronized void EntraEscritor (int id) {
        try {
            while ((this.leitor > 0) || (this.escritor > 0)) {
                //if ((this.leit > 0) || (this.escr > 0)) {
                System.out.println ("le.escritorBloqueado("+id+")");
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.escritor++; //registra que ha um escritor escrevendo
            System.out.println ("le.escritorEscrevendo("+id+")");
        } catch (InterruptedException e) { }
    }

    // Saida para escritores
    public synchronized void SaiEscritor (int id) {
        this.escritor--; //registra que o escritor saiu
        notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
        System.out.println ("le.escritorSaindo("+id+")");
    }
}



//Aplicacao de exemplo--------------------------------------------------------
// Leitor
class Leitor extends Thread {
    int id; //identificador da thread
    int delay; //atraso
    M monitor;//objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    Leitor (int id, int delayTime, M m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.monitor.EntraLeitor(this.id);
                //System.out.print("LEITOR INICIADO:");
                if(M.var % 2 == 0){
                    System.out.println("O valor da variável é par! ("+M.var+")");
                }else{
                    System.out.println("O valor da variável é ímpar! ("+M.var+")");
                }
                //System.out.println("LEITOR FINALIZADO.");
                this.monitor.SaiLeitor(this.id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) { return; }
    }
}

//--------------------------------------------------------
// Escritor
class Escritor extends Thread {
    int id; //identificador da thread
    int delay; //atraso
    M monitor; //objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    Escritor (int id, int delayTime, M m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.monitor.EntraEscritor(this.id);
                //System.out.println("método escritor iniciado");
                M.var = id;
                //System.out.println("método escritor finalizado");
                this.monitor.SaiEscritor(this.id);
                sleep(this.delay); //atraso bobo...
            }
        } catch (InterruptedException e) { return; }
    }
}

//--------------------------------------------------------
//LeitorEscritor
class LeitorEscritor extends Thread{

    int id; //identificador da thread
    int delay; //atraso
    M monitor; //objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    LeitorEscritor (int id, int delayTime, M m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {
        double j=777777777.7, i;
        try {
            for (;;) {
                this.monitor.EntraLeitor(this.id);
                //System.out.println("método LEITORESCRITOR 11111 iniciado");
                System.out.println("Variável:" + M.var);
                //System.out.println("método LEITORESCRITOR 111111 finalizados");
                this.monitor.SaiLeitor(this.id);
                for (i=0; i<100000000; i++) {j=j/2;} //processamento bobo qualquer
                this.monitor.EntraEscritor(this.id);
                //System.out.println("método LEITORESCRITOR ESCREVE INICIADO");
                M.var++;
                //System.out.println("método LEITORESCRITOR ESCREVE FINALIZADO");
                this.monitor.SaiEscritor(this.id);
                sleep(this.delay); //atraso bobo...
            }
        } catch (InterruptedException e) { return; }
    }

}


//--------------------------------------------------------
// Classe principal
class lab8 {
    static final int L = 3;
    static final int E = 3;
    static final int LE = 3;

    public static void main (String[] args) {
        int i;
        M monitor = new M();            // Monitor (objeto compartilhado entre leitores e escritores)
        Leitor[] l = new Leitor[L];       // Threads leitores
        Escritor[] e = new Escritor[E];   // Threads escritores
        LeitorEscritor[] le = new LeitorEscritor[LE]; //Threads leitores-escritores

        //inicia o log de saida
        System.out.println ("import verificaM");
        System.out.println ("le = verificaM.M()");

        for (i=0; i<L; i++) {
            l[i] = new Leitor(i+1, (i+1)*500, monitor);
            l[i].start();
        }
        for (i=0; i<E; i++) {
            e[i] = new Escritor(i+1, (i+1)*500, monitor);
            e[i].start();
        }
        for (i=0; i<LE; i++) {
            le[i] = new LeitorEscritor(i+1, (i+1)*500, monitor);
            le[i].start();
        }

    }
}