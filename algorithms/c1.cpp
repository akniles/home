#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

size_t BinSearch(const std::vector<int>& raising_arrays, 
    const std::vector<int>& declaining_arrays) {
    size_t start = 0;
    size_t end = raising_arrays.size() - 1;
    size_t diff = end - start;
    size_t middle = start + diff / 2;
    while (diff > 1) {
        if (raising_arrays[middle] - declaining_arrays[middle] > 0) {
            end = middle;
        } else {
            start = middle;
        }
        diff = end - start;
        middle = start + diff / 2;
    }
    return start;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int raising_arrays_number;
    int declaining_arrays_number;
    int size;
    int queries_number;

    std::cin >> raising_arrays_number >> declaining_arrays_number >> size;
    
    vector<vector<int>> raising_arrays(raising_arrays_number, vector<int>(size, 0));
    vector<vector<int>> declaining_arrays(declaining_arrays_number, vector<int>(size, 0));

    for (auto& array : raising_arrays) {
        for (auto& element : array) {
            std::cin >> element;
        }
    } 
    for (auto& array : declaining_arrays) {
        for (auto& element : array) {
            std::cin >> element;
        }
    }  

    std::cin >> queries_number;

    for (int index = 0; index < queries_number; ++index) {
        int first_index;
        int second_index;
        std::cin >> first_index >> second_index;
        size_t left = BinSearch(raising_arrays[first_index - 1], 
            declaining_arrays[second_index - 1]);
        if (raising_arrays[first_index - 1][left + 1] < 
            declaining_arrays[second_index - 1][left]) {
            left += 1;
        }
        std::cout << left + 1 << "\n";
    }
    return 0;
}
