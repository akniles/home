#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>

using std::vector;

struct Value {
    int number = 0;
    size_t index = 0;
};

class MaxHeap {
private:
    vector<Value> heap_;
    size_t size_ = 0;
    size_t max_size_ = 0;


public:
    void Initialize(size_t size) {
        max_size_ = size + 1;
        heap_.resize(max_size_);
    }

    size_t size() {
        return size_;
    }

    bool empty() {
        return size_ == 0;
    }

    int GetMax() {
        return heap_[0].number;
    }

    int GetAfterLast() {
        return heap_[size_].number;
    }

    void SiftDown(size_t index, vector<size_t> *indexes) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t more = index;
        if (left < size_ and heap_[left].number > heap_[index].number) {
            more = left;
        }
        if (right < size_ && heap_[right].number > heap_[more].number) {
            more = right;
        }
        if (more != index) {
            std::swap(heap_[index], heap_[more]);
            std::swap(indexes->at(heap_[more].index),
                      indexes->at(heap_[index].index));
            SiftDown(more, indexes);
        }
    }

    Value ExtractMax(vector<size_t> *indexes) {
        if (size_ == 0) {
            return {-1, 0};
        } else {
            Value maximum = heap_[0];
            std::swap(indexes->at(heap_[0].index),
                      indexes->at(heap_[size_ - 1].index));
            heap_[0] = heap_[size_ - 1];
            --size_;
            SiftDown(0, indexes);
            return maximum;
        }
    }

    void Insert(int value, vector<size_t> *indexes, size_t position) {
        size_t index = size_;
        heap_[index] = {value, position};
        indexes->at(position) = index;
        ++size_;

        while (index != 0 && heap_[(index - 1) / 2].number < heap_[index].number) {
            std::swap(heap_[index], heap_[(index - 1) / 2]);
            std::swap(indexes->at(heap_[(index - 1) / 2].index),
                      indexes->at(heap_[index].index));
            index = (index - 1) / 2;
        }
    }

    void SiftUp(vector<size_t>* indexes, size_t index) {
        if (index == 0) {
            return;
        }
        size_t previous = (index - 1) / 2;
        if (heap_[previous].number < heap_[index].number) {
            std::swap(heap_[index], heap_[previous]);
            std::swap(indexes->at(heap_[previous].index),
                      indexes->at(heap_[index].index));
            SiftUp(indexes, previous);
        }
    }

    void Remove(int index, vector<size_t> *indexes) {
        if (size_ == 0) {
            throw std::invalid_argument("deleting from empty heap");
        }
        std::swap(heap_[index], heap_[size_ - 1]);
        std::swap(indexes->at(heap_[size_ - 1].index),
                  indexes->at(heap_[index].index));
        --size_;
        if (size_ == 0) {
            return;
        }
        int previous = static_cast<int>(index - 1) / 2;
        if (heap_[previous].number < heap_[index].number) {
            SiftUp(indexes, index);
        } else {
            SiftDown(index, indexes);
        }
    }

    void PrintHeap() {
        std::cout << "size: " << size_ << "\n";
        for (size_t index = 0; index < size_; ++index) {
            std::cout << heap_[index].number << " ";
        }
        std::cout << "\n";
    }
};

class MinHeap {
private:
    vector<Value> heap_;
    size_t size_ = 0;
    size_t max_size_ = 0;

public:
    void Initialize(size_t size) {
        max_size_ = size + 1;
        heap_.resize(max_size_);
    }

    size_t size() {
        return size_;
    }

    bool empty() {
        return size_ == 0;
    }

    int GetMin() {
        return heap_[0].number;
    }

    int GetAfterLast() {
        return heap_[size_].number;
    }

