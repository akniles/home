#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using std::vector;

void ReadInput(vector<int> *data) {
    // std::ifstream input;
    // input.open("D:/yandex.school/algorithms/input.txt");
    size_t size;
    std::cin >> size;
    data->resize(size);
    for (auto &element : *data) {
        std::cin >> element;
    }
    std::sort(data->begin(), data->end());
}

int64_t Process(const vector<int> &data) {
    const int modulo = 123456789;
    const auto size = data.size();
    vector<vector<int64_t>> table(size + 1, vector<int64_t>(size + 1, 0));
    for (size_t index = 0; index < size; ++index) {
        table[index][index] = 1;
        table[index][index + 1] = 1;
    }
    table[size][size] = 1;
    for (size_t subtree_size = 2; subtree_size <= size; ++subtree_size) {
        for (size_t column = 0; column + subtree_size <= size; ++column) {
            for (size_t row = column; row < column + subtree_size; ++row) {
                if (data[row] != data[row - 1] or row == column) {
                    table[column][column + subtree_size] +=
                            (table[column][row] * table[row + 1][column + subtree_size]) % modulo;
                    table[column][column + subtree_size] %= modulo;
                }
            }
        }
    }
    return table[0][size];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    vector<int> data;
    ReadInput(&data);
    auto result = Process(data);
    std::cout << result << "\n";
    return 0;
}
