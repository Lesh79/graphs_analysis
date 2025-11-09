import os
import numpy as np
import matplotlib.pyplot as plt

base_folder = "./../results/msbfs"
output_folder = "./../graphics"

os.makedirs(output_folder, exist_ok=True)

default_order = ["NY", "FLA", "CAL", "W", "USA"]
N_values = [1, 4, 16, 64]

colors = {"gb": "#fcbba1", "spla": "#9ecae1"}

if __name__ == "__main__":
    data = {"spla": {}, "gb": {}}
    stds = {"spla": {}, "gb": {}}

    for filename in os.listdir(base_folder):
        if not (filename.startswith("spla-") or filename.startswith("gb-")):
            continue

        prefix = "spla" if filename.startswith("spla-") else "gb"
        path = os.path.join(base_folder, filename)

        tail = filename.split("msbfs_", 1)[1].replace(".txt", "")
        if "_N=" not in tail:
            continue

        graph_name, n_part = tail.split("_N=", 1)
        try:
            N = int(n_part)
        except ValueError:
            continue

        with open(path, "r") as f:
            times_ms = [float(line.strip()) for line in f if line.strip()]
            times_s = np.array(times_ms) / 1000.0
            mean_val = np.mean(times_s) / N
            std_val = np.std(times_s) / N

        if graph_name not in data[prefix]:
            data[prefix][graph_name] = {}
            stds[prefix][graph_name] = {}

        data[prefix][graph_name][N] = mean_val
        stds[prefix][graph_name][N] = std_val

    graphs = [g for g in default_order if g in (set(data["spla"]) | set(data["gb"]))]
    if not graphs:
        exit()

    for graph in graphs:
        plt.figure(figsize=(10, 6))
        x = np.arange(len(N_values))
        width = 0.35

        for j, prefix in enumerate(["gb", "spla"]):
            means = [data[prefix].get(graph, {}).get(N, np.nan) for N in N_values]
            errs = [stds[prefix].get(graph, {}).get(N, 0) for N in N_values]
            offset = (j - 0.5) * width

            plt.bar(
                x + offset, means, width,
                yerr=errs, capsize=4,
                label=prefix.upper(),
                color=colors[prefix],
            )

        plt.yscale("log")
        plt.ylabel("Время на стартовую вершину, сек")
        plt.xlabel("Количество стартовых вершин")
        plt.title(f"Оценка масштабируемости MSBFS — {graph}")
        plt.xticks(x, N_values)
        plt.legend()
        plt.grid(False)
        plt.tight_layout()

        output_path = os.path.join(output_folder, f"msbfs_scaling_{graph}.png")
        plt.savefig(output_path, dpi=300, bbox_inches="tight")
        plt.close()

        print(f"✅ {graph} готов: {output_path}")
