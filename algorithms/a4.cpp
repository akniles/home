#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

using std::pair;
using std::vector;

struct InputData {
    vector<vector<int>> edges;
    vector<vector<int>> inversed_edges;
    size_t vertexes_number = 0;
    size_t edges_number = 0;
};

void ReadInput(InputData *data) {
    // std::ifstream input_file;
    // input_file.open("D:/yandex.school/algorithms/input.txt");
    // input_file >> data->vertexes_number;
    // input_file >> data->edges_number;
    std::cin >> data->vertexes_number;
    std::cin >> data->edges_number;
    data->edges.resize(data->vertexes_number);
    data->inversed_edges.resize(data->vertexes_number);
    int one, other, result;
    for (size_t index = 0; index < data->edges_number; ++index) {
        // input_file >> one >> other >> result;
        std::cin >> one >> other >> result;
        if (result == 1) {
            data->edges[one - 1].push_back(other - 1);
            data->inversed_edges[other - 1].push_back(one - 1);
        } else if (result == 2) {
            data->edges[other - 1].push_back(one - 1);
            data->inversed_edges[one - 1].push_back(other - 1);
        }
    }
    for (auto &adjs : data->edges) {
        std::sort(adjs.begin(), adjs.end());
    }
    for (auto &adjs : data->inversed_edges) {
        std::sort(adjs.begin(), adjs.end());
    }
}

void Paint(vector<int> *colors, vector<int> *linker, const InputData &data, size_t vertex) {
    colors->at(vertex) = 1;
    for (size_t index = 0; index < data.edges[vertex].size(); ++index) {
        if (colors->at(data.edges[vertex][index]) == 0) {
            Paint(colors, linker, data, data.edges[vertex][index]);
        }
    }
    linker->push_back(vertex);
}

void CountComponents(vector<int> *colors, vector<int> *scc,
                     vector<int> *component, const InputData &data, size_t vertex, int value) {
    colors->at(vertex) = 1;
    scc->at(vertex) = value;
    component->push_back(vertex);
    for (size_t index = 0; index < data.inversed_edges[vertex].size(); ++index) {
        if (colors->at(data.inversed_edges[vertex][index]) == 0) {
            CountComponents(colors, scc, component, data,
                            data.inversed_edges[vertex][index], value);
        }
    }
}

bool DidntLose(const vector<int> &scc, const InputData &data, const vector<int> &component) {
    auto color = scc[component[0]];
    for (const auto &neighbour : component) {
        for (const auto &brother : data.inversed_edges[neighbour]) {
            if (scc[brother] != color) {
                return false;
            }
        }
    }
    return true;
}

void Process(const InputData &data) {
    vector<int> colors(data.vertexes_number, 0);
    vector<int> scc(data.vertexes_number, 0);
    vector<int> component;
    vector<int> linker;
    size_t minimum_company_size = data.vertexes_number;
    linker.reserve(data.vertexes_number);
    component.reserve(data.vertexes_number);
    for (size_t index = 0; index < data.vertexes_number; ++index) {
        if (colors[index] == 0) {
            Paint(&colors, &linker, data, index);
        }
    }
    colors.assign(data.vertexes_number, 0);
    for (size_t index = 0; index < data.vertexes_number; ++index) {
        auto vertex = linker[data.vertexes_number - index - 1];
        if (colors[vertex] == 0) {
            CountComponents(&colors, &scc, &component, data, vertex, index + 1);
        }
        if (not component.empty() and component.size() < minimum_company_size) {
            if (DidntLose(scc, data, component)) {
                minimum_company_size = component.size();
            }
        }
        component.resize(0);
    }
    std::cout << data.vertexes_number - minimum_company_size + 1 << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    InputData data;
    ReadInput(&data);
    Process(data);
    return 0;
}
