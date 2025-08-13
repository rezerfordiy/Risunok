#include "Graph.h"
#include "AStar.h"
#include <vector>
#include <functional>


std::vector<int> AStar::findPath(int from, int to, std::function<double(double, double)> f, Graph const& g) {
    
    auto cmp = [](std::pair<int, int> left, std::pair<int, int> right) {return left.first > left.second; };
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(cmp)> q(cmp);
    q.push({ from, 0 });

    std::vector<std::pair<int, int>> distances(g.ptrs.size(), { -1, -1 }); // best_dist, from
    distances[from].first = 0;

    
    while (!q.empty()) {
        int v = q.top().first;
        int d = q.top().second;
        q.pop();

        if (v == to) {
            break;
        
        }

        int cnt = g.ptrs[v];
        int bound = -1;
        if (v == g.ptrs.size() - 1) {
            bound = g.edges.size();
        }
        else {
            bound = g.ptrs[v + 1];
        }
        while (cnt < bound) {
            
            int neighbor = g.edges[cnt].first;
            int new_dist = d + g.edges[cnt].second + f(to, neighbor);

            if (distances[neighbor].first == -1 || distances[neighbor].first > new_dist) {
                distances[neighbor].first = new_dist;
                distances[neighbor].second = v;
                q.push({ neighbor, new_dist });

            }
                        
            cnt++;
        }

    }

    if (distances[to].first == -1) {
        return {};
    }

    std::vector<int> path;
    int cnt = to;
    while (cnt != from) {
        path.push_back(cnt);
        cnt = distances[cnt].second;
    }
    path.push_back(from);
    path.push_back(distances[to].first);
    return path;
}

