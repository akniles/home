#include <iostream>
#include <algorithm>
#include <string>
#include <cassert>
#include <vector>

using std::string;
using std::vector;

string Input() {
    string sequence;
    std::cin >> sequence;
    return sequence;
};

char mapping(char bracket) {
    if (bracket    == '}') {
        return    '{';
    } else if (bracket    == ')') {
        return    '(';
    } else {
        return '[';
    }
}

void Process(const string& sequence) {
    int length_of_correct_part = 0;
    vector<char> opened;
    opened.reserve(1000);
    for (const char& bracket : sequence) {
        if (bracket == '(' or bracket == '{' or bracket == '[') {
            opened.push_back(bracket);
            length_of_correct_part += 1;
        } else if (opened.size() > 0 and mapping(bracket) == opened.back()) {
            auto iter = opened.begin() + opened.size() - 1;
            opened.erase(iter);
            length_of_correct_part += 1;
        } else {
            std::cout << length_of_correct_part << '\n';
            return;
        }
    }
    if (opened.size() == 0) {
        std::cout << "CORRECT" << '\n';
    } else {
        std::cout << length_of_correct_part << '\n';
    }
}

int main() {
    const string sequence = Input();
    Process(sequence);
    return 0;
}
