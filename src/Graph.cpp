#include "Graph.h"
#include <fstream> // need for read
#include <cmath> // need for math
#include <queue> // need for 268
#include <iostream> // for std::cout

Graph::info::Point& Graph::info::Point::operator+=(Point const& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Graph::info::Point& Graph::info::Point::operator-=(Point const& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Graph::info::Point& Graph::info::Point::operator/=(double other) {
    x /= other;
    y /= other;
    return *this;
}

Graph::info::Point& Graph::info::Point::operator*=(double other) {
    x *= other;
    y *= other;
    return *this;
}

Graph::info::Point Graph::info::Point::operator/(double other) const {
    Point a(*this);
    return a /= other;
}

Graph::info::Point Graph::info::Point::operator*(double other) const {
    Point a(*this);
    return a *= other;
}

Graph::info::Point Graph::info::Point::operator+(Point const& other) const {
    Point a(*this);
    return a += other;
}

Graph::info::Point Graph::info::Point::operator-(Point const& other) const {
    Point a(*this);
    return a -= other;
}

double Graph::info::Point::abs() const {
    return sqrt(x * x + y * y) + 1e-9; // not to devide zero
}

bool Graph::info::Point::little() const {
    return x < 1e-9 && y < 1e-9;
}

void Graph::info::Point::fit(double max_size) {
    double len = abs();
    (*this).operator/=(len);
    len = std::min(abs(), max_size);
    this->operator*=(len);
}

void Graph::info::Point::bounds(Point const& from, Point const& to) {
    x = std::min(to.x, std::max(x, from.x));
    y = std::min(to.y, std::max(y, from.y));
}

int Graph::read_from_file(std::string const& s) {
    ptrs.clear();
    edges.clear();
    infos.clear();
    std::ifstream in(s);
    if (!in.is_open()) {
        std::cout << "\nCANT OPEN\n" << std::endl;
        return -1;
    }
    int x;
    in >> x;
    do {
        ptrs.push_back(x);
        in >> x;
    } while (x != -1);

    do {
        in >> x;
        if (x != -1) edges.push_back({ x, -1 });
    } while (x != -1);

    int k = 0;
    while (!in.eof()) {
        in >> x;
        edges[k++].second = x;
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

    return k != edges.size();
}

void Graph::init_info() {
    infos.resize(ptrs.size());
    for (int i = 0; i < infos.size(); i++) {
        infos[i].p = { (double)(rand() % (WIDTH + 1)), (double)(rand() % (HEIGHT + 1)) };
    }
}

void Graph::process(int iters) {
    double k = sqrt((double)WIDTH * HEIGHT / infos.size());
    double scale = std::max(WIDTH, HEIGHT) / (double)2;
    double T = scale / 10;

    int numNodes = ptrs.size();

    for (int i = 0; i < iters; i++) {
        std::vector<info::Point> deltas(numNodes, { 0, 0 });

        // Matrix not to count twice
        std::vector<std::vector<bool>> visited(numNodes, std::vector<bool>(numNodes, false));
        for (int v = 0; v < numNodes; v++) {
            int bound = ((v == (numNodes - 1)) ? edges.size() : ptrs[v + 1]);
#ifdef DEBUG
            //cout << "BOUND: " << bound << endl;
#endif

            for (int edge_cnt = ptrs[v]; edge_cnt < bound; edge_cnt++) {
                int dist = edges[edge_cnt].second;
                int neighbor = edges[edge_cnt].first;
                if (visited[v][neighbor]) {
                    continue;
                }
                visited[v][neighbor] = visited[neighbor][v] = 1;

                info::Point normal = infos[neighbor].p - infos[v].p;
                double r_abs = normal.abs();
                normal /= r_abs;
                double force_abs = r_abs * r_abs / k;
                info::Point force = normal * force_abs;

                deltas[v] += force;
                deltas[neighbor] -= force;
            }
        }

        // отталкивание
        for (int v1 = 0; v1 < numNodes - 1; v1++) {
            for (int v2 = v1 + 1; v2 < numNodes; v2++) {
                info::Point normal = infos[v2].p - infos[v1].p;
                double r_abs = normal.abs();
                normal /= r_abs;
                double force_abs = k * k / (r_abs * r_abs);
                info::Point force = normal * force_abs;

                deltas[v2] += force;
                deltas[v1] -= force;
            }
        }

        for (int v = 0; v < numNodes; v++) {
            deltas[v].fit(T);
            infos[v].p += deltas[v];
            info::Point start = { 0, 0 }, end = { (double) WIDTH, (double) HEIGHT };
            infos[v].p.bounds(start, end);
        }

        T -= T / 10;
    }

}

void Graph::process2(int iters)
{
    if (lenOfWay == -1) {
        return;
    }
    double k = sqrt((double)WIDTH * HEIGHT / infos.size());
    double scale = std::max(WIDTH, HEIGHT) / (double)2;
    double T = scale / 10;
    double diag = sqrt((double)WIDTH * WIDTH + HEIGHT * HEIGHT) + 1e-9;
    double koef = diag / lenOfWay;








    int numNodes = ptrs.size();

    for (int i = 0; i < iters; i++) {
        std::vector<info::Point> deltas(numNodes, { 0, 0 });

        // Matrix not to count twice
        std::vector<std::vector<int>> visited(numNodes, std::vector<int>(numNodes, -1));
        for (int v = 0; v < numNodes; v++) {
            int bound = ((v == (numNodes - 1)) ? edges.size() : ptrs[v + 1]);
#ifdef DEBUG
            //cout << "BOUND: " << bound << endl;
#endif

            for (int edge_cnt = ptrs[v]; edge_cnt < bound; edge_cnt++) {
                int dist = edges[edge_cnt].second;
                int neighbor = edges[edge_cnt].first;
                visited[v][neighbor] = dist;
                if (visited[neighbor][v] == -1) {
                    continue;
                }
                
                info::Point normal = infos[neighbor].p - infos[v].p;
                double r_abs = normal.abs();
                normal /= r_abs;
                info::Point force = normal * (r_abs - koef * dist);

                deltas[v] += force;
                deltas[neighbor] -= force;
            }
        }

        // отталкивание
        for (int v1 = 0; v1 < numNodes - 1; v1++) {
            for (int v2 = v1 + 1; v2 < numNodes; v2++) {
                info::Point normal = infos[v2].p - infos[v1].p;
                double r_abs = normal.abs();
                normal /= r_abs;
                double force_abs = k * k / (r_abs * r_abs);
                info::Point force = normal * force_abs;

                deltas[v2] += force;
                deltas[v1] -= force;
            }
        }

        for (int v = 0; v < numNodes; v++) {
            deltas[v].fit(T);
            infos[v].p += deltas[v];
            info::Point start = { 0, 0 }, end = { (double)WIDTH, (double)HEIGHT };
            infos[v].p.bounds(start, end);
        }

        T -= T / 10;
    }

}

bool Graph::is_connected() {
    if (this->is_checked_connected) {
        return is_conn;
    }
    int numNodes = ptrs.size();
    std::vector<int> conn(numNodes, 0);
    int k = 0;
    for (int v = 0; v < numNodes; v++) {
        if (conn[v] == 0) {
            k++;
            std::queue<int> q;
            q.push(v);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                conn[u] = k;
                int bound = ((u == numNodes - 1) ? (edges.size()) : (ptrs[u + 1]));
                for (int edge_cnt = ptrs[u]; edge_cnt < bound; edge_cnt++) {
                    int neighbor = edges[edge_cnt].first;
                    if (conn[neighbor] == 0) {
                        q.push(neighbor);
                    }
                }
            }
        }
    }
    cities = k;
    this->is_checked_connected = true;

    for (int i = 0; i < numNodes; i++) {
        infos[i].group = conn[i];
    }


    if (k == 1) {
        is_conn = true;
    }

    return this->is_conn;
}

std::vector<Graph> Graph::split() {
    if (!is_checked_connected) {
        is_connected();
    }
#ifndef DEBUG
    if (cities == 1) {
        return std::vector<Graph>{*this};
    }
#endif // !DEBUG

    double tmp = log2(cities);
    int spaces = (int)tmp;
    if ((int)tmp != tmp) { spaces += 1; }

    // spaces = pow(2, spaces);

    int slice_w = spaces / 2 + spaces % 2;
    int slice_h = spaces / 2;



    int numNodes = ptrs.size();

    struct helper {
        int cnt;
        std::vector<int> map;
    };

    std::vector<helper> helpers(cities, { 0, std::vector<int>(numNodes, -1) });


    std::vector<Graph> ans(pow(2, spaces));
    
    
    for (int v = 0; v < numNodes; v++) {
        int city = infos[v].group - 1;
        int& cnt = helpers[city].cnt;

#ifdef DEBUG
        std::cout << "v: " << v << std::endl;
        std::cout << "group: " << city << std::endl;
        std::cout << "cnt: " << cnt << std::endl;

#endif // DEBUG



        ans[city].ptrs.push_back(cnt);
        ans[city].infos.push_back(infos[v]);
        helpers[city].map[v] = ans[city].ptrs.size() - 1;

        int bound = ((v == numNodes - 1) ? (edges.size()) : (ptrs[v + 1]));
        for (int edge_cnt = ptrs[v]; edge_cnt < bound; edge_cnt++) {
            int neighbor = edges[edge_cnt].first;
            int dist = edges[edge_cnt].second;
            ans[city].edges.push_back({ neighbor, dist });
            cnt++;
        }

    }

    for (int i = 0; i < cities; i++) {
        for (int j = 0; j < ans[i].edges.size(); j++) {
            int old = ans[i].edges[j].first;
            ans[i].edges[j].first = helpers[i].map[old];
        }
    }

    return ans;
}

void Graph::show() const {
    using namespace std;
    cout << "GRAPH: ( " << ptrs.size() << ", " << edges.size() / 2 << " )" << endl;
    for (int i = 0; i < ptrs.size(); i++) {
        cout << ptrs[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < edges.size(); i++) {
        cout << edges[i].first << ' ';
    }
    cout << endl;
    for (int i = 0; i < edges.size(); i++) {
        cout << edges[i].second << ' ';
    }
    cout << endl;
}



#ifdef DEBUG

std::ostream& operator<<(std::ostream& out, Graph::info::Point const& p) {

    out << "(" << p.x << ",\t" << p.y << ")";

    return out;
}

#endif // DEBUG



