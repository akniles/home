#ifndef ALGORITHMS_GRAPH_ORACLE_H
#define ALGORITHMS_GRAPH_ORACLE_H

#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

int find_set(int child, vector<int> *tree) {
    if (child == tree->at(child))
        return child;
    tree->at(child) = find_set(tree->at(child), tree);
    return tree->at(child);
}

class GraphOracle {
public:
    explicit GraphOracle(int vertex_count) {
        size_ = vertex_count;
        tree_size_ = 0;
        edges_.resize(vertex_count, vector<Edge>(0));
        tree_.resize(vertex_count, vector<Edge>(0));
    }

    void add_edge(int first, int second, int cost) {
        edges_[first].push_back({first, second, cost});
        edges_[second].push_back({first, second, cost});
        all_edges_.push_back({first, second, cost});
    }

    int64_t estimate_path(int first, int second);

    void initialize() {
        std::sort(all_edges_.begin(), all_edges_.end());
        vector<int> tree(size_);
        int64_t sum = 0;
        int number = 0;
        vector<Edge> fat;
        for (size_t index = 0; index < size_; ++index) {
            tree[index] = index;
        }
        size_t index = 0;
        for (const auto &edge : all_edges_) {
            int first = edge.first;
            int second = edge.second;
            auto first_parent = find_set(first, &tree);
            auto second_parent = find_set(second, &tree);
            if (first_parent != second_parent) {
                tree_[first].push_back(edge);
                tree_[second].push_back(edge);
                tree[second_parent] = first_parent;
                sum += edge.price;
                ++number;
            } else {
                if (edge.price * number < sum * 100) {
                    fat.push_back(edge);
                }
            }
            ++index;
        }
        tree_size_ = tree_.size();
        for (const auto& edge : fat) {
            auto price = get_path(edge.first, edge.second);
            if (price > edge.price * 5) {
                tree_[edge.first].push_back(edge);
                tree_[edge.second].push_back(edge);
            }
        }
    }


        private:
    struct Edge {
        int first = 0;
        int second = 0;
        int64_t price = 0;

        bool operator<(Edge other) {
            return price < other.price;
        }
    };

    size_t size_;
    size_t tree_size_;
    vector<vector<Edge>> edges_;
    vector<vector<Edge>> tree_;
    vector<Edge> all_edges_;

    int64_t get_path(int first, int second) {
        if (first == second) {
            return 0;
        }
        auto current = first;
        vector<int> colors(tree_size_, 0);
        vector<int> queue;
        vector<int> distances;
        queue.reserve(tree_size_);
        distances.reserve(tree_size_);
        for (const auto &edge : tree_[first]) {
            auto neightbour = edge.second;
            if (current == neightbour) {
                neightbour = edge.first;
            }
            queue.push_back(neightbour);
            distances.push_back(edge.price);
            colors[neightbour] = 1;
        }
        size_t index = 0;
        current = queue[index];
        while (current != second) {
            for (const auto &edge : tree_[current]) {
                auto neightbour = edge.second;
                if (current == neightbour) {
                    neightbour = edge.first;
                }
                if (colors[neightbour] == 1) {
                    continue;
                }
                queue.push_back(neightbour);
                distances.push_back(edge.price + distances[index]);
                colors[neightbour] = 1;
            }
            ++index;
            current = queue[index];
        }
        return distances[index];
    }
};

int64_t GraphOracle::estimate_path(int first, int second) {
    return get_path(first, second);
}

#endif // ALGORITHMS_GRAPH_ORACLE_H
