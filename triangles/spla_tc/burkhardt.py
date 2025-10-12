from pyspla import *

rows = [0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5]
cols = [1, 2, 0, 2, 3, 0, 1, 1, 4, 5, 3, 5, 3, 4]
vals = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]

M = Matrix.from_lists(rows, cols, vals, shape=(6, 6), dtype=INT)
print(M)

def burkhardt(A):
    sq = A.mxm(A, INT.MULT, INT.PLUS)
    res = sq.emult(INT.MULT, A)

    sum = res.reduce(op_reduce=INT.PLUS) // 6

    return sum

count = burkhardt(M)

print(count)