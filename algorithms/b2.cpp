#include <random>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <vector>

using std::vector;

struct Pair{
    int64_t power;
    int64_t index;
};

void Input (vector<Pair>* sorted_data, vector<int64_t>* data) {

    struct CustomLess {
        inline bool operator() (const Pair& one, const Pair& other) {
            return (one.power < other.power);
        }
    };

    int64_t players;
    std::cin >> players;
    data->reserve(players);
    
    for (int64_t i = 0; i < players; ++i) {
        Pair pair{0, i};
        std::cin >> pair.power;
        sorted_data->push_back(pair);
        data->push_back(pair.power);
    }

    std::sort(sorted_data->begin(), sorted_data->end(), CustomLess());
}

template<class T>
void Output (const T& sequence, size_t length, int64_t min_element, size_t min_count, 
    const vector<int64_t>& data) {
    int64_t time  = 0;
    for (const auto& element : sequence) {
        if (data[element] == min_element and time + sequence.size() < min_count + length) {
            std::cout << element + 1 << ' ';
            time += 1;
        } else if (data[element] != min_element) {
            std::cout << element + 1 << ' ';
        }
    }
    std::cout << '\n';
}

void Process (const vector<Pair>& sorted_data, const vector<int64_t>& data) {
    int64_t sum = 0;
    if (data.size() < 3) {
        vector<int64_t> indexes;
        for (size_t i = 0; i < data.size(); ++i) {
            indexes.push_back(i);
            sum += sorted_data[i].power;
        }
        std::cout << sum << '\n';
        Output(indexes, data.size(), 0, data.size(), data);
        return;
    }
    int64_t length = 0;
    int64_t best_end = 0;
    int64_t best_start = 0;
    size_t end = 0;
    int64_t maximum_power = 0;

    for (size_t start = 0; start + 1 < data.size(); ++start) {
        int64_t minimum_power = sorted_data[start].power + sorted_data[start + 1].power;
        while (end < data.size()) {
            if (sorted_data[end].power > minimum_power) {
                break;
            }
            sum += sorted_data[end].power;
            end += 1;
        }
        if (sum >= maximum_power) {
            best_start = sorted_data[start].power;
            best_end = sorted_data[end - 1].power;
            length = end - start;
            maximum_power = sum;
        }
        sum -= sorted_data[start].power;
    }
    vector<int64_t> result;
    result.reserve(10e5);
    size_t min_count = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] >= best_start and data[i] <= best_end) {
            result.push_back(i);
            if (data[i] == best_start) {
                min_count += 1;
            }
        }
    }

    std::cout << maximum_power << '\n';
    Output(result, length, best_start, min_count, data);
}

void RandomData (vector<Pair>* sorted_data, vector<int64_t>* data, int size, int seed) {

    struct CustomLess {
        inline bool operator() (const Pair& one, const Pair& other) {
            return (one.power < other.power);
        }
    };
    data->reserve(size);
    sorted_data->reserve(size);
    
    std::mt19937 generator(size + seed);
    std::uniform_int_distribution<int64_t> random_seed(1, 2147483647);

    for (int64_t i = 0; i < static_cast<int64_t>(size); ++i) {
        Pair pair{0, i};
        pair.power = random_seed(generator);
        sorted_data->push_back(pair);
        data->push_back(pair.power);
    }

    std::sort(sorted_data->begin(), sorted_data->end(), CustomLess());
}

void StressTest () {
    for (int time = 1; time < 10e5; ++time) {
        std::cout << "test with time: " << time << '\n';
        vector<int64_t> data;
        vector<Pair> sorted_data;
        RandomData(&sorted_data, &data, 10, time);
        Process(sorted_data, data);
        // std::cout << result << '\n';
    }
}

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    vector<int64_t> data;
    vector<Pair> sorted_data;
    Input(&sorted_data, &data);
    Process(sorted_data, data);
    return 0;
}
