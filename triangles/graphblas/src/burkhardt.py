import graphblas as gb
gb.init("suitesparse", blocking=True)

from graphblas import binary

def burkhardt(A):
    squared = A.mxm(A)
    res = squared.ewise_mult(A, op=binary.gt)
    triangle_count = res // 6
    
    return triangle_count
