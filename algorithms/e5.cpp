#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <random>
#include <ctime>
#include <cmath>
#include <string>

using std::pair;
using std::vector;

struct InputData {
    vector<pair<int, int>> queries;
    vector<char> commands;
    size_t command_size = 0;
};

class LSA {
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
    explicit LSA(size_t size = 200000);

    void Kill(const size_t &one);

    size_t Shift(size_t one);

    size_t Lift(size_t first, size_t second);

    size_t Judge(size_t first, size_t second);

    void Birth(size_t father);
};

LSA::LSA(const size_t size) {
    max_size_ = size;
    size_ = 1;
    log_size_ = std::log2(size) + 1;
    sets_.resize(max_size_, Node(log_size_));
}

void LSA::Kill(const size_t &one) {
    sets_[one].living_neighbour = sets_[one].parents[0];
}

size_t LSA::Shift(size_t one) {
    if (sets_[one].living_neighbour == one) {
        return one;
    }
    return sets_[one].living_neighbour = Shift(sets_[one].parents.front());
}

size_t LSA::Lift(size_t first, size_t second) {
    int index = static_cast<int>(log_size_) - 1;
    while (index) {
        if (sets_[first].parents[index] != sets_[second].parents[index]) {
            first = sets_[first].parents[index];
            second = sets_[second].parents[index];
        }
        --index;
    }
    return first;
}

size_t LSA::Judge(size_t first, size_t second) {
    if (first == second) {
        return first;
    }
    if (sets_[first].height > sets_[second].height) {
        std::swap(first, second);
    }
    for (int index = static_cast<int>(log_size_) - 1; index >= 0; --index) {
        if (sets_[second].height < sets_[first].height) {
            second = sets_[second].parents[index];
        }
    }

    if (first == second) {
        return first;
    }

    auto result = Lift(first, second);
    result = Shift(result);
    return result;
}

void LSA::Birth(const size_t father) {
    sets_[size_].parents[0] = father;
    sets_[size_].living_neighbour = size_;
    sets_[size_].height = sets_[father].height + 1;
    for (size_t index = 0; index + 1 < log_size_; ++index) {
        sets_[size_].parents[index + 1] = sets_[sets_[size_].parents[index]].parents[index];
    }
    ++size_;
}

void ReadInput(InputData *data) {
    // std::ifstream input_file;
    // input_file.open("D:/yandex.school/algorithms/input.txt");

    std::cin >> data->command_size;
    data->queries.resize(data->command_size, pair<int, int>{-1, -1});
    data->commands.resize(data->command_size);
    for (size_t index = 0; index < data->command_size; ++index) {
        std::cin >> data->commands[index];
        if (data->commands[index] == '?') {
            std::cin >> data->queries[index].first;
            std::cin >> data->queries[index].second;
        } else {
            std::cin >> data->queries[index].first;
        }
    }
}

void Process(const InputData &data) {
    LSA sets;
    std::cout << "wtf?\n";
    size_t index = 0;
    for (const auto &command : data.commands) {
        if (command == '+') {
            sets.Birth(data.queries[index].first - 1);
        } else if (command == '-') {
            sets.Kill(data.queries[index].first - 1);
        } else {
            std::cout << sets.Judge(data.queries[index].first - 1,
                                    data.queries[index].second - 1) + 1 << "\n";
        }
        ++index;
    }
}

void Test() {

    // std::mt19937 generator(time(nullptr));
    size_t size = 50000;
    // std::uniform_int_distribution<int> random_seed(2, 4 * size);
    InputData data;
    data.command_size = size;
    data.commands.push_back('+');
    data.commands.push_back('+');
    data.queries.emplace_back(1, -1);
    data.queries.emplace_back(1, -1);
    for (size_t index = 2; index < size; ++index) {
        data.commands.push_back('+');
        data.queries.emplace_back(index, -1);
    }
    for (size_t index = 0; index < size; ++index) {
        data.commands.push_back('?');
        data.queries.emplace_back(size, size - 1);
    }
    std::clock_t c_start = std::clock();
    Process(data);
    std::clock_t c_end = std::clock();

    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: ";
}

int main() {
    // std::ios_base::sync_with_stdio(false);
    // std::cin.tie(nullptr);
    InputData data;
    ReadInput(&data);
    Process(data);
    // Test();
    return 0;
}
