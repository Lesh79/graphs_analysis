import pytest

from boruvka.boruvka import boruvka_mst
from pyspark.sql import SparkSession


@pytest.fixture(scope="session")
def spark():
    return SparkSession.builder.appName("BoruvkaMST").getOrCreate()

def test_graph_1(spark):
    edges = [
        (0, 1, 4.0),
        (0, 2, 2.0),
        (1, 2, 1.0),
        (1, 3, 5.0),
        (2, 3, 8.0),
        (2, 4, 10.0),
        (3, 4, 2.0),
        (3, 5, 6.0),
        (4, 5, 3.0)
    ]

    edges_df = spark.createDataFrame(edges, ["u", "v", "weight"])
    num_vertices = 6

    mst_edges, total_weight = boruvka_mst(spark, edges_df, num_vertices)

    assert total_weight == 13

def test_graph_2(spark):
    edges = [
        (0, 1, 10.0),
        (0, 2, 6.0),
        (0, 3, 5.0),
        (1, 3, 15.0),
        (2, 3, 4.0),
        (1, 4, 3.0),
        (3, 4, 8.0),
        (4, 5, 7.0),
        (3, 5, 12.0)
    ]
    
    edges_df = spark.createDataFrame(edges, ["u", "v", "weight"])
    num_vertices = 6
        
    mst_edges, total_weight = boruvka_mst(spark, edges_df, num_vertices)

    assert total_weight == 27

def test_graph_3(spark):
    edges = [
        (0, 1, 1.0),
        (0, 2, 2.0),
        (1, 2, 2.0),
        (2, 3, 2.0),
        (2, 4, 2.0),
        (3, 4, 1.0),
    ]
    
    edges_df = spark.createDataFrame(edges, ["u", "v", "weight"])
    num_vertices = 5
        
    mst_edges, total_weight = boruvka_mst(spark, edges_df, num_vertices)

    assert total_weight == 6
