#include "DynamicGraph.h"

size_t Find(vector<Node>* graph, const size_t vertex) {
    if (graph->at(vertex).parent == vertex) {
        return vertex;
    }
    graph->at(vertex).parent = Find(graph, graph->at(vertex).parent);
    return graph->at(vertex).parent;
}

DynamicGraph::DynamicGraph(int vertex_count) {
    size_ = vertex_count;
    edges.Initialize(size_);
    components_number_ = size_;
    StronglyConnectedComponents_.resize(size_);
    ConnectedComponents_.resize(size_);
    for (size_t index = 0; index < size_; ++index) {
        StronglyConnectedComponents_[index].parent = index;
        ConnectedComponents_[index].parent = index;
    }
}

void DynamicGraph::AddEdge(int u, int v) {
    int first_component = Find(&ConnectedComponents_, u);
    int second_component = Find(&ConnectedComponents_, v);
    if (first_component == second_component) {
        first_component = Find(&StronglyConnectedComponents_, u);
        second_component = Find(&StronglyConnectedComponents_, v);
        if (first_component == second_component) {
            edges.Add({{u, v}, 0});
        } else {
            edges.Add({{u, v}, 1});
            StronglyConnectedComponents_[first_component].parent = second_component;
        }
    } else {
        ConnectedComponents_[first_component].parent = second_component;
        Edge new_edge{{u, v}, 2};
        edges.Add(new_edge);
        --components_number_;
    }
}

int DynamicGraph::GetComponentsNumber() const {
    return components_number_;
}

void DynamicGraph::RemoveEdge(int u, int v) {
    edges.Remove({{u, v}, 0});
}
