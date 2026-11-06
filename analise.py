import csv
import os
import sys
import matplotlib.pyplot as plt


arquivo = "resultados/resultados.csv"

marcadores = {
    "FIFO": "o",
    "LRU": "s",
    "Otimo": "^",
    "SegundaChance": "D",
    "LFU": "v"
}

algoritmos = ["FIFO", "LRU", "Otimo", "SegundaChance", "LFU"]

if len(sys.argv) > 1 and sys.argv[1].lower() != "todos":
    selecionados = [nome.lower() for nome in sys.argv[1].split(",")]
    algoritmos = [
        nome for nome in algoritmos
        if nome.lower() in selecionados
    ]

dados = {}

with open(arquivo, "r", encoding="utf-8", newline="") as f:
    leitor = csv.DictReader(f)

    for linha in leitor:
        algoritmo = linha["Algoritmo"]
        sequencia = int(linha["Sequencia"])
        quadros = int(linha["Quadros"])
        faltas = int(linha["Faltas"])

        chave = sequencia

        if chave not in dados:
            dados[chave] = {}

        if algoritmo not in dados[chave]:
            dados[chave][algoritmo] = []

        dados[chave][algoritmo].append((quadros, faltas))


os.makedirs("resultados/graficos", exist_ok=True)

for sequencia, grupos in dados.items():

    plt.figure()

    for algoritmo, pontos in grupos.items():

        if algoritmo not in algoritmos:
            continue

        pontos.sort(key=lambda ponto: ponto[0])

        quadros = [ponto[0] for ponto in pontos]
        faltas = [ponto[1] for ponto in pontos]

        plt.plot(
            quadros,
            faltas,
            marker=marcadores[algoritmo],
            markersize=8,
            linewidth=2,
            label=algoritmo
        )

    plt.xlabel("Quantidade de quadros")
    plt.ylabel("Quantidade de faltas de página")
    plt.xticks(sorted(set(
        ponto[0]
        for grupo in grupos.values()
        for ponto in grupo
    )))
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.savefig(
        f"resultados/graficos/sequencia_{sequencia}.png",
        dpi=300
    )

    plt.close()