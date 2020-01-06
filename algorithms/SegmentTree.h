//
// Created by Admin on 21.11.2019.
//

#ifndef ALGORITHMS_SEGMENTTREE_H
#define ALGORITHMS_SEGMENTTREE_H

#include <vector>

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


#endif //ALGORITHMS_SEGMENTTREE_H
