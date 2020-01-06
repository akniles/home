#include <iostream>
#include <vector>
#include <utility>
#include <fstream>

using std::vector;
using std::pair;

struct Field {
    vector<vector<char>> field;
    int columns_number = 0;
    int rows_number = 0;
};

struct Fathers {
    bool left = false;
    bool up = false;
    bool down = false;
    bool right = false;
};

struct Point {
    pair<int, int> coordinate{0, 0};
    char cannot_turn = '0';
    char came_from = '_';
    int previous = -1;
};

void ReadInput(Field *field, pair<int, int> *start, pair<int, int> *end) {
    // std::ifstream input;
    // input.open("D:/yandex.school/algorithms/input.txt");
    std::cin >> field->columns_number >> field->rows_number;
    field->field.resize(field->columns_number, vector<char>(field->rows_number));
    for (auto &row : field->field) {
        for (auto &symbol : row) {
            std::cin >> symbol;
        }
    }
    std::cin >> start->first >> start->second;
    std::cin >> end->first >> end->second;
    --start->first;
    --start->second;
    --end->first;
    --end->second;
}

char GetWheel(const Point &point, const int type = 0) {
    if (type == 0) {
        if (point.came_from == 'd') {
            return 'l';
        } else if (point.came_from == 't') {
            return 'r';
        } else {
            return point.cannot_turn;
        }
    } else if (type == 1) {
        if (point.came_from == 'l') {
            return 'l';
        } else if (point.came_from == 'r') {
            return 'r';
        } else {
            return point.cannot_turn;
        }
    } else if (type == 2) {
        if (point.came_from == 't') {
            return 'l';
        } else if (point.came_from == 'd') {
            return 'r';
        } else {
            return point.cannot_turn;
        }
    } else {
        if (point.came_from == 'l') {
            return 'r';
        } else if (point.came_from == 'r') {
            return 'l';
        } else {
            return point.cannot_turn;
        }
    }
}

vector<Point> GetNeighbours(const Field &field, const Point &start) {
    vector<Point> result;
    result.reserve(4);
    auto point = start.coordinate;
    if (point.second > 0 and field.field[point.first][point.second - 1] != '#' and
        not(start.came_from == 'd' and start.cannot_turn == 'l') and
        not(start.came_from == 't' and start.cannot_turn == 'r')) {

        auto cannot_turn = GetWheel(start, 0);
        result.push_back({{point.first, point.second - 1},
                          cannot_turn, 'r'});
    }
    if (point.first > 0 and field.field[point.first - 1][point.second] != '#' and
        not(start.came_from == 'l' and start.cannot_turn == 'l') and
        not(start.came_from == 'r' and start.cannot_turn == 'r')) {

        auto cannot_turn = GetWheel(start, 1);
        result.push_back({{point.first - 1, point.second},
                          cannot_turn, 'd'});
    }
    if (point.second + 1 < field.rows_number and
        field.field[point.first][point.second + 1] != '#' and
        not(start.came_from == 'd' and start.cannot_turn == 'r') and
        not(start.came_from == 't' and start.cannot_turn == 'l')) {

        auto cannot_turn = GetWheel(start, 2);
        result.push_back({{point.first, point.second + 1},
                          cannot_turn, 'l'});
    }
    if (point.first + 1 < field.columns_number and
        field.field[point.first + 1][point.second] != '#' and
        not(start.came_from == 'l' and start.cannot_turn == 'r') and
        not(start.came_from == 'r' and start.cannot_turn == 'l')) {

        auto cannot_turn = GetWheel(start, 3);
        result.push_back({{point.first + 1, point.second},
                          cannot_turn, 't'});
    }
    return result;
}

void PrintCoordinate(const pair<int, int> &one) {
    std::cout << one.first + 1 << " " << one.second + 1 << "\n";
}

void PrintResult(const vector<Point> &stack, const int distance, const pair<int, int> &start,
                 const pair<int, int> &end, const int dark) {
    std::cout << distance + 2 << "\n";
    if (distance == -1) {
        PrintCoordinate(start);
        PrintCoordinate(end);
        return;
    }
    vector<pair<int, int>> result;
    result.reserve(distance);
    result.push_back(end);
    auto previous = dark;
    while (stack[previous].previous != -1) {
        result.push_back(stack[previous].coordinate);
        previous = stack[previous].previous;
    }
    result.push_back(stack[previous].coordinate);
    result.push_back(start);
    while (not result.empty()) {
        auto coordinate = result.back();
        result.pop_back();
        PrintCoordinate(coordinate);
    }
}

