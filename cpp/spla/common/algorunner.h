#pragma once
#include "graph.h"

template <typename ResultT>
class AlgoRunner {
   public:
    virtual void RunAlgo(SPLAGraph const& graph) = 0;
    ResultT const& GetResult() const = 0;
};