    void SiftDown(size_t index, vector<size_t> *indexes) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t less = index;
        if (left < size_ and heap_[left].number < heap_[less].number) {
            less = left;
        }
        if (right < size_ && heap_[right].number < heap_[less].number) {
            less = right;
        }
        if (less != index) {
            std::swap(heap_[index], heap_[less]);
            std::swap(indexes->at(heap_[less].index),
                      indexes->at(heap_[index].index));
            SiftDown(less, indexes);
        }
    }

    void SiftUp(vector<size_t>* indexes, size_t index) {
        if (index == 0) {
            return;
        }
        size_t previous = (index - 1) / 2;
        if (heap_[previous].number > heap_[index].number) {
            std::swap(heap_[index], heap_[previous]);
            std::swap(indexes->at(heap_[previous].index),
                      indexes->at(heap_[index].index));
            SiftUp(indexes, previous);
        }
    }

    Value ExtractMin(vector<size_t> *indexes) {
        if (size_ == 0) {
            throw std::invalid_argument("extracting from empty min heap");
        } else {
            Value minimum = heap_[0];
            std::swap(indexes->at(heap_[0].index),
                      indexes->at(heap_[size_ - 1].index));
            heap_[0] = heap_[size_ - 1];
            --size_;
            SiftDown(0, indexes);
            return minimum;
        }
    }

    void Insert(int value, vector<size_t> *indexes, size_t position) {
        size_t index = size_;
        heap_[index] = {value, position};
        indexes->at(position) = index;
        ++size_;

        while (index != 0 && heap_[(index - 1) / 2].number > heap_[index].number) {
            std::swap(heap_[index], heap_[(index - 1) / 2]);
            std::swap(indexes->at(heap_[(index - 1) / 2].index),
                      indexes->at(heap_[index].index));
            index = (index - 1) / 2;
        }
    }

    void Remove(size_t index, vector<size_t> *indexes) {
        if (size_ == 0) {
            throw std::invalid_argument("deleting from empty min heap");
        }
        std::swap(heap_[index], heap_[size_ - 1]);
        std::swap(indexes->at(heap_[size_ - 1].index),
                  indexes->at(heap_[index].index));
        --size_;
        if (size_ == 0) {
            return;
        }
        int previous = static_cast<int>(index - 1) / 2;
        if (heap_[previous].number > heap_[index].number) {
            SiftUp(indexes, index);
        } else {
            SiftDown(index, indexes);
        }
    }

    void PrintHeap() {
        std::cout << "size: " << size_ << "\n";
        for (size_t index = 0; index < size_; ++index) {
            std::cout << heap_[index].number << " ";
        }
        std::cout << "\n";
    }
};

template<class T>
void PrintArray(const T &sequence) {
    for (const auto &element : sequence) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
}

struct InputData {
    vector<unsigned int> sequence;
    vector<char> commands;
    size_t max_heap_size = 0;
};

void ReadInput(InputData *data) {
    // std::ifstream input_file;
    // input_file.open("D:/yandex.school/algorithms/input.txt");
    size_t size;
    size_t command_number;
    std::cin >> size >> command_number >> data->max_heap_size;
    data->sequence.resize(size);
    for (auto &element : data->sequence) {
        std::cin >> element;
    }
    data->commands.resize(command_number);
    for (auto &element : data->commands) {
        std::cin >> element;
    }
}