void Draw(vector<vector<Fathers>> *colors, const Point &point) {
    if (point.came_from == 't') {
        colors->at(point.coordinate.first)[point.coordinate.second].up = true;
    } else if (point.came_from == 'r') {
        colors->at(point.coordinate.first)[point.coordinate.second].right = true;
    } else if (point.came_from == 'd') {
        colors->at(point.coordinate.first)[point.coordinate.second].down = true;
    } else if (point.came_from == 'l') {
        colors->at(point.coordinate.first)[point.coordinate.second].left = true;
    }
}

vector<Point> MakeStack(const Field &field, const pair<int, int> &point) {
    vector<Point> result;
    result.reserve(8);
    if (point.second > 0 and field.field[point.first][point.second - 1] != '#') {
        result.push_back({{point.first, point.second - 1},
                          'r', 'r', -1});
        result.push_back({{point.first, point.second - 1},
                          'l', 'r', -1});
    }
    if (point.first > 0 and field.field[point.first - 1][point.second] != '#') {
        result.push_back({{point.first - 1, point.second},
                          'r', 'd', -1});
        result.push_back({{point.first - 1, point.second},
                          'l', 'd', -1});
    }
    if (point.second + 1 < field.rows_number and
        field.field[point.first][point.second + 1] != '#') {
        result.push_back({{point.first, point.second + 1},
                          'r', 'l', -1});
        result.push_back({{point.first, point.second + 1},
                          'l', 'l', -1});
    }
    if (point.first + 1 < field.columns_number and
        field.field[point.first + 1][point.second] != '#') {
        result.push_back({{point.first + 1, point.second},
                          'r', 't', -1});
        result.push_back({{point.first + 1, point.second},
                          'l', 't', -1});
    }
    return result;
}

void Step(vector<vector<Fathers>> *colors, vector<Point> *stack,
          Point *neighbour, size_t *color, const int index) {
    if (not(colors->at(neighbour->coordinate.first)[neighbour->coordinate.second].up and
            neighbour->came_from == 't') and
        not(colors->at(neighbour->coordinate.first)[neighbour->coordinate.second].down and
            neighbour->came_from == 'd') and
        not(colors->at(neighbour->coordinate.first)[neighbour->coordinate.second].right
            and neighbour->came_from == 'r') and
        not(colors->at(neighbour->coordinate.first)[neighbour->coordinate.second].left and
            neighbour->came_from == 'l')) {
        Draw(colors, *neighbour);
        neighbour->previous = index;
        stack->push_back(*neighbour);
        ++*color;
    }
}

void Process(const Field &field, const pair<int, int> &start, const pair<int, int> &end) {
    if (start == end) {
        std::cout << "0\n";
        PrintCoordinate(start);
        return;
    }
    vector<vector<Fathers>> lefts(field.columns_number, vector<Fathers>(field.rows_number));
    vector<vector<Fathers>> rights(field.columns_number, vector<Fathers>(field.rows_number));
    lefts[start.first][start.second].up = true;
    lefts[start.first][start.second].down = true;
    lefts[start.first][start.second].right = true;
    lefts[start.first][start.second].left = true;
    rights[start.first][start.second].up = true;
    rights[start.first][start.second].down = true;
    rights[start.first][start.second].right = true;
    rights[start.first][start.second].left = true;
    auto stack = MakeStack(field, start);
    int index = 0;
    for (auto neighbour : stack) {
        if (neighbour.coordinate == end) {
            PrintResult(stack, -1, start, end, index);
            return;
        }
        ++index;
    }
    stack.reserve(field.rows_number * field.columns_number);
    int distance = 0;
    size_t dark = 0;
    size_t gray = stack.size();
    size_t white = stack.size();
    while (dark < white and dark < stack.size()) {
        auto neighbours = GetNeighbours(field, stack[dark]);
        for (auto neighbour : neighbours) {
            if (neighbour.coordinate == end) {
                PrintResult(stack, distance, start, end, dark);
                return;
            } else if (neighbour.cannot_turn == 'r') {
                Step(&rights, &stack, &neighbour, &white, dark);
            } else if (neighbour.cannot_turn == 'l') {
                Step(&lefts, &stack, &neighbour, &white, dark);
            }
        }
        ++dark;
        if (dark == gray) {
            ++distance;
            gray = white;
        }
    }
    std::cout << "-1\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Field field;
    pair<int, int> start;
    pair<int, int> end;
    ReadInput(&field, &start, &end);
    Process(field, start, end);
    return 0;
}
