#include <gtest/gtest.h>
#include <fstream>
#include "parser.h"
#include "graph.h"

class DimacsParserTest : public ::testing::Test {
protected:
    std::string tempFile = "test_dimacs.gr";

    void SetUp() override {
        std::ofstream f(tempFile);
        ASSERT_TRUE(f.is_open());
        f << "c This is a comment line\n";
        f << "p edge 4 3\n";
        f << "a 1 2 5\n";
        f << "a 2 3 1\n";
        f << "a 4 1 0\n";
        f.close();
    }

    void TearDown() override {
        std::remove(tempFile.c_str());
    }
};

TEST_F(DimacsParserTest, ParsesBasicDimacsFile) {
    Parser parser;
    GBGraph graph = parser.ParseDIMACS(tempFile, true);

    EXPECT_TRUE(graph.is_inited);
    EXPECT_EQ(graph.n_nodes, 4);
    EXPECT_EQ(graph.n_arcs, 3);

    int64_t val;
    GrB_Info info;

    info = GrB_Matrix_extractElement_INT64(&val, graph.matrix, 0, 1);
    EXPECT_EQ(info, GrB_SUCCESS);
    EXPECT_EQ(val, 5);

    info = GrB_Matrix_extractElement_INT64(&val, graph.matrix, 1, 2);
    EXPECT_EQ(info, GrB_SUCCESS);
    EXPECT_EQ(val, 1);

    info = GrB_Matrix_extractElement_INT64(&val, graph.matrix, 3, 0);
    EXPECT_EQ(info, GrB_SUCCESS);
    EXPECT_EQ(val, 1);

    info = GrB_Matrix_extractElement_INT64(&val, graph.matrix, 1, 0);
    EXPECT_EQ(info, GrB_SUCCESS);
    EXPECT_EQ(val, 5);
}

TEST_F(DimacsParserTest, ThrowsIfGraphNotInited) {
    std::ofstream f(tempFile);
    f << "a 1 2 5\n";
    f.close();

    Parser parser;
    EXPECT_THROW(parser.ParseDIMACS(tempFile, true), std::runtime_error);
}

TEST_F(DimacsParserTest, ThrowsIfFileNotFound) {
    Parser parser;
    EXPECT_THROW(parser.ParseDIMACS("nonexistent_file.txt", true), std::runtime_error);
}
