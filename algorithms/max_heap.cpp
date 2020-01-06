//
// Created by Admin on 28.10.2019.
//
#include <iostream>
#include <vector>
#include <tools.cpp>

using std::vector;

template<class KeyType>
class MaxHeap {
private:
    vector<KeyType> heap_;
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
        if (left < size_ and (heap_[left].number > heap_[index].number or
                              (heap_[left].number == heap_[index].number
                               and heap_[left].start < heap_[index].start))) {
            more = left;
        }
        if (right < size_ and (heap_[right].number > heap_[more].number or
                               (heap_[right].number == heap_[more].number and
                                heap_[right].start < heap_[more].start))) {
            more = right;
        }
        if (more != index) {
            std::swap(heap_[index], heap_[more]);
            std::swap(indexes->at(heap_[more].index),
                      indexes->at(heap_[index].index));
            SiftDown(more, indexes);
        }
    }

    KeyType ExtractMax(vector<size_t> *indexes) {
        if (size_ == 0) {
            return {0, 0, 0, 0};
        } else {
            KeyType maximum = heap_[0];
            std::swap(indexes->at(heap_[size_ - 1].index),
                      indexes->at(heap_[0].index));
            heap_[0] = heap_[size_ - 1];
            --size_;
            SiftDown(0, indexes);
            return maximum;
        }
    }

    void Insert(KeyType value, vector<size_t> *indexes, size_t position) {
        size_t index = size_;
        heap_[index] = value;
        indexes->at(position) = index;
        ++size_;

        while (index != 0 and (heap_[(index - 1) / 2].number < heap_[index].number or
                               (heap_[(index - 1) / 2].number == heap_[index].number and
                                heap_[(index - 1) / 2].start > heap_[index].start))) {
            std::swap(heap_[index], heap_[(index - 1) / 2]);
            std::swap(indexes->at(heap_[(index - 1) / 2].index),
                      indexes->at(heap_[index].index));
            index = (index - 1) / 2;
        }
    }

    void SiftUp(size_t index, vector<size_t> *indexes) {
        if (index == 0) {
            return;
        }
        size_t previous = (index - 1) / 2;
        if (heap_[previous].number < heap_[index].number or
            (heap_[previous].number == heap_[index].number and
             heap_[previous].start > heap_[index].start)) {
            std::swap(heap_[index], heap_[previous]);
            std::swap(indexes->at(heap_[previous].index),
                      indexes->at(heap_[index].index));
            SiftUp(previous, indexes);
        }
    }

    void ChangeKey(size_t index, KeyType new_value, vector<size_t> *indexes) {
        heap_[index] = new_value;
        int previous = static_cast<int>(index - 1) / 2;
        if (heap_[previous].number < heap_[index].number or
            (heap_[previous].number == heap_[index].number and
             heap_[previous].start > heap_[index].start)) {
            SiftUp(index, indexes);
        } else {
            SiftDown(index, indexes);
        }
    }

    void Remove(size_t index, vector<size_t> *indexes) {
        std::swap(heap_[index], heap_[size_ - 1]);
        std::swap(indexes->at(heap_[size_ - 1].index),
                  indexes->at(heap_[index].index));
        --size_;
        if (size_ == 0) {
            return;
        }

        int previous = static_cast<int>(index - 1) / 2;
        if (heap_[previous].number < heap_[index].number or
            (heap_[previous].number == heap_[index].number and
             heap_[previous].start > heap_[index].start)) {
            SiftUp(index, indexes);
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
