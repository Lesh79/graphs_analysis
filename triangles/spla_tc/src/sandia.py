from pyspla import *

def sandia(A):
    sq = A.mxm(A, INT.MULT, INT.PLUS)
    res = sq.emult(INT.MULT, A)
    sum = res.reduce(op_reduce=INT.PLUS)

    return sum
