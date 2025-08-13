#include "Graph.h"
#include "GraphLoader.h"
#include <iostream>
#include <fstream>
#include <cmath>
int GraphLoader::load(std::string const& s, Graph* g) {
    g->ptrs.clear();
    g->edges.clear();
    g->infos.clear();
    std::ifstream in(s);
    if (!in.is_open()) {
        //std::cout << "\nCANT OPEN\n" << std::endl;
        return -1;
    }
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

#ifdef DEBUG
    std::cout << "READING\n";
    for (int i = 0; i < ptrs.size(); i++) {
        std::cout << ptrs[i] << std::endl;
    }
    for (int i = 0; i < edges.size(); i++) {
        std::cout << edges[i].first << ' ' << edges[i].second << std::endl;
    }
    std::cout << "END\n";
#endif

    return k != g->edges.size();
    
    
}
