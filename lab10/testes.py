import sys
import subprocess

if (len(sys.argv) < 2) :
    print("Execute como: python3 <{}> <numero de operacoes>".format(sys.argv[0]))
    print("O numero de operacoes eh o numero de execucoes das funcoes Inserir + Retirar")
    sys.exit(0)

print("Pressione ctrl+Z para pausar a execucao do programa. Cada execução terminada ate entao sera printada em saida.txt")
print("Executando...")
produtores = [1, 3] # dimensoes do vetor (em milhoes)
consumidores = [1, 3]
operacoes = sys.argv[1]
processos = []

with open("saida.txt", "w") as f :
    for produtor in produtores :
        for consumidor in consumidores :
            processos.append(subprocess.run(
                ["./lab10", str(produtor), str(consumidor), str(operacoes)], stdout=f))

for processo in processos :
    if (processo.returncode > 0) :
        print("Falha na execucao do programa!")
        sys.exit(1)
print("O programa executou com sucesso!")
