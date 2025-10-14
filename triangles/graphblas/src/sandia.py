import graphblas as gb
gb.init("suitesparse", blocking=True)

from graphblas import binary

def sandia(L):
    squared = L.mxm(L)
    res = squared.ewise_mult(L, op=binary.gt)
    return res.nvals
