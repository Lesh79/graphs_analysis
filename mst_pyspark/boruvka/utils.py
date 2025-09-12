from pyspark.sql import SparkSession

def load_dimacs_graph_with_metadata(spark: SparkSession, path: str):
    raw_df = spark.read.text(path)

    p = (
        raw_df.filter(raw_df.value.startswith("p"))
        .head()[0]
        .split()
    )
    num_vertices = int(p[2])
    # num_edges = int(p[3])

    edges_df = (
        raw_df.filter(raw_df.value.startswith("a"))
        .selectExpr(
            "int(split(value, ' ')[1]) as u",
            "int(split(value, ' ')[2]) as v",
            "double(split(value, ' ')[3]) as weight"
        )
    )

    return edges_df, num_vertices

