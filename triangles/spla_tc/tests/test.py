from pyspla import *

from src.sandia import sandia
from src.burkhardt import burkhardt

tri_rows = [1, 2, 2, 3, 4, 5, 5]
tri_cols = [0, 0, 1, 1, 3, 3, 4]
tri_vals = [1, 1, 1, 0, 1, 1, 1]

rows = [0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5]
cols = [1, 2, 0, 2, 3, 0, 1, 1, 4, 5, 3, 5, 3, 4]
vals = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]

M = Matrix.from_lists(rows, cols, vals, shape=(6, 6), dtype=INT)
N = Matrix.from_lists(tri_rows, tri_cols, tri_vals, shape=(6, 6), dtype=INT)

count = burkhardt(M)
tri_count = sandia(N)

print(count) # 2
print(tri_count) # 2