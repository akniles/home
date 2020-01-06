#include <iostream>
#include <algorithm>
#include <string>
#include <math.h>
#include <vector>
#include <ctime>
#include <iomanip>
#include <chrono>
// include <cstdlib>

using std::string;
using std::vector;

struct Point {
    double x_part, y_part;
};

struct Data {
    vector<Point> points;
    size_t to_cover;
    size_t size;
};

struct Circle {
    double radius;
    double center;
    vector<Point> points_covered;
};

void PrintSequence(const vector<int>& sequence) {
    for (const auto& element : sequence) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
}

vector<int> Input() {
    vector<int> sequence;
    size_t sequence_length;
    std::cin >> sequence_length;
    sequence.reserve(sequence_length);
    int previous = 0;
    for (size_t row = 0; row < sequence_length; ++row) {
        int current;
        std::cin >> current;
        if (sequence.size() == 0 or current != previous) {
            sequence.push_back(current);
        }
        previous = current;
    }
    return sequence;
};

vector<int> FindExtremums(const vector<int>& sequence) {
    vector<int> extremums;
    extremums.reserve(sequence.size());
    int status;
    int previous = sequence[0];
    if (sequence[0] < sequence[1]) {
        status = 1;
    } else {
        status = 0;
    }
    extremums.push_back(0);
    for (size_t index = 1; index < sequence.size(); ++index) {
        int element = sequence[index];
        
        if (status == 1 and previous < element) {
            extremums.push_back(index);
            status = (status + 1) % 2;
        } else if (status == 0 and element < previous) {
            extremums.push_back(index);
            status = (status + 1) % 2;
        } else {
            extremums[extremums.size() - 1] = index;
        }
        previous = element;
    }
    return extremums;
}

void ChangeMaximum (
        vector<int>* subsequence, 
        const vector<int>& sequence, 
        size_t first, 
        size_t second, 
        size_t third, 
        size_t index) {
    if (sequence[second] < sequence[first]) {
        for (size_t i = first; i < second; ++i) {
            if (sequence[i] < sequence[third] and sequence[i] < subsequence->at(index)) {
                subsequence->at(index + 1) = sequence[i];
                return;
            }
        }
    } else {
        for (size_t i = first; i < second; ++i) {
            if (sequence[i] > sequence[third] and sequence[i] > subsequence->at(index)) {
                subsequence->at(index + 1) = sequence[i];
                return;
            }
        }
    }
    return;
}

void UpdateLastElement(
        const vector<int>& extremums, 
        const vector<int>& sequence, 
        vector<int>* subsequence) {
    size_t size = extremums.size();
    if (sequence[extremums[size - 1]] > sequence[extremums[size - 2]]) {
        for (size_t i = extremums[size - 2]; i < sequence.size(); ++i) {
            if (sequence[i] > subsequence->at(size - 2)) {
                subsequence->at(size - 1) = sequence[i];
                return;
            }
        }
    } else {
        for (size_t i = extremums[size - 2]; i < sequence.size(); ++i) {
            if (sequence[i] < subsequence->at(size - 2)) {
                subsequence->at(size - 1) = sequence[i];
                return;
            }
        }
    }
    return;
}

vector<int> ChangeLimits(const vector<int>& extremums, const vector<int>& sequence) {
    vector<int> subsequence;
    subsequence.reserve(extremums.size());

    for (const auto& index : extremums) {
        subsequence.push_back(sequence[index]);
    }
    
    for (size_t i = 0; i + 2 < extremums.size(); ++i) {
        ChangeMaximum(
            &subsequence, 
            sequence, 
            extremums[i], 
            extremums[i + 1], 
            extremums[i + 2],
            i);
    }
    if (extremums.size() > 1) {
        UpdateLastElement(extremums, sequence, &subsequence);
    }
    return subsequence;
}

vector<int> FindSubequence(const vector<int>& sequence) {
    if (sequence.size() == 1) {
        return sequence;
    }
    const vector<int> extremums = FindExtremums(sequence);
    return ChangeLimits(extremums, sequence);
    // return extremums;
}

int main() {
    const vector<int> sequence = Input();
    const vector<int> subsequence = FindSubequence(sequence);
    PrintSequence(subsequence);
    return 0;
}
