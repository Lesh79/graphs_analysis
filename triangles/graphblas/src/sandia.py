from pygraphblas import *
from pygraphblas.gviz import *

def sandia(L):
    return L.mxm(L, mask=L)
