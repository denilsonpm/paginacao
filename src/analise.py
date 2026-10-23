
import csv
import matplotlib.pyplot as plt


def main():
    caminho_csv = "resultados/resultados.csv"
    caminho_grafico = "resultados/grafico_faltas.png"

    algoritmos = ["FIFO", "LRU", "Otimo", "SegundaChance", "LFU"]

    dados = {alg: [] for alg in algoritmos}

    with open(caminho_csv, "r", encoding="utf-8") as arquivo:
        leitor = csv.DictReader(arquivo)

        for linha in leitor:
            algoritmo = linha["Algoritmo"]
            quadros = int(linha["Quadros"])
            faltas = int(linha["Faltas"])

            if algoritmo in dados:
                dados[algoritmo].append((quadros, faltas))

    plt.figure(figsize=(10, 6))

    encontrou_dados = False

    for algoritmo, pontos in dados.items():

        if not pontos:
            continue

        encontrou_dados = True

        pontos.sort()

        quadros, faltas = zip(*pontos)

        plt.plot(quadros, faltas, marker="o", label=algoritmo)

    plt.title("Comparacao de faltas de pagina")
    plt.xlabel("Quantidade de quadros")
    plt.ylabel("Quantidade de faltas de pagina")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.savefig(caminho_grafico, dpi=300)
    plt.show()


if __name__ == "__main__":
    main()
