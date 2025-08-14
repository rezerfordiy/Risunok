#ifndef ASTAR_H
#define ASTAR_H

#include "Graph.h"
#include <vector>
#include <functional>

class AStar {
public:
    static std::vector<int> findPath(int from, int to,
                                   std::function<int(int, int)> heuristic,
                                   const Graph& graph);
};

#endif // ASTAR_H
