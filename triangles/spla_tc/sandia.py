from pyspla import *

rows = [1, 2, 2, 3, 4, 5, 5]
cols = [0, 0, 1, 1, 3, 3, 4]
vals = [1, 1, 1, 0, 1, 1, 1]

M = Matrix.from_lists(rows, cols, vals, shape=(6, 6), dtype=INT)
print(M)

def sandia(A):
    sq = A.mxm(A, INT.MULT, INT.PLUS)
    res = sq.emult(INT.MULT, A)
    sum = res.reduce(op_reduce=INT.PLUS)

    return sum

count = sandia(M)

print(count)