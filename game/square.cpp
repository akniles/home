//
// Created by Admin on 06.01.2020.
//

#include "square.h"

Square::Square(pair<int, int> coordinates) {
    coordinates_ = coordinates;
    value_ = 1;
    status_ = true;
}

void Square::move(pair<int, int> coordinates) {
    coordinates_ = coordinates;
}

void Square::value(int64_t value) {
    value_ = value;
}

pair<int, int> Square::move(int side) {
    if (side == 1) {
        ++coordinates_.first;
    } else if (side == 2) {
        ++coordinates_.second;
    } else if (side == 3) {
        --coordinates_.first;
    } else {
        --coordinates_.second;
    }
    return coordinates_;
}

pair<int, int> Square::get_coordinates() {
    return coordinates_;
}

int64_t Square::get_value() {
    return value_;
}

Square::Square() {
    value_ = 0;
    coordinates_ = {0, 0};
    status_ = false;
}
