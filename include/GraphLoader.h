#ifndef GRAPHLOADER_H
#define GRAPHLOADER_H

#include "Graph.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <queue>


class GraphLoader {
public:
    static int load(std::string const& s, Graph* g);
};



#endif
