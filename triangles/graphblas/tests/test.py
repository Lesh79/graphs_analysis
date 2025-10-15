import graphblas as gb
gb.init("suitesparse", blocking=True)

from src.sandia import sandia
from src.burkhardt import burkhardt

rows = [0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5]
cols = [1, 2, 0, 2, 3, 0, 1, 1, 4, 5, 3, 5, 3, 4]

M = gb.Matrix.from_coo(rows, cols, nrows=6, ncols=6)
N = gb.select.tril(M)

count = burkhardt(M)
tri_count = sandia(N)

print(f"Burkhardt count: {count}") # 2
print(f"Sandia count: {tri_count}") # 2