from pyspark.sql import SparkSession
from pyspark.sql.functions import col, row_number
from pyspark.sql.window import Window
from typing import List, Tuple


def boruvka_mst(
    spark: SparkSession, edges_df, num_vertices: int
) -> Tuple[List[Tuple[int, int, float]], float]:

    vertices = [(i, i) for i in range(num_vertices)]
    components_df = spark.createDataFrame(vertices, ["vertex", "comp"])

    tree_count = num_vertices
    total_weight = 0.0
    mst_edges = []  # (u, v, weight) tuples

    while tree_count > 1:
        c1 = components_df.alias("c1")
        c2 = components_df.alias("c2")
        e = edges_df.alias("e")

        edges_with_comps = (
            e.join(c1, col("e.u") == col("c1.vertex"))
            .join(c2, col("e.v") == col("c2.vertex"))
            .select(
                col("e.u").alias("u"),
                col("e.v").alias("v"),
                col("e.weight").alias("weight"),
                col("c1.comp").alias("comp_u"),
                col("c2.comp").alias("comp_v"),
            )
        )

        cross_comp_edges = edges_with_comps.filter(col("comp_u") != col("comp_v"))
        if cross_comp_edges.rdd.isEmpty():
            break

        candidates_u = cross_comp_edges.select(
            col("comp_u").alias("comp"),
            col("comp_v").alias("other_comp"),
            col("u"),
            col("v"),
            col("weight"),
        )
        candidates_v = cross_comp_edges.select(
            col("comp_v").alias("comp"),
            col("comp_u").alias("other_comp"),
            col("u"),
            col("v"),
            col("weight"),
        )
        candidates = candidates_u.union(candidates_v)

        win = Window.partitionBy("comp").orderBy("weight")
        cheapest_per_comp = (
            candidates.withColumn("rn", row_number().over(win))
            .filter(col("rn") == 1)
            .select("comp", "other_comp", "u", "v", "weight")
        )

        cheapest_rows = cheapest_per_comp.collect()

        parent = {}
        rank = {}

        def make_set(x):
            if x not in parent:
                parent[x] = x
                rank[x] = 0

        def find(x):
            while parent[x] != x:
                parent[x] = parent[parent[x]]
                x = parent[x]
            return x

        def union(a, b):
            ra = find(a)
            rb = find(b)
            if ra == rb:
                return False
            if rank[ra] < rank[rb]:
                parent[ra] = rb
            elif rank[ra] > rank[rb]:
                parent[rb] = ra
            else:
                parent[rb] = ra
                rank[ra] += 1
            return True

        for r in cheapest_rows:
            make_set(r["comp"])
            make_set(r["other_comp"])

        newly_added = []
        for r in cheapest_rows:
            comp_a = r["comp"]
            comp_b = r["other_comp"]
            if find(comp_a) != find(comp_b):
                merged = union(comp_a, comp_b)
                if merged:
                    u = int(r["u"])
                    v = int(r["v"])
                    w = float(r["weight"])
                    mst_edges.append((u, v, w))
                    total_weight += w
                    newly_added.append((comp_a, comp_b))

        if not newly_added:
            break

        distinct_comps = (
            components_df.select("comp").distinct().rdd.map(lambda r: r[0]).collect()
        )
        for c in distinct_comps:
            make_set(c)

        comp_root_map = {c: find(c) for c in distinct_comps}

        comp_map_df = spark.createDataFrame(
            list(comp_root_map.items()), ["comp", "new_comp"]
        )

        components_df = components_df.join(comp_map_df, on="comp", how="left").select(
            col("vertex"), col("new_comp").alias("comp")
        )

        distinct_count = components_df.select("comp").distinct().count()
        tree_count = distinct_count

    return mst_edges, total_weight


if __name__ == "__main__":
    spark = SparkSession.builder.appName("BoruvkaMST").getOrCreate()

    edges = [
        (0, 1, 4.0),
        (0, 2, 2.0),
        (1, 2, 1.0),
        (1, 3, 5.0),
        (2, 3, 8.0),
        (2, 4, 10.0),
        (3, 4, 2.0),
        (3, 5, 6.0),
        (4, 5, 3.0),
    ]
    edges_df = spark.createDataFrame(edges, ["u", "v", "weight"])
    num_vertices = 6

    mst_edges, total_weight = boruvka_mst(spark, edges_df, num_vertices)
    print("MST edges:", mst_edges)
    print("Total MST weight:", total_weight)
    
    spark.stop()
