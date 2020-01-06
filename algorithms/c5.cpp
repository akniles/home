#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cmath>

using std::pair;
using std::vector;

vector<int> neighbours_bridges;

struct Road {
    int city = 0;
    int color = 0;
};

struct Graph {
    vector<vector<Road>> edges;
    size_t capital = 0;
    size_t cities = 0;
    size_t roads = 0;
};

class LCA {
private:
    struct Node {
        explicit Node(size_t size) {
            parents.resize(size);
        }

        vector<size_t> parents;
        size_t living_neighbour = 0;
        size_t height = 0;
    };

    vector<Node> sets_;
    size_t max_size_;
    size_t size_;
    size_t log_size_;

public:
    explicit LCA(size_t size = 200000);

    void Kill(const size_t &one);

    size_t Shift(size_t one);

    size_t Lift(size_t first, size_t second);

    size_t Judge(size_t first, size_t second);

    void Birth(int father = -1);
};

LCA::LCA(const size_t size) {
    max_size_ = size;
    size_ = 1;
    log_size_ = std::log2(size) + 1;
    sets_.resize(max_size_, Node(log_size_));
}

void LCA::Kill(const size_t &one) {
    sets_[one].living_neighbour = sets_[one].parents.front();
}

size_t LCA::Shift(size_t one) {
    if (sets_[one].living_neighbour == one) {
        return one;
    }
    return sets_[one].living_neighbour = Shift(sets_[one].living_neighbour);
}

size_t LCA::Lift(size_t first, size_t second) {
    int index = static_cast<int>(log_size_) - 1;
    while (index >= 0) {
        if (sets_[first].parents[index] != sets_[second].parents[index]) {
            first = sets_[first].parents[index];
            second = sets_[second].parents[index];
        }
        --index;
    }
    return first;
}

size_t LCA::Judge(size_t first, size_t second) {
    if (first == second) {
        return sets_[first].height;
    }
    if (sets_[first].height > sets_[second].height) {
        std::swap(first, second);
    }
    for (int index = static_cast<int>(log_size_) - 1; index >= 0; --index) {
        if (sets_[second].height >= sets_[first].height + std::pow(2, index)) {
            second = sets_[second].parents[index];
        }
    }

    if (first == second) {
        return sets_[first].height;
    }

    auto result = Lift(first, second);
    return sets_[sets_[result].parents.front()].height;
}

void LCA::Birth(const int father) {
    if (father == -1 and size_ != 0) {
        Birth(static_cast<int>(size_) - 1);
        return;
    } else if (size_ == 0) {
        Birth(0);
    }
    sets_[size_].parents[0] = father;
    sets_[size_].living_neighbour = size_;
    sets_[size_].height = sets_[father].height + 1;
    for (size_t index = 0; index + 1 < log_size_; ++index) {
        sets_[size_].parents[index + 1] = sets_[sets_[size_].parents[index]].parents[index];
    }
    ++size_;
}

void ReadInput(Graph *graph) {
    // std::ifstream input_file;
    // input_file.open("D:/yandex.school/algorithms/input.txt");
    // input_file >> graph->cities;
    // input_file >> graph->roads;
    std::cin >> graph->cities;
    std::cin >> graph->roads;
    std::cin >> graph->capital;
    --graph->capital;
    graph->edges.resize(graph->cities);
    for (auto &city : graph->edges) {
        city.resize(0);
        city.reserve(10);
    }
    for (size_t index = 0; index < graph->roads; ++index) {
        int one, other;
        std::cin >> one >> other;
        graph->edges[one - 1].push_back({other - 1, 0});
        graph->edges[other - 1].push_back({one - 1, 0});
    }
}

void Mark(Graph *graph, const int first, const int second) {
    for (auto &edge : graph->edges[first]) {
        if (edge.city == second) {
            edge.color = -1;
            return;
        }
    }
}

void SearchBridges(Graph *graph, vector<int> *colors,
                   vector<int> *visitin_time, vector<int> *leaving_time, int *time,
                   vector<pair<int, int>> *bridges, int vertex = 0, int position = -1) {
    colors->at(vertex) = 1;
    ++*time;
    visitin_time->at(vertex) = *time;
    leaving_time->at(vertex) = *time;
    for (auto &edge : graph->edges[vertex]) {
        int next = edge.city;
        if (next == position) {
            continue;
        }
        if (colors->at(next)) {
            leaving_time->at(vertex) = std::min(leaving_time->at(vertex), visitin_time->at(next));
        } else {
            SearchBridges(graph, colors, visitin_time, leaving_time, time, bridges, next, vertex);
            leaving_time->at(vertex) = std::min(leaving_time->at(vertex), leaving_time->at(next));
            if (leaving_time->at(next) > visitin_time->at(vertex)) {
                bridges->emplace_back(vertex, next);
                edge.color = -1;
                Mark(graph, next, vertex);
            }
        }
    }
}

void Dfs(Graph *graph, int start, LCA *tree, vector<int> *colors,
         size_t index, vector<pair<int, int>> *queue) {
    colors->at(start) = index;
    for (auto &edge : graph->edges[start]) {
        if (colors->at(edge.city) != 0) {
            continue;
        }
        if (edge.color == -1) {
            tree->Birth(static_cast<int>(index - 1));
            edge.color = -2;
            queue->push_back({start, edge.city});
        }
        if (edge.color == 0) {
            edge.color = index;
            Dfs(graph, edge.city, tree, colors, index, queue);
        }
    }
}

void Bfs(Graph *graph, int start, LCA *tree, vector<int> *colors) {
    vector<pair<int, int>> queue;
    queue.reserve(graph->edges[start].size());
    size_t index = 1;
    size_t position = 0;
    do {
        Dfs(graph, start, tree, colors, index, &queue);
        start = queue[position].second;
        ++position;
        ++index;
    } while (position <= queue.size() and queue.size() < graph->cities);
}

void Process(Graph *graph) {
    vector<pair<int, int>> bridges;
    bridges.reserve(graph->roads);
    // neighbours_bridges.resize(graph->cities);
    vector<int> colors(graph->cities, 0);
    int time = 0;
    vector<int> visiting_time(graph->cities, 0);
    vector<int> leaving_time(graph->cities, 0);
    SearchBridges(graph, &colors, &visiting_time, &leaving_time, &time, &bridges);

    int size = static_cast<int>(bridges.size()) + 1;
    LCA tree(size);
    colors.assign(graph->cities, 0);
    Bfs(graph, graph->capital, &tree, &colors);
    std::cin >> size;
    vector<int> result;
    for (int index = 0; index < size; ++index) {
        int first, second;
        std::cin >> first >> second;
        result.push_back(tree.Judge(colors[first - 1] - 1, colors[second - 1] - 1));
    }
    for (const auto &answer : result) {
        std::cout << answer << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Graph graph;
    ReadInput(&graph);
    Process(&graph);
    return 0;
}
