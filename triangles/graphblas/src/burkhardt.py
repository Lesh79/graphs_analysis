import graphblas as gb
gb.init("suitesparse", blocking=True)

def burkhardt(A):
    squared = A.mxm(A)
    res = squared.ewise_mult(A)
    return (res.reduce_scalar("sum") // 6).value
