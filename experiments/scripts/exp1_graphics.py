import os
import numpy as np
import matplotlib.pyplot as plt

base_folder = "./../results"
output_folder = "./../graphics"

os.makedirs(output_folder, exist_ok=True)

prefixes = ["spla-", "gb-"]
default_order = ["NY", "FLA", "CAL", "W", "USA"]
pg_order = ["web-stanford", "web-google", "web-berkstan", "wiki-topcats", "sx-stackoverflow"]

if __name__ == "__main__":
    for algo in os.listdir(base_folder):
        algo_path = os.path.join(base_folder, algo)
        if not os.path.isdir(algo_path):
            continue

        algo_lower = algo.lower()
        data = {"spla": {}, "gb": {}}
        stds = {"spla": {}, "gb": {}}

        for filename in os.listdir(algo_path):
            if not (filename.startswith("spla-") or filename.startswith("gb-")):
                continue

            if "msbfs" in algo_lower and "n=16" not in filename.lower():
                continue

            prefix = "spla" if filename.startswith("spla-") else "gb"
            path = os.path.join(algo_path, filename)

            try:
                tail = filename.split(f"{algo}_", 1)[1]
            except IndexError:
                tail = filename
            tail = tail.replace(".txt", "")
            if "msbfs" in algo_lower:
                graph_name = tail.split("_N=", 1)[0]
            else:
                graph_name = tail

            with open(path, "r") as f:
                times_ms = [float(line.strip()) for line in f if line.strip()]
                times_s = np.array(times_ms) / 1000.0
                data[prefix][graph_name] = np.mean(times_s)
                stds[prefix][graph_name] = np.std(times_s)

        if not data["spla"] and not data["gb"]:
            continue

        if algo_lower == "pagerank":
            graphs = [g for g in pg_order if g in (set(data["spla"]) | set(data["gb"]))]
        else:
            graphs = [g for g in default_order if g in (set(data["spla"]) | set(data["gb"]))]

        if not graphs:
            continue

        spla_means = [data["spla"].get(g, np.nan) for g in graphs]
        gb_means   = [data["gb"].get(g, np.nan)   for g in graphs]
        spla_std   = [stds["spla"].get(g, 0)      for g in graphs]
        gb_std     = [stds["gb"].get(g, 0)        for g in graphs]

        x = np.arange(len(graphs))
        width = 0.35

        plt.figure(figsize=(10, 6))
        plt.bar(x - width/2, spla_means, width, yerr=spla_std, capsize=4,
                label="SPLA", color="#9ecae1")
        plt.bar(x + width/2, gb_means, width, yerr=gb_std, capsize=4,
                label="GB", color="#fcbba1")

        plt.yscale("log")
        plt.ylabel("Время, сек")
        title_suffix = "(N=16)" if "msbfs" in algo_lower else ""
        plt.title(f"Сравнение времени выполнения {title_suffix}")
        plt.xticks(x, graphs, rotation=45, ha="right")
        plt.legend()
        plt.grid(False)
        plt.tight_layout()

        output_path = os.path.join(output_folder, f"{algo}.png")
        plt.savefig(output_path, dpi=300)
        plt.close()
