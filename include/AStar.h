#ifndef ASTAR_H
#define ASTAR_H

#include "Graph.h"
#include <functional> //need 12


class AStar {
public:
    static std::vector<int> findPath(int from, int to,
                                   std::function<int(int, int)> heuristic,
                                   const Graph& graph);
};

#endif // ASTAR_H
