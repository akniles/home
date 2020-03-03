#include "field.cpp"
#include <ctime>

int main() {
    Field field(10, 10);
    field.create({1, 1}, 5);
    field.create({1, 2}, 5);
    field.move_all();
    return 0;
}
