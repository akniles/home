//
// Created by Admin on 28.10.2019.
//
#include <iostream>
#include <vector>
#include <tools.cpp>

using std::vector;

class MinHeap {
private:
    vector<HeapValue> heap_;
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

    void SiftDown(int index, vector<size_t> *indexes) {
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

    HeapValue ExtractMin(vector<size_t> *indexes) {
        if (size_ == 0) {
            throw std::invalid_argument("extracting from empty min heap");
        } else {
            HeapValue minimum = heap_[0];
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


