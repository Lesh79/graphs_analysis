#include <GraphBLAS.h>
#include <limits>
#include <stdexcept>

#include <gtest/gtest.h>

#include "cpp/graphblas/msbfs/algo.h"
#include "graph.h"

class GBMSBFSTest : public ::testing::Test {
protected:
    GBGraph graph{};

    void SetUp() override {
        GrB_init(GrB_NONBLOCKING);

        graph.n_nodes = 4;
        graph.n_arcs = 4;
        graph.is_inited = true;
        GrB_Matrix_new(&graph.matrix, GrB_INT64, 4, 4);

        GrB_Matrix_setElement_INT64(graph.matrix, 1, 0, 1);
        GrB_Matrix_setElement_INT64(graph.matrix, 1, 1, 2);
        GrB_Matrix_setElement_INT64(graph.matrix, 1, 2, 3);
        GrB_Matrix_setElement_INT64(graph.matrix, 1, 0, 2);
    }

    void TearDown() override {
        if (graph.matrix != nullptr) {
            GrB_Matrix_free(&graph.matrix);
        }
        GrB_finalize();
    }
};

TEST_F(GBMSBFSTest, RunsSuccessfullyForTwoSources) {
    std::vector<int> sources = {0, 1};
    GBMSBFS msbfs(sources);

    EXPECT_NO_THROW(msbfs.RunAlgo(graph));

    auto result = msbfs.GetResult();

    ASSERT_EQ(result.size(), sources.size());
    ASSERT_EQ(result[0].size(), 4);

    EXPECT_EQ(result[0][0], 0);
    EXPECT_EQ(result[0][1], 0);
    EXPECT_TRUE(result[0][2] == 1 || result[0][2] == 0);
    EXPECT_TRUE(result[0][3] == 2);

    EXPECT_EQ(result[1][1], 1);
    EXPECT_EQ(result[1][2], 1);
    EXPECT_EQ(result[1][3], 2);

    EXPECT_GT(msbfs.GetExecTime().count(), 0);
}
