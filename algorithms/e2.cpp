#include <iomanip>
#include <random>
#include <iostream>
#include <chrono>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <vector>

using std::vector;

struct Pair {
    int coordinate;
    size_t time;
};

vector<Pair> Input() {

    struct CustomLessCoordinate {
        inline bool operator()(const Pair &one, const Pair &other) {
            return (one.coordinate < other.coordinate);
        }
    };

    size_t size;
    std::cin >> size;
    vector<Pair> data(size);
    for (auto &pair : data) {
        std::cin >> pair.coordinate >> pair.time;
    }
    std::sort(data.begin(), data.end(), CustomLessCoordinate());
    return data;
}

template<class T>
void Output(const T &sequence) {
    for (const auto &element : sequence) {
        std::cout << element.coordinate << ' ' << element.time << "\n";
    }
    std::cout << '\n';
}

void CalculatePoints(vector<vector<int>> *table,
                     const vector<Pair> &data, size_t line, size_t row, int side) {
    size_t end = row + line;
    if (side == 0) {
        int first = table->at(line + 1)[end] - data[line].coordinate + data[line + 1].coordinate;
        int second = table->at(end)[line + 1] - data[line].coordinate + data[end].coordinate;
        size_t value = std::min(first, second);
        table->at(line)[end] = value > data[line].time ? 2e9 : value;
    } else {
        int first = table->at(line)[end - 1] - data[line].coordinate + data[end].coordinate;
        int second = table->at(end - 1)[line] - data[end - 1].coordinate + data[end].coordinate;
        size_t value = std::min(first, second);
        table->at(end)[line] = value > data[end].time ? 2e9 : value;
    }
}

void PrintTable(const vector<vector<int>> &table) {
    for (const auto &row : table) {
        for (const auto &element : row) {
            std::cout << element << " ";
        }
        std::cout << "\n";
    }
}

int MakeTable(const vector<Pair> &data) {
    size_t size = data.size();
    vector<vector<int>> table(size, vector<int>(size, 0));
    for (size_t row = 1; row < size; ++row) {
        for (size_t line = 0; row + line < size; ++line) {
            CalculatePoints(&table, data, line, row, 0);
        }
        for (size_t line = 0; row + line < size; ++line) {
            CalculatePoints(&table, data, line, row, 1);
        }
    }
    return std::min(table[0][size - 1], table[size - 1][0]);
}

int main() {
    const vector<Pair> data = Input();
    int result = MakeTable(data);
    if (result == 2e9) {
        std::cout << "No solution\n";
    } else {
        std::cout << result;
    }
    return 0;
}
