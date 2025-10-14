import graphblas as gb
gb.init("suitesparse", blocking=True)

from src.sandia import sandia
from src.burkhardt import burkhardt

tri_rows = [1, 2, 2, 3, 4, 5, 5]
tri_cols = [0, 0, 1, 1, 3, 3, 4]
tri_vals = [1, 1, 1, 0, 1, 1, 1]

rows = [0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5]
cols = [1, 2, 0, 2, 3, 0, 1, 1, 4, 5, 3, 5, 3, 4]
vals = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]

M = gb.Matrix.from_coo(rows, cols, vals, nrows=6, ncols=6)
N = gb.Matrix.from_coo(tri_rows, tri_cols, tri_vals, nrows=6, ncols=6)


count = burkhardt(M)
tri_count = sandia(N)

print(f"Burkhardt count: {count}") # 2
print(f"Sandia count: {tri_count}") # 2