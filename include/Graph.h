
#ifndef GRAPH_H
#define GRAPH_H
#include <vector> // need
#include <utility> //need for std::pair
class Graph {
public:
    int WIDTH = 800;
    int HEIGHT = 400;
    int cities = -1;
    int lenOfWay = -1;
private:
    bool is_checked_connected = false;
    bool is_conn = false;
public:


    struct info {
        struct Point {
            double x;
            double y;

            Point& operator+=(Point const& other);
            Point& operator-=(Point const& other);
            Point& operator/=(double other);
            Point& operator*=(double other);
            Point operator/(double other) const;
            Point operator*(double other) const;
            Point operator+(Point const& other) const;
            Point operator-(Point const& other) const;

#ifdef DEBUG
            friend std::ostream& operator<<(std::ostream& out, Graph::info::Point const& p);
#endif

            double abs() const;
            bool little() const;
            void fit(double max_size);
            void bounds(Point const& from, Point const& to);
        };
        Point p;
        int group;
    };

    std::vector<int> ptrs;
    std::vector<std::pair<int, int>> edges;
    std::vector<info> infos;

   


    int read_from_file(std::string const& s);
    void init_info();
    void process(int iters = 1);
    void process2(int iters = 1);

    bool is_connected();
    std::vector<Graph> split();
    void show() const;
};

#endif // GRAPH_H
