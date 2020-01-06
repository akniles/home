#include <iostream>
#include <utility>
#include <vector>
#include "fixed_set.h"

#ifndef ALGORITHMS_DYNAMICGRAPH_H
#define ALGORITHMS_DYNAMICGRAPH_H

using std::vector;
using std::pair;

struct Node {
    size_t parent = 0;
    size_t weight = 0;
};

struct Edge {
    pair<int, int> edge;
    int status = 0;
};

class Compare {
public:
    inline size_t operator()(const Edge& one) const {
        return static_cast<int64_t>(one.edge.first) * one.edge.second + 7 * (one.edge.first + one.edge.second);
    }
};

class DynamicGraph {
private:
    size_t size_;
    size_t components_number_;
    vector<Node> ConnectedComponents_;
    vector<Node> StronglyConnectedComponents_;
    FixedSet<Edge, Compare> edges;

public:
    explicit DynamicGraph(int vertex_count);

    void AddEdge(int u, int v);

    void RemoveEdge(int u, int v);

    int GetComponentsNumber() const;
};


#endif //ALGORITHMS_DYNAMICGRAPH_H
