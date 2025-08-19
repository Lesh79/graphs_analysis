#include "algos/boruvka.h"

#include <LAGraphX.h>

#include <iostream>

int BoruvkaRunner::RunAlgo(Graph const& graph) {
    GrB_Matrix msf;
    GrB_Vector components;
    char msg[LAGRAPH_MSG_LEN];
    int status = LAGraph_msf(&msf, &components, graph.matrix, false, msg);
    std::cout << msg << std::endl;
    return status;
}