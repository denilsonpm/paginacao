import csv
from pathlib import Path

import matplotlib.pyplot as plt


BASE = Path(__file__).resolve().parent
CSV = BASE / "resultados" / "resultados.csv"
SAIDA = BASE / "resultados" / "grafico_faltas.png"

ALGORITMOS = {
    "FIFO": ("FIFO", "o", "-"),
    "LFU": ("LFU", "s", "-"),
    "LRU": ("LRU", "^", "-"),
    "Otimo": ("Otimo", "D", "--"),
    "SegundaChance": ("Segunda Chance", "v", "-"),
}


def main():
    dados = {nome: [] for nome in ALGORITMOS}

    try:
        with CSV.open("r", encoding="utf-8-sig", newline="") as arquivo:
            leitor = csv.DictReader(arquivo)

            for linha in leitor:
                algoritmo = linha["Algoritmo"].strip()
                if algoritmo in dados:
                    quadros = int(linha["Quadros"])
                    faltas = int(linha["Faltas"])
                    dados[algoritmo].append((quadros, faltas))
    except (OSError, ValueError, KeyError) as erro:
        print(f"Erro ao ler o CSV: {erro}")
        return

    plt.figure(figsize=(10, 6))
    todos_quadros = set()

    for algoritmo, (legenda, marcador, estilo) in ALGORITMOS.items():
        pontos = sorted(dados[algoritmo])
        if not pontos:
            print(f"Aviso: não há dados para {legenda}.")
            continue

        quadros, faltas = zip(*pontos)
        todos_quadros.update(quadros)
        plt.plot(quadros, faltas, marker=marcador, linestyle=estilo, markersize=8, linewidth=2, label=legenda)

    if not todos_quadros:
        print("Nenhum dado de algoritmo encontrado no CSV.")
        plt.close()
        return

    plt.title("Faltas de página por número de quadros")
    plt.xlabel("Número de quadros (frames)")
    plt.ylabel("Número de faltas de página")
    plt.xticks(sorted(todos_quadros))
    plt.grid(True, linestyle="--", alpha=0.4)
    plt.legend()
    plt.tight_layout()

    SAIDA.parent.mkdir(parents=True, exist_ok=True)
    plt.savefig(SAIDA, dpi=300)
    print(f"Gráfico salvo em: {SAIDA}")
    plt.show()


if __name__ == "__main__":
    main()
