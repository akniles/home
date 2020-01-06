#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <string>
#include <vector>
#include <iterator>

using std::vector;
using std::string;

struct Pair {
    size_t index;
    vector<int> array;
};

vector<Pair> Input() {

    struct CustomMore {
        inline bool operator()(const vector<int> &one, const vector<int> &other) {
            return (one[0] > other[0]);
        }
    };

    int number_arrays = 0, length = 0;
    std::cin >> number_arrays >> length;
    std::vector<vector<int>> data(number_arrays, std::vector<int>(length, 0));

    for (auto &array : data) {
        for (auto &element : array) {
            std::cin >> element;
        }
    }
    std::sort(data.begin(), data.end(), CustomMore());

    vector<Pair> data_with_indexes;
    data_with_indexes.reserve(number_arrays);
    for (const auto &array : data) {
        data_with_indexes.push_back({0, array});
    }

    return data_with_indexes;
}

template<class T>
void Output(const T &sequence) {
    for (const auto &element : sequence) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
}

vector<int> Process (const vector<Pair>& data) {
    vector<int> result;
    for (const auto& pair : data) {
        for (const auto& element : pair.array) {
            result.push_back(element);
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    vector<Pair> data = Input();
    const vector<int> result = Process(data);
    Output(result);
    return 0;
}
