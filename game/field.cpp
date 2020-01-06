//
// Created by Admin on 06.01.2020.
//

#include "field.h"
#include <ctime>

void Field::create(pair<int, int> coordinates, int value) {
    Square square(coordinates);
    square.value(value);
    line_.push_back(square);
    field_[coordinates.first][coordinates.second] = square;
}

Field::Field(int rows, int columns) {
    rows_ = rows;
    columns_ = columns;
    field_.resize(columns, vector<Square>(rows));
    generator_.seed(time(nullptr));
    std::uniform_int_distribution<int> seed(1, 4);
    seed_.param(seed.param());
}

void Field::move_all() {
    for (auto &line : field_) {
        for (auto &square : line) {
            if (square.get_value() == 0) {
                continue;
            }
            auto side = seed_(generator_);
            auto coordinates = square.get_coordinates();
            if (side == 1) {
                if (field_[coordinates.first + 1][coordinates.second].get_value() == 0) {
                    coordinates = square.move(side);
                    field_[coordinates.first][coordinates.second].value(square.get_value());
                } else if (field_[coordinates.first + 1][coordinates.second].get_value() ==
                           square.get_value()) {
                    field_[coordinates.first][coordinates.second].value(0);
                    field_[coordinates.first + 1][coordinates.second].value(
                            field_[coordinates.first + 1][coordinates.second].get_value() * 2);
                }
            } else if (side == 2) {
                if (field_[coordinates.first][coordinates.second + 1].get_value() == 0) {
                    coordinates = square.move(side);
                    field_[coordinates.first][coordinates.second].value(square.get_value());
                } else if (field_[coordinates.first][coordinates.second + 1].get_value() ==
                           square.get_value()) {
                    field_[coordinates.first][coordinates.second].value(0);
                    field_[coordinates.first][coordinates.second + 1].value(
                            field_[coordinates.first][coordinates.second + 1].get_value() * 2);
                }
            } else if (side == 3) {
                if (field_[coordinates.first - 1][coordinates.second].get_value() == 0) {
                    coordinates = square.move(side);
                    field_[coordinates.first][coordinates.second].value(square.get_value());
                } else if (field_[coordinates.first - 1][coordinates.second].get_value() ==
                           square.get_value()) {
                    field_[coordinates.first][coordinates.second].value(0);
                    field_[coordinates.first - 1][coordinates.second].value(
                            field_[coordinates.first - 1][coordinates.second].get_value() * 2);
                }
            } else {
                if (field_[coordinates.first][coordinates.second - 1].get_value() == 0) {
                    coordinates = square.move(side);
                    field_[coordinates.first][coordinates.second].value(square.get_value());
                } else if (field_[coordinates.first][coordinates.second - 1].get_value() ==
                           square.get_value()) {
                    field_[coordinates.first][coordinates.second].value(0);
                    field_[coordinates.first][coordinates.second - 1].value(
                            field_[coordinates.first][coordinates.second - 1].get_value() * 2);
                }
            }
        }
    }
}
