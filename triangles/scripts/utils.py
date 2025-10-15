import graphblas as gb
gb.init("suitesparse", blocking=True)

def file_to_adj_matrix(filename,
                       n_rows, 
                       n_cols, 
                       index_from_zero: bool, 
                       bidirectional: bool):
    rows = []
    cols = []
    
    with open(filename, 'r') as f:
        for line in f:
            if line.strip():
                edge = line.split()

                if index_from_zero:
                    from_node = int(edge[0])
                    to_node = int(edge[1])
                else:
                    from_node = int(edge[0]) - 1
                    to_node = int(edge[1]) - 1

                rows.append(from_node)
                cols.append(to_node)

    if bidirectional:
        M = gb.Matrix.from_coo(rows, cols, nrows=n_rows, ncols=n_cols)
    else: 
        all_rows = rows + cols
        all_cols = cols + rows
        M = gb.Matrix.from_coo(all_rows, all_cols, nrows=n_rows, ncols=n_cols)

    return M