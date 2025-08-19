#pragma once
#include <GraphBLAS.h>

#include "graph.h"

class AlgoRunner {
   public:
    virtual int RunAlgo(Graph const& graph) = 0;
};