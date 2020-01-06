#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <fstream>
#include "set.cpp"

using std::pair;
using std::vector;

struct Rectangle {
    pair<int, int> start{0, 0};
    pair<int, int> end{0, 0};
    size_t index = 0;
};

struct Segment {
    int value = 0;
    int start = 0;
    int end = 0;
    size_t index = 0;
    int high_start = 0;
    bool segment_type = true;
};

bool Contains(const Rectangle &big, const Rectangle &small) {
    auto result = big.start.first < small.start.first and big.start.second < small.start.second and
                  big.end.first > small.end.first and big.end.second > small.end.second;
    return result;
}

void ReadInput(vector<Rectangle> *data) {
    // std::ifstream input_file;
    // input_file.open("D:/yandex.school/algorithms/input.txt");

    size_t size;
    std::cin >> size;
    // input_file >> size;
    data->resize(size);
    int first;
    int second;
    int third;
    int fourth;
    size_t index = 0;
    for (auto &rectangle : *data) {
        std::cin >> first >> second >> third >> fourth;
        rectangle = {{first, second}, {third, fourth}, index};
        ++index;
    }
}

void CountExternal(const vector<Rectangle> &data) {
    struct SegmentLess {
        inline bool operator()(const Segment &one, const Segment &other) {
            return (one.value < other.value or (one.value == other.value and one.high_start < other.high_start));
        }
    };
    vector<Segment> segments;
    segments.reserve(data.size() * 2);
    size_t index = 0;
    for (const auto &rectangle : data) {
        segments.push_back(
                {rectangle.start.first, rectangle.start.first, rectangle.end.first, index, rectangle.start.second, true});
        segments.push_back(
                {rectangle.end.first, rectangle.start.first, rectangle.end.first, index, rectangle.start.second, false});
        ++index;
    }
    std::sort(segments.begin(), segments.end(), SegmentLess());
    Tree<Segment, Comp<Segment>> tree(segments.size());
    size_t counter = 0;
    for (const auto &segment : segments) {
        auto start = data[segment.index].start.second;
        auto end = data[segment.index].end.second;
        Segment plain_segment{start, start, end, segment.index};
        if (segment.segment_type) {
            if (tree.empty()) {
                tree.Insert(plain_segment);
            } else {
                auto big = data[tree.FindMaximumLess(plain_segment).index];
                if (not Contains(big, data[plain_segment.index])) {
                    tree.Insert(plain_segment);
                }
            }
        } else {
            if (tree.Remove(plain_segment)) {
                ++counter;
            }
        }
    }
    std::cout << counter << "\n";
}

int main() {
    vector<Rectangle> data;
    ReadInput(&data);
    CountExternal(data);
    return 0;
}
