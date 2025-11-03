#include "algo.h"
#include "graph.h"

#include <GraphBLAS.h>
#include <iostream>
#include <vector>
#include <chrono>

int main() {
    // Инициализация GraphBLAS
    GrB_init(GrB_BLOCKING);

    // ===== 1. Создаём тестовый граф =====
    // Вершины: 0, 1, 2, 3
    // Рёбра:
    // 0 -> 1, 0 -> 2
    // 1 -> 2
    // 2 -> 0
    // 3 -> 0, 3 -> 2
    // A = 0, B = 1, C
    std::vector<std::pair<GrB_Index, GrB_Index>> edges = {
        {0, 1}, {0, 2}, {0, 3},
        {1, 0}, {1, 3},
        {2, 3},
        {3, 2}, {3, 1}
    };
    GBGraph graph{};
    graph.n_nodes = 4;
    graph.n_arcs = edges.size();
    graph.is_inited = true;

    // Матрица смежности (bool)
    GrB_Matrix_new(&graph.matrix, GrB_BOOL, graph.n_nodes, graph.n_nodes);

    for (auto [src, dst] : edges) {
        GrB_Matrix_setElement_BOOL(graph.matrix, true, src, dst);
    }

    // ===== 2. Запускаем PageRank =====
    GBPageRank pagerank(0.85, 1e-6, 100);
    pagerank.RunAlgo(graph);

    const auto& result = pagerank.GetResult();

    // ===== 3. Печатаем результаты =====
    std::cout << "PageRank results:\n";
    for (size_t i = 0; i < result.size(); i++) {
        std::cout << "Vertex " << i << ": " << result[i] << "\n";
    }

    std::cout << "Execution time: " << pagerank.GetExecTime().count() << " ms\n";

    // ===== 4. Очистка =====
    GrB_Matrix_free(&graph.matrix);
    GrB_finalize();
    return 0;
}
