#ifndef GRAPHLOADER_H
#define GRAPHLOADER_H

#include "Graph.h"


class GraphLoader {
public:
    static int load(std::string const& s, Graph* g);
};



#endif
