#include "dsu.h"

DSU::DSU(const size_t size) {
    size_ = size;
    sets_.resize(size_);
    size_t index = 0;
    for (auto& element : sets_) {
        element.key = index;
        element.parent = index;
        element.height = 1;
        element.last_alive_parent = index;
        ++index;
    }
}

size_t DSU::Find(const size_t index) {
    if (not sets_[index].alive) {
        return DSU::Find(sets_[index].parent);
    }
    if (sets_[sets_[index].parent].key == index) {
        return index;
    }
    sets_[sets_[index].parent].key = Find(sets_[sets_[index].parent].key);
    return sets_[sets_[index].parent].key;
}

void DSU::Join(const size_t &one, const size_t &other) {
    sets_[other].parent = one;
    sets_[other].last_alive_parent = one;
}

bool DSU::Check(const size_t one, const size_t other) {
    return Find(other) == other and Find(one) != other;
}

void DSU::Kill(const size_t &one) {
    sets_[one].alive = false;
}

size_t DSU::Judge(const size_t one, const size_t other) {
    if (one == other) {
        return one;
    }
    auto first = one;
    auto second = other;
    size_t first_kostyan = 0;
    size_t second_kostyan = 0;
    while (first != second or (not sets_[first].alive and not sets_[second].alive)) {
        if (first < second) {
            if (not sets_[second].alive and second_kostyan == 0) {
                second_kostyan = second;
            }
            if (sets_[second].alive and second_kostyan != 0) {
                sets_[second_kostyan].last_alive_parent = second;
                second_kostyan = 0;
            }
            second = sets_[second].last_alive_parent;
        } else {
            if (not sets_[first].alive and first_kostyan == 0) {
                first_kostyan = first;
            }
            if (sets_[first].alive and first_kostyan != 0) {
                sets_[first_kostyan].last_alive_parent = first;
                first_kostyan = 0;
            }
            first = sets_[first].last_alive_parent;
        }
    }
    return first;
}

void DSU::Birth(const size_t father) {
    Node child{father, size_, true, sets_[father].height + 1, father};
    sets_.push_back(child);
    ++size_;
}
