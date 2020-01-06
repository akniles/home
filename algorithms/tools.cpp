#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <ctime>

#ifndef tools
#define tools

using std::pair;
using std::vector;

template<class T>
void PrintArray(const T &sequence) {
    for (const auto &element : sequence) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
}

template<class Data, class Answer>
void StressTest(Data RandomData(const int seed),
                void Process(const Data &, Answer *), void Result(const Data &, Answer *),
                bool Check(const Answer &, const Answer &), void PrintAnswer(const Answer &),
                void PrintData(const Data &), const int iterations) {
    for (int index = 1; index < iterations; ++index) {
        Data data = RandomData(index);
        Answer your_answer;
        Answer true_answer;
        Process(data, &your_answer);
        Result(data, &true_answer);
        if (Check(your_answer, true_answer)) {
            std::cout << "test " << index << " is passed!\n";
        } else {
            std::cout << "test " << index << " is failed!\nYour answer:\n";
            PrintAnswer(your_answer);
            std::cout << "\nTrue answer:\n";
            PrintAnswer(true_answer);
            std::cout << "\nData:\n";
            PrintData(data);
            return;
        }
    }
}

/*
struct CustomLess {
    inline bool operator()(const Edge &one, const Edge &other) {
        return (one.value < other.value or (one.value == other.value and one.start > other.start));
    }
};*/
#endif