#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>

using std::vector;

class FixedSet {
private:
    int multiplier;
    int shift;
    int modulo;
    vector<int> hash_map;

public:
    FixedSet();

    void Initialize(const vector<int> &numbers) {
        size_t size = numbers.size();
        multiplier = 1007;
        shift = 1234;
        modulo = 10 * size + 17;
        hash_map.resize(modulo);

        for (const auto &number : numbers) {
            size_t index = (number * multiplier + shift) % modulo;
            while (hash_map[index] != 0) {
                index = (index + 1) % modulo;
            }
            hash_map[index] = number;
        }
    }

    bool Contains(int number) {
        size_t index = (number * multiplier + shift) % modulo;
        while (hash_map[(index + 1) % modulo] != 0) {
            if (hash_map[index] == number) {
                return 1;
            }
            index += 1;
        }
        return 0;
    }
};