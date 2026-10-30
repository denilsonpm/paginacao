import csv
from collections import defaultdict

import matplotlib.pyplot as plt


def main():
    caminho_csv = "resultados/resultados.csv"
    caminho_grafico = "resultados/grafico_faltas.png"
    marcadores = {
        "FIFO": "o",
        "LRU": "s",
        "Otimo": "^",
        "SegundaChance": "D",
        "LFU": "v",
    }

    # Cada par (algoritmo, sequência) forma uma linha independente no gráfico.
    dados = defaultdict(list)

    with open(caminho_csv, "r", encoding="utf-8", newline="") as arquivo:
        leitor = csv.DictReader(arquivo)

        for linha in leitor:
            algoritmo = linha["Algoritmo"]
            sequencia = int(linha["Sequencia"])
            quadros = int(linha["Quadros"])
            faltas = int(linha["Faltas"])
            dados[(algoritmo, sequencia)].append((quadros, faltas))

    plt.figure(figsize=(10, 6))

    for (algoritmo, sequencia), pontos in sorted(dados.items()):
        pontos.sort(key=lambda ponto: ponto[0])
        quadros, faltas = zip(*pontos)
        plt.plot(
            quadros,
            faltas,
            marker=marcadores.get(algoritmo, "X"),
            label=f"{algoritmo} - Sequência {sequencia}",
        )

    plt.title("Comparação de faltas de página por sequência")
    plt.xlabel("Quantidade de quadros")
    plt.ylabel("Quantidade de faltas de página")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.savefig(caminho_grafico, dpi=300)
    plt.show()


if __name__ == "__main__":
    main()
