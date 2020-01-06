#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <string>

using std::pair;
using std::vector;
using std::string;

struct InputData {
    vector<pair<int, int>> queries;
    size_t company_size;
    size_t queries_number;
};

class DSU {
private:
    struct Node {
        size_t parent = 0;
        size_t key = 0;
    };
    vector<Node> sets_;
    size_t size_;

public:
    explicit DSU(const size_t size) {
        size_ = size;
        sets_.resize(size_);
        size_t index = 0;
        for (auto &element : sets_) {
            element.key = index;
            element.parent = index;
            ++index;
        }
    }

    bool Check(const size_t one, const size_t other) {
        return Find(other) == other and Find(one) != other;
    }

    size_t Find(const size_t index) {
        if (sets_[sets_[index].parent].key == index) {
            return index;
        }
        sets_[sets_[index].parent].key = Find(sets_[sets_[index].parent].key);
        return sets_[sets_[index].parent].key;
    }

    void Join(const size_t &one, const size_t &other) {
        sets_[other].parent = one;
    }
};

void ReadInput(InputData *data) {
    // std::ifstream input_file;
    // input_file.open("D:/yandex.school/algorithms/input.txt");

    // input_file >> data->company_size;
    // input_file >> data->queries_number;
    std::cin >> data->company_size;
    std::cin >> data->queries_number;
    data->queries.resize(data->queries_number);
    for (auto &query : data->queries) {
        string line;
        std::getline(std::cin, line);
        if (line.empty()) {
            std::getline(std::cin, line);
        }
        int parser = line.find_first_of(' ');
        if (parser != -1) {
            query.first = std::stoi(line.substr(0, parser));
            query.second = std::stoi(line.substr(parser + 1));
        } else {
            query.first = std::stoi(line);
            query.second = -1;
        };
    }
}

void Process (const InputData& data) {
    DSU sets(data.company_size);
    for (const auto& command : data.queries) {
        if (command.second == -1) {
            std::cout << sets.Find(command.first - 1) + 1 << "\n";
        } else {
            if (sets.Check(command.first - 1, command.second - 1)) {
                sets.Join(command.first - 1, command.second - 1);
                std::cout << "1\n";
            } else {
                std::cout << "0\n";
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    InputData data;
    ReadInput(&data);
    Process(data);
    return 0;
}
