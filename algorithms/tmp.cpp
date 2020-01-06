#include "set.h"
#include <string>

using std::string;

int main() {
    Set<int> set{1, 4, 3, 6, 2};
    for (auto iter = set.begin(); iter != set.end(); ++iter) {
        std::cout << *iter << "\n";
    }
    return 0;
}