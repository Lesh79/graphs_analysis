#include <stdexcept>

#include <gtest/gtest.h>

#include "GraphBLAS.h"
#include "LAGraph.h"
#include "cpp/graphblas/pagerank/algo.h"
#include "graph.h"
#include "parser.h"

class PageRankTest : public ::testing::Test {
protected:
    GBGraph graph{};

    void SetUp() override {
        GrB_init(GrB_NONBLOCKING);
        LAGraph_Init(NULL);

        graph.n_nodes = 3;
        graph.n_arcs = 3;
        graph.is_inited = true;
        GrB_Matrix_new(&graph.matrix, GrB_INT64, 3, 3);
        GrB_Matrix_setElement_INT64(graph.matrix, 1, 0, 1);
        GrB_Matrix_setElement_INT64(graph.matrix, 1, 1, 2);
        GrB_Matrix_setElement_INT64(graph.matrix, 1, 2, 0);
    }

    void TearDown() override {
        if (graph.matrix != nullptr) {
            GrB_Matrix_free(&graph.matrix);
        }
        LAGraph_Finalize(NULL);
        GrB_finalize();
    }
};

TEST_F(PageRankTest, ComputesPageRankSuccessfully) {
    GBPageRank pagerank(0.85, 1e-6, 100);
    EXPECT_NO_THROW(pagerank.RunAlgo(graph));

    auto result = pagerank.GetResult();
    EXPECT_EQ(result.size(), 3);

    EXPECT_NEAR(result[0], result[1], 1e-6);
    EXPECT_NEAR(result[1], result[2], 1e-6);
    EXPECT_NEAR(result[0], 1.0 / 3.0, 1e-2);

    EXPECT_GT(pagerank.GetExecTime().count(), 0);
}
