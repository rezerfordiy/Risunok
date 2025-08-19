#include "GraphLoader.h"
#include <iostream> // for std::cout
#include <fstream> // for read


int GraphLoader::load(std::string const& s, Graph* g) {
        std::ifstream in(s);
    if (!in.is_open()) {
        return -1;
    }
    g->ptrs.clear();
    g->edges.clear();
    g->infos.clear();
    int x;
    in >> x;
    do {
        g->ptrs.push_back(x);
        in >> x;
    } while (x != -1);

    do {
        in >> x;
        if (x != -1) g->edges.push_back({ x, -1 });
    } while (x != -1);

    int k = 0;
    while (!in.eof()) {
        in >> x;
        g->edges[k++].second = x;
    }
    in.close();

    return k != g->edges.size();
}
