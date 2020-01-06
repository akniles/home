//
// Created by Admin on 06.01.2020.
//

#ifndef GAME_SQUARE_H
#define GAME_SQUARE_H

#include <utility>
#include <iostream>

using std::pair;

class Square {
public:
    explicit Square(pair<int, int> coordinates);

    Square();

    void move(pair<int, int> coordinates);

    pair<int, int> move(int side);

    void value(int64_t value);

    pair<int, int> get_coordinates();

    int64_t get_value();

private:
    pair<int, int> coordinates_;
    bool status_;
    int64_t value_;
};


#endif //GAME_SQUARE_H
