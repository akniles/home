#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

using std::pair;
using std::vector;

struct Road {
    int city = 0;
    int price = 0;
};

struct InputData {
    vector<vector<Road>> edges;
    size_t cities = 0;
    size_t roads = 0;
};

void ReadInput(InputData *data) {
    // std::ifstream input_file;
    // input_file.open("D:/yandex.school/algorithms/input.txt");
    // input_file >> data->cities;
    // input_file >> data->roads;
    std::cin >> data->cities;
    std::cin >> data->roads;
    data->edges.resize(data->cities);
    for (auto &city : data->edges) {
        city.resize(0);
        city.reserve(10);
    }
    int one, other, price;
    for (size_t index = 0; index < data->roads; ++index) {
        // input_file >> one >> other >> price;
        std::cin >> one >> other >> price;
        data->edges[one - 1].push_back({other - 1, price});
        data->edges[other - 1].push_back({one - 1, price});
    }
}

void SearchBridges(const InputData &data, vector<int> *colors,
                   vector<int> *visitin_time, vector<int> *leaving_time, int *time,
                   vector<int> *bridges, int vertex = 0, int position = -1) {
    colors->at(vertex) = 1;
    ++*time;
    visitin_time->at(vertex) = *time;
    leaving_time->at(vertex) = *time;
    for (const auto &edge : data.edges[vertex]) {
        int next = edge.city;
        if (next == position) {
            continue;
        }
        if (colors->at(next)) {
            leaving_time->at(vertex) = std::min(leaving_time->at(vertex), visitin_time->at(next));
        } else {
            SearchBridges(data, colors, visitin_time, leaving_time, time, bridges, next, vertex);
            leaving_time->at(vertex) = std::min(leaving_time->at(vertex), leaving_time->at(next));
            if (leaving_time->at(next) > visitin_time->at(vertex)) {
                bridges->push_back(edge.price);
            }
        }
    }
}

void FindCheapestBridge(const vector<int> &bridges, const InputData &data) {
    int minimum_price = 2e9;
    for (const auto &bridge : bridges) {
        if (bridge < minimum_price) {
            minimum_price = bridge;
        }
    }
    if (minimum_price == 2e9 or bridges.empty()) {
        std::cout << "-1\n";
    } else {
        std::cout << minimum_price << "\n";
    }
}

void Process(const InputData &data) {
    vector<int> bridges;
    bridges.reserve(data.roads);
    vector<int> colors(data.cities, 0);
    int time = 0;
    vector<int> visiting_time(data.cities, 0);
    vector<int> leaving_time(data.cities, 0);
    SearchBridges(data, &colors, &visiting_time, &leaving_time, &time, &bridges);
    FindCheapestBridge(bridges, data);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    InputData data;
    ReadInput(&data);
    Process(data);
    return 0;
}
