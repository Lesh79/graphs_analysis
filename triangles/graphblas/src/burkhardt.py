import graphblas as gb
gb.init("suitesparse", blocking=True)

from graphblas import binary

def burkhardt(A):
    squared = A.mxm(A)
    res = squared.ewise_mult(A, op=binary.gt)
    return res.nvals // 6