vector<int> Process(const InputData &data) {
    MinHeap window;
    MaxHeap minimums;
    vector<int> result;
    result.reserve(data.commands.size());
    vector<int> which_heap;
    vector<size_t> indexes;
    indexes.resize(data.sequence.size(), 0);
    which_heap.resize(data.sequence.size(), 0);
    window.Initialize(data.sequence.size());
    minimums.Initialize(data.max_heap_size);
    minimums.Insert(data.sequence[0], &indexes, 0);
    which_heap[0] = 1;
    size_t left = 0;
    size_t right = 0;
    if (data.max_heap_size > 1) {
        for (const auto &command : data.commands) {
            if (command == 'R') {
                ++right;
                int new_element = data.sequence[right];
                if (minimums.size() + 1 < data.max_heap_size) {
                    minimums.Insert(new_element, &indexes, right);
                    which_heap[right] = 1;
                    result.push_back(-1);
                    continue;
                } else if (minimums.size() + 1 == data.max_heap_size) {
                    minimums.Insert(new_element, &indexes, right);
                    which_heap[right] = 1;
                } else if (window.empty() or new_element < window.GetMin()) {
                    minimums.Insert(new_element, &indexes, right);
                    which_heap[right] = 1;
                    Value change = minimums.ExtractMax(&indexes);
                    window.Insert(change.number, &indexes, change.index);
                    which_heap[change.index] = 0;
                } else {
                    window.Insert(new_element, &indexes, right);
                }
            } else {
                if (which_heap[left] == 0) {
                    window.Remove(indexes[left], &indexes);
                } else {
                    minimums.Remove(indexes[left], &indexes);
                    if (window.empty()) {
                        result.push_back(-1);
                        ++left;
                        continue;
                    } else {
                        Value minimum = window.ExtractMin(&indexes);
                        minimums.Insert(minimum.number, &indexes, minimum.index);
                        which_heap[minimum.index] = 1;
                    }
                }
                ++left;
            }
            result.push_back(minimums.GetMax());
        }
    } else {
        window.Insert(data.sequence[0], &indexes, 0);
        for (const auto &command : data.commands) {
            if (command == 'R') {
                ++right;
                int new_element = data.sequence[right];
                window.Insert(new_element, &indexes, right);
            } else {
                window.Remove(indexes[left], &indexes);
                ++left;
            }
            result.push_back(window.GetMin());
        }
    }
    return result;
}

/*
bool CheckAnswer(const vector<int>& first, const vector<int>& second) {
    if (first.size() != second.size()) {
        return false;
    }
    for (size_t i = 0; i < first.size(); ++i) {
        if (first[i] != second[i]) {
            return false;
        }
    }
    return true;
}

vector<int> Check(const InputData &data) {
    int right = 0;
    int left = 0;
    vector<int> result;
    result.reserve(data.commands.size());
    for (const auto &command : data.commands) {
        if (command == 'R') {
            ++right;
        } else {
            ++left;
        }
        vector<int> tmp;
        tmp.reserve(right);
        for (int i = left; i <= right; ++i) {
            tmp.push_back(data.sequence[i]);
        }
        std::sort(tmp.begin(), tmp.end());
        if (tmp.size() >= data.max_heap_size) {
            result.push_back(tmp[data.max_heap_size - 1]);
        } else {
            result.push_back(-1);
        }
    }
    return result;
}

InputData RandomData(int seed = 0) {
    InputData data;
    size_t size = 100;
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> random_seed(1, 100);
    data.sequence.resize(size);
    data.commands.resize(size);
    for (auto& element : data.sequence) {
        element = random_seed(generator);
    }
    data.max_heap_size = 10;
    int sum = 0;
    for (auto& element : data.commands) {
        if ((random_seed(generator) % 10 < 6 or sum == 0) and sum + 2 < data.sequence.size()) {
            element = 'R';
            ++sum;
        } else {
            element = 'L';
            --sum;
        }
    }
    return data;
}

void StressTest(int times = 100000) {
    for (int time = 1; time < times; ++time) {
        InputData data = RandomData(time);
        vector<int> true_result = Check(data);
        vector<int> result = Process(data);
        bool res = CheckAnswer(result, true_result);
        if (res) {
            std::cout << "test passed!" << '\n';
        } else {
            std::cout << "test failed!\t Your answer: \n";
            PrintArray(result);
            std::cout << "true answer: \n";
            PrintArray(true_result);
            std::cout << "data: \n";
            PrintArray(data.sequence);
            std::cout << "commands: \n";
            PrintArray(data.commands);
            std::cout << data.max_heap_size << "\n";
            return;
        }
    }
}
*/

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    InputData data;
    ReadInput(&data);
    auto result = Process(data);
    for (const auto& element : result) {
        std::cout << element << "\n";
    }
    return 0;
}
