from pygraphblas import *
from pygraphblas.gviz import *

def burkhardt(matrix):
    A = matrix
    
    A_squared = A.mxm(A)
    triangles_matrix = A_squared * A
    triangle_count = triangles_matrix.nvals // 6
    
    return triangle_count, triangles_matrix