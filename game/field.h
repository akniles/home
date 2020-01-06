//
// Created by Admin on 06.01.2020.
//

#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <utility>
#include <vector>
#include <random>
#include "square.cpp"

using std::pair;
using std::vector;

class Field {
public:
    Field(int rows, int columns);

    void create(pair<int, int> coordinates, int value);

    void move_all();

private:
    size_t rows_;
    size_t columns_;
    vector<vector<Square>> field_;
    vector<Square> line_;
    std::mt19937 generator_;
    std::uniform_int_distribution<int> seed_;
};


#endif //GAME_FIELD_H
