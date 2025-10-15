import graphblas as gb
gb.init("suitesparse", blocking=True)

def sandia(L):
    squared = L.mxm(L)
    res = squared.ewise_mult(L)
    return (res.reduce_scalar("sum")).value