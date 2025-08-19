#include <LAGraph.h>

#include "graph.h"

class GraphParser {
   public:
    static Graph ParseDIMACS(std::string const& filepath);
};