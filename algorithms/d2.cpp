#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <vector>

using std::vector;

unsigned int cur = 0;
unsigned int nextRand24(int multiplier, int shift) {  
    cur = cur * multiplier + shift; // вычисляется с переполнениями  
    return cur >> 8; // число от 0 до 2**24-1.  
}

unsigned int nextRand32(int multiplier, int shift) {  
    unsigned int first = nextRand24(multiplier, shift); 
    unsigned int second = nextRand24(multiplier, shift);  
    return (first << 8) ^second; // число от 0 до 2**32-1.  
}

void Input (vector<unsigned int>* data, int time) {
    srand(time);
    size_t settlers = rand() % 10;
    int multiplier = rand() % 1000000000;
    int shift = rand() % 1000000000;

    data->reserve(settlers);

    for (size_t i = 0; i < settlers; ++i) {
        data->push_back(nextRand32(multiplier, shift));
    }
    
    return;
}

template<class T>
void Output (const T& sequence) {
    for (const auto& element : sequence) {
        std::cout << element << ' ';
    }
    std::cout << '\n';
    return;
}

struct Pair{
    vector<unsigned int> less;
    vector<unsigned int> more;
    vector<unsigned int> equal;
};


Pair* split(const vector<unsigned int>& to_split, Pair* pair) {
    size_t size = to_split.size();
    pair->less.reserve(size);
    pair->more.reserve(size);
    pair->equal.reserve(size);
    size_t index = rand() % size;
    double partioner = to_split.at(index);
    for (const auto& point : to_split) {
        if (point < partioner) {
            pair->less.push_back(point);
        } else if (partioner < point) {
            pair->more.push_back(point);
        } else {
            pair->equal.push_back(point);
        }
    }
    return pair;
}

unsigned int get_kth_stats(const vector<unsigned int>& distances, size_t to_cover) {
    if (to_cover > distances.size()) {
        throw std::invalid_argument("received wrong data");
        return 0;
    }
    if (to_cover == 1) {
        return *std::min_element(distances.begin(), distances.end());
    }
    if (distances.size() == 1) {
        return distances.at(0);
    }
    if (distances.size() == to_cover) {
        return *std::max_element(distances.begin(), distances.end());
    }
    Pair splitted;
    split(distances, &splitted);
    if (splitted.less.size() >= to_cover) {
        return get_kth_stats(splitted.less, to_cover);
    } else if (splitted.less.size() + splitted.equal.size() >= to_cover) {
        return splitted.equal[0];
    } else if (
        splitted.more.size() >= to_cover - 
        splitted.less.size() - splitted.equal.size()) {
        return get_kth_stats(splitted.more, 
            to_cover - splitted.less.size() - splitted.equal.size());
    } else {
        std::cout << "SORTING ERROR" << "to_cover: " 
                  << to_cover << "part_size: " << splitted.less.size() << '\n';
        return 0;
    }
}

unsigned int Result(const vector<unsigned int>& data) {
    unsigned int median_index = data.size() / 2;
    return get_kth_stats(data, median_index);
}

int64_t Sum(const vector<unsigned int>& data, const unsigned int coordinate) {
    int64_t sum = 0;
    for (const auto& element : data) {
        if (element > coordinate) {
            sum += element - coordinate;    
        } else {
            sum += coordinate - element;
        }
    }
    return sum;
}

unsigned int k_value(const std::vector<unsigned int>& massiv, size_t k_val) {
    if (k_val == 1) {
        unsigned int minim = massiv[0];
        for (size_t it = 0; it < massiv.size(); ++it) {
            minim = std::min(massiv[it], minim);
        }
        return minim;
    } else if (k_val == massiv.size()) {
        unsigned int maxim = massiv[0];
        for (size_t it = 0; it < massiv.size(); ++it) {
            maxim = std::max(massiv[it], maxim);
        }
        return maxim;
    } else if (massiv.size() == 1) {
        return massiv[0];
    }
    int x_r = rand() % massiv.size();
    std::vector<unsigned int>less_massiv;
    std::vector<unsigned int>equal_massiv;
    std::vector<unsigned int>more_massiv;
    for (size_t it = 0; it < massiv.size(); ++it) {
        if (massiv[it] < massiv[x_r]) {
            less_massiv.push_back(massiv[it]);
        } else if (massiv[it] > massiv[x_r]) {
            more_massiv.push_back(massiv[it]);
        } else {
            equal_massiv.push_back(massiv[it]);
        }
    }
    
    if (less_massiv.size() >= k_val) {
        return k_value(less_massiv, k_val);
    } else if (less_massiv.size() + equal_massiv.size() >= k_val) {
        return equal_massiv[0];
    } else {
        int now_t = less_massiv.size();
        return k_value(more_massiv, k_val - now_t - equal_massiv.size());
    }
}


int64_t answer(size_t not_n, vector<unsigned int> massiv) {
    
    int med_index = not_n / 2 + 1;
    unsigned int med_num = k_value(massiv, med_index);
    int64_t summa = 0;
    for (size_t it = 0; it < not_n; ++it) {
        if (massiv[it] > med_num) {
            int ttr = massiv[it] - med_num;
            summa += ttr;
        } else {
            int ttr = med_num - massiv[it];
            summa += ttr;
        }
    }
    return summa;
}

void StressText(int time = 10) {
    for (int t = 0; t < time; ++t) {
        vector<unsigned int> data;
        Input(&data, time);
        unsigned int result = Result(data);
        int64_t sum = Sum(data, result); 
        auto res = answer(data.size(), data); 
        if (sum != res) {
            std::cout << "Test failed: " << t << "\tright answer: " << sum << "\twrong answer: " << res << '\n';
            Output(data);
        } else {
            std::cout << "test passed" << '\n';
        }
    }
}

int main () {
    StressText();
    return 0;
}
