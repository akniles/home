#include "SegmentTree.h"

template<class Segment>
bool Contains(const Segment& one, const Segment& other) {
    return one.start <= other.start and one.end >= other.end; // other contained by one
}

template<class Segment>
bool Disjoint(const Segment& one, const Segment& other) {
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
void SegmentTree<Segment>::Remove(Segment segment, size_t index){
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
size_t SegmentTree<Segment>::GetLength(size_t index){
    if (index >= size_) {
        return 0;
    }
    if (tree_[index].times_covered > 0) {
        return tree_[index].value;
    } else {
        return GetLength(2 * index + 1) + GetLength(2 * index + 2);
    }
}
