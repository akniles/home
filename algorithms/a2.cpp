#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <vector>

using std::vector;

struct InputData {
    vector<int> sequence;
    vector<char> commands;
};

void Input (InputData* data) {
    size_t size;
    std::cin >> size;
    data->sequence.resize(size);
    for (auto& element : data->sequence) {
        std::cin >> element;
    }
    std::cin >> size;
    data->commands.resize(size);
    for (auto& element : data->commands) {
        std::cin >> element;
    }
    return;
}

template<class T>
void Output (const T& sequence) {
    for (const auto& element : sequence) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
    return;
}

void Transform(
    vector<int>* deque, 
    const vector<int>& sequence, 
    size_t left, 
    size_t right
    ) {
    
    for (size_t i = right; i >= left; --i) {
        if (deque->size() == 0) {
            deque->push_back(sequence[i]);
        } else {
            deque->push_back(std::max(sequence[i], deque->back()));
        }
    }

    return;
}

void Process (const InputData& data, vector<int>* result) {
    const size_t commands_size = data.commands.size();
    const int minimum = - 2000000000;
    vector<int> deque;
    deque.reserve(data.commands.size());
    size_t left = 0;
    size_t right = 0;
    int local_maximum = data.sequence[0];
    for (size_t index = 0; index < commands_size; ++index) {
        // Output(deque);
        if (data.commands[index] == 'R') {
            right += 1;
            local_maximum = std::max(local_maximum, data.sequence[right]);
            if (deque.size() == 0) {
                result->push_back(local_maximum);
            } else {
                result->push_back(std::max(local_maximum, deque.back()));
            }
        } else if (deque.size() != 0 and data.commands[index] == 'L') {
            left += 1;
            deque.pop_back();
            if (deque.size() == 0) {
                Transform(&deque, data.sequence, left, right);
                local_maximum = minimum;
            }
            result->push_back(std::max(deque.back(), local_maximum));
        } else if (deque.size() == 0 and data.commands[index] == 'L') {
            left += 1;
            Transform(&deque, data.sequence, left, right);
            result->push_back(deque.back());
            local_maximum = minimum;
        }
    }
    return;
}

int main () {
    InputData data;
    Input(&data);
    vector<int> result;
    Process(data, &result);
    Output(result);
    return 0;
}