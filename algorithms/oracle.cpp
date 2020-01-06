#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <ratio>
#include "graph_oracle.h"

const double TIME_LIMIT = 4.0;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    GraphOracle oracle(vertex_count);
    for (int i = 0; i < edge_count; ++i) {
        int u, v, cost;
        std::cin >> u >> v >> cost;
        oracle.add_edge(--u, --v, cost);
    }
    oracle.initialize();
    int query_count;
    auto start = std::clock();
    std::cin >> query_count;
    for (int i = 0; i < query_count; ++i) {
        int u, v;
        std::cin >> u >> v;
        std::cout << oracle.estimate_path(--u, --v) << "\n";
    }
    auto end = std::clock();
    auto duration = (end - start) / static_cast<double> (CLOCKS_PER_SEC);
    std::cerr << std::fixed << std::setprecision(3);
    std::cerr << "Answer for " << duration << " seconds\n";
    if (duration > TIME_LIMIT) {
        std::cerr << "Too slow\n";
        return 1;
    }
    return 0;
}