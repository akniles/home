//
// Created by Admin on 20.12.2019.
//

#include "graph_oracle.h"

/*void initialize() {
        std::sort(all_edges_.begin(), all_edges_.end());
        vector<int> tree(size_);
        vector<Edge> fat;
        for (size_t index = 0; index < size_; ++index) {
            tree[index] = index;
        }
        for (auto &edge : all_edges_) {
            int first = edge.first;
            int second = edge.second;
            auto first_parent = find_set(first, &tree);
            auto second_parent = find_set(second, &tree);
            if (first_parent != second_parent) {
                tree_[first].push_back(edge);
                tree_[second].push_back(edge);
                tree[second_parent] = first_parent;
            } else {
                fat.push_back(edge);
            }
        }
        tree_size_ = tree_.size();
    }
    for (const auto &edge : fat) {
            auto price = get_path(edge.first, edge.second);
            if (price > edge.price * 5) {
                tree_[edge.first].push_back(edge);
                tree_[edge.second].push_back(edge);
            }
        }*/
