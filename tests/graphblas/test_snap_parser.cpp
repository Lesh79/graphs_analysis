#include <cstdio>
#include <fstream>

#include <gtest/gtest.h>

#include "graph.h"
#include "parser.h"

class SnapParserTest : public ::testing::Test {
protected:
    std::string tempFile = "test_snap.txt";

    void SetUp() override {
        std::ofstream f(tempFile);
        ASSERT_TRUE(f.is_open());
        f << "# Nodes: 5 Edges: 4\n";
        f << "1 2\n";
        f << "2 3\n";
        f << "3 5\n";
        f << "4 1\n";
        f.close();
    }

    void TearDown() override {
        std::remove(tempFile.c_str());
    }
};

TEST_F(SnapParserTest, ParsesValidSnapFile) {
    Parser parser;
    GBGraph graph = parser.ParseSNAP(tempFile);

    EXPECT_TRUE(graph.is_inited);
    EXPECT_EQ(graph.n_nodes, 5);
    EXPECT_EQ(graph.n_arcs, 4);

    int64_t val;
    GrB_Info info;

    info = GrB_Matrix_extractElement_INT64(&val, graph.matrix, 0, 1);
    EXPECT_EQ(info, GrB_SUCCESS);
    EXPECT_EQ(val, 1);

    info = GrB_Matrix_extractElement_INT64(&val, graph.matrix, 1, 2);
    EXPECT_EQ(info, GrB_SUCCESS);
    EXPECT_EQ(val, 1);

    info = GrB_Matrix_extractElement_INT64(&val, graph.matrix, 2, 4);
    EXPECT_EQ(info, GrB_SUCCESS);
    EXPECT_EQ(val, 1);

    info = GrB_Matrix_extractElement_INT64(&val, graph.matrix, 3, 0);
    EXPECT_EQ(info, GrB_SUCCESS);
    EXPECT_EQ(val, 1);
}

TEST_F(SnapParserTest, ThrowsIfFileMissing) {
    Parser parser;
    EXPECT_THROW(parser.ParseSNAP("nonexistent_file.txt"), std::runtime_error);
}

TEST_F(SnapParserTest, ThrowsIfNoHeaderFound) {
    std::ofstream f(tempFile);
    f << "1 2\n2 3\n";
    f.close();

    Parser parser;
    EXPECT_THROW(parser.ParseSNAP(tempFile), std::runtime_error);
}
