#include <vector>
#include <iostream>
#include <iomanip>

using std::vector;

int main() {
    size_t size;
    size_t height;
    std::cin >> size;
    std::cin >> height;
    if (height >= size) {
        std::cout << "0\n";
        return 0;
    }
    vector<vector<double>> table(size + 2, vector<double>(size + 2, 0));
    for (size_t index = 0; index <= size; ++index) {
        for (size_t jindex = index; jindex <= size; ++jindex) {
            table[index][jindex] = 1;
        }
    }
    for (size_t index_i = 2; index_i <= size; ++index_i) {
        for (size_t index_j = 1; index_j < index_i; ++index_j) {
            for (size_t h = 1; h <= index_i; ++h) {
                table[index_i][index_j] += (table[h - 1][index_j - 1] *
                                            table[index_i - h][index_j - 1]);
            }
            table[index_i][index_j] /= index_i;
        }
    }

    std::cout << std::setprecision(20)
              << static_cast<double>(table[size][height + 1] - table[size][height])
              << "\n";
    return 0;
}
