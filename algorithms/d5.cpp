#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <fstream>

using std::pair;
using std::vector;

template<class Segment>
class SegmentTree {
private:
    vector<Segment> tree_;
    size_t size_;

    Segment RecursiveConstruction(size_t index) {
        if (2 * index + 2 > size_) {
            return tree_[index];
        } else if (2 * index + 2 == size_) {
            tree_[index] = tree_[2 * index + 1];
            return tree_[2 * index + 1];
        }
        auto left = RecursiveConstruction(2 * index + 1);
        auto right = RecursiveConstruction(2 * index + 2);
        if (left.value == 0) {
            tree_[index] = right;
        } else if (right.value == 0) {
            tree_[index] = left;
        } else {
            tree_[index] = {right.end - left.start, left.start, right.end, false};
        }
        return tree_[index];
    }

public:
    explicit SegmentTree(const vector<Segment> &segments);

    void Insert(Segment segment, size_t index = 0);

    void Remove(Segment segment, size_t index = 0);

    size_t GetLength(size_t index = 0);
};


template<class Segment>
bool Contains(const Segment &one, const Segment &other) {
    return one.start <= other.start and one.end >= other.end; // other contained by one
}

template<class Segment>
bool Disjoint(const Segment &one, const Segment &other) {
    return one.start >= other.end or one.end <= other.start;
}

template<class Segment>
SegmentTree<Segment>::SegmentTree(const vector<Segment> &segments) {
    size_ = 2;
    while (size_ <= segments.size()) {
        size_ *= 2;
    }
    int start = size_ - 1;
    size_ = size_ * 2 - 1;
    tree_.resize(size_);
    for (size_t index = 0; index < segments.size(); ++index) {
        tree_[index + start] = segments[index];
    }
    RecursiveConstruction(0);
}

template<class Segment>
void SegmentTree<Segment>::Insert(const Segment segment, size_t index) {
    if (index > size_ or Disjoint(tree_[index], segment)) {
        return;
    }
    if (Contains(segment, tree_[index])) {
        ++tree_[index].times_covered;
        return;
    } else {
        Insert(segment, 2 * index + 1);
        Insert(segment, 2 * index + 2);
        return;
    }
}

template<class Segment>
void SegmentTree<Segment>::Remove(Segment segment, size_t index) {
    if (index > size_ or Disjoint(tree_[index], segment)) {
        return;
    }
    if (Contains(segment, tree_[index])) {
        --tree_[index].times_covered;
        return;
    } else {
        Remove(segment, 2 * index + 1);
        Remove(segment, 2 * index + 2);
        return;
    }
}

template<class Segment>
size_t SegmentTree<Segment>::GetLength(size_t index) {
    if (index >= size_) {
        return 0;
    }
    if (tree_[index].times_covered > 0) {
        return tree_[index].value;
    } else {
        return GetLength(2 * index + 1) + GetLength(2 * index + 2);
    }
}

struct Segment {
    int value = 0;
    int start = 0;
    int end = 0;
    int times_covered = 0;

    inline bool operator<(const Segment &one) const {
        return value < one.value;
    }

    inline bool operator==(const Segment &one) const {
        return start == one.start and end == one.end;
    }
};

struct Query {
    char operation = '+';
    int start = 0;
    int end = 0;
};

struct InputData {
    vector<Query> queries;
    vector<Segment> segments;
};

void ReadInput(InputData *data) {
    // std::ifstream input_file;
    // input_file.open("D:/yandex.school/algorithms/input.txt");

    size_t size;
    std::cin >> size;
    data->queries.resize(size);
    data->segments.reserve(size);
    for (auto &query : data->queries) {
        std::cin >> query.operation >> query.start >> query.end;
    }

    for (const auto &query : data->queries) {
        if (query.operation == '+') {
            Segment segment{query.end - query.start, query.start, query.end};
            data->segments.push_back(segment);
        }
    }
}

vector<Segment> SortPoints(const vector<Segment> &segments) {
    vector<int> coordinates;
    vector<Segment> points;
    points.reserve(2 * segments.size());
    coordinates.reserve(segments.size() * 2);
    for (const auto &segment : segments) {
        coordinates.push_back(segment.start);
        coordinates.push_back(segment.end);
    }
    std::sort(coordinates.begin(), coordinates.end());
    vector<int> without_duplicates;
    without_duplicates.reserve(segments.size() * 2);
    auto last = coordinates.front();
    without_duplicates.push_back(coordinates.front());
    for (const auto &element : coordinates) {
        if (element != last) {
            without_duplicates.push_back(element);
            last = element;
        }
    }
    for (size_t index = 0; index + 1 < without_duplicates.size(); ++index) {
        Segment segment{without_duplicates[index + 1] - without_duplicates[index],
                        without_duplicates[index],
                        without_duplicates[index + 1], 0};
        points.push_back(segment);
    }
    return points;
}

void CountArea(const InputData &data) {
    auto points = SortPoints(data.segments);
    SegmentTree<Segment> tree(points);
    for (const auto &command : data.queries) {
        if (command.operation == '+') {
            tree.Insert({command.end - command.start, command.start, command.end});
        } else {
            tree.Remove({command.end - command.start, command.start, command.end});
        }
        std::cout << tree.GetLength() << "\n";
    }
}

int main() {
    InputData data;
    ReadInput(&data);
    CountArea(data);
    return 0;
}
