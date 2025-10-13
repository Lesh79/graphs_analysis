import graphblas as gb
gb.init("suitesparse", blocking=True)

from graphblas import binary

def sandia(L):
    squared = L.mxm(L)
    res = squared.ewise_mult(L, op=binary.gt)
    return res.nvals

rows = [1, 2, 2, 3, 4, 5, 5]
cols = [0, 0, 1, 1, 3, 3, 4]
vals = [1, 1, 1, 0, 1, 1, 1]

M = gb.Matrix.from_coo(rows, cols, vals, nrows=6, ncols=6)
print(sandia(M))