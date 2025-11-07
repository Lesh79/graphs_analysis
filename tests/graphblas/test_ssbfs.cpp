#include <GraphBLAS.h>
#include <limits>
#include <stdexcept>

#include <gtest/gtest.h>

#include "cpp/graphblas/ssbfs/algo.h"
#include "graph.h"

class GBSSBFSTest : public ::testing::Test {
protected:
    GBGraph graph{};

    void SetUp() override {
        GrB_init(GrB_NONBLOCKING);

        graph.n_nodes = 4;
        graph.n_arcs = 4;
        graph.is_inited = true;
        GrB_Matrix_new(&graph.matrix, GrB_INT64, 4, 4);

        GrB_Matrix_setElement_INT64(graph.matrix, 1, 0, 1);
        GrB_Matrix_setElement_INT64(graph.matrix, 1, 0, 2);
        GrB_Matrix_setElement_INT64(graph.matrix, 1, 1, 3);
        GrB_Matrix_setElement_INT64(graph.matrix, 1, 2, 3);
    }

    void TearDown() override {
        if (graph.matrix != nullptr) {
            GrB_Matrix_free(&graph.matrix);
        }
        GrB_finalize();
    }
};

TEST_F(GBSSBFSTest, RunsSuccessfully) {
    GBSSBFS bfs(0);
    EXPECT_NO_THROW(bfs.RunAlgo(graph));

    auto result = bfs.GetResult();

    ASSERT_EQ(result.size(), 4);

    EXPECT_EQ(result[0], 0);
    EXPECT_EQ(result[1], 0);
    EXPECT_EQ(result[2], 0);
    EXPECT_TRUE(result[3] == 1 || result[3] == 2);

    EXPECT_GT(bfs.GetExecTime().count(), 0);
}
