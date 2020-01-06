#include <random>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <ctime>

using std::vector;

struct HashValue {
    int first_side = 0;
    int second_side = 0;
    int third_side = 0;
    int64_t hash = 0;
};

int64_t GetHashValue(const HashValue &triangle) {
    int64_t first = triangle.second_side;
    int64_t second = triangle.first_side;
    int64_t result = first * 2000000000 / second;
    return result;
}

int64_t SecondHash(const HashValue &triangle) {
    int64_t first = triangle.third_side;
    int64_t second = triangle.first_side;
    int64_t result = first * 2000000000 / second;
    return result;
}

bool Similar(const HashValue one, const HashValue other) {
    return one.first_side * other.second_side == one.second_side * other.first_side and
           one.first_side * other.third_side == one.third_side * other.first_side and
           one.second_side * other.third_side == one.third_side * other.second_side;
}

class Sets {
private:
    int64_t multiplier_ = 0;
    int64_t shift_ = 0;
    size_t modulo_ = 0;
    int64_t prime_modulo_ = 0;
    size_t chunks_used_ = 0;
    int classes_number = 0;
    vector<vector<HashValue> *> hash_map_;


public:
    void Initialize(size_t size = 2) {
        multiplier_ = 35531;
        shift_ = 2447;
        prime_modulo_ = 1004987;
        modulo_ = 2;
        while (modulo_ <= size) {
            modulo_ *= 2;
        }
        hash_map_.resize(modulo_);
    }

    void Realloc() {
        hash_map_.resize(modulo_ * 2);
        vector<vector<HashValue> *> new_hash_map;
        new_hash_map.reserve(modulo_);
        modulo_ *= 2;

        for (const auto &chain : hash_map_) {
            if (chain != nullptr) {
                new_hash_map[index] = chain;
            }
        }

        for (auto &chain : hash_map_) {
            chain = nullptr;
        }

        for (const auto &chain : new_hash_map) {
            size_t index = std::abs((chain->at(0).hash * multiplier_ + shift_)
                                    % prime_modulo_) & (modulo_ - 1);
            hash_map_[index] = chain;
        }
    }

    void Upsert(vector<HashValue> *new_triangle) {
        size_t index = std::abs((new_triangle->at(0).hash * multiplier_ + shift_)
                                % prime_modulo_) & (modulo_ - 1);
        if (hash_map_[index] == nullptr) {
            hash_map_[index] = new_triangle;
            ++classes_number;
            ++chunks_used_;
        } else {
            for (const auto &triangle : *hash_map_[index]) {
                if (Similar(triangle, new_triangle->at(0))) {
                    return;
                }
            }

            hash_map_[index]->push_back(new_triangle->front());
            ++classes_number;

            if (chunks_used_ * 2 == modulo_) {
                Realloc();
            }
        }
    }

    bool Empty() {
        return chunks_used_ == 0;
    }

    int GetClasses() const {
        return classes_number;
    }
};

class FixedSet {
private:
    int64_t multiplier_ = 0;
    int64_t shift_ = 0;
    size_t modulo_ = 0;
    int64_t prime_modulo_ = 0;
    vector<Set> hash_map_;

public:
    void Initialize(int size) {
        multiplier_ = 35531;
        shift_ = 2447;
        prime_modulo_ = 1004987;
        modulo_ = 2 * size + 17;
        hash_map_.resize(modulo_);
    }

    void Upsert(vector<HashValue> *new_triangle) {
        size_t index = std::abs((new_triangle->at(0).hash * multiplier_ + shift_)
                                % prime_modulo_) % modulo_;
        if (hash_map_[index].Empty()) {
            hash_map_[index].Initialize();
        }
        new_triangle->at(0).hash = GetHashValue(new_triangle->at(0));
        hash_map_[index].Upsert(new_triangle);
    }

    int GetClasses() {
        size_t classes_number = 0;
        for (const auto &set : hash_map_) {
            classes_number += set.GetClasses();
        }
        return classes_number;
    }
};

vector<vector<HashValue>> Input() {
    std::ifstream input_file;
    input_file.open("D:/yandex.school/algorithms/input.txt");

    size_t size;
    input_file >> size;
    vector<int> to_sort(3);
    vector<vector<HashValue>> data(size, vector<HashValue>(1));
    for (auto& triangle : data) {
        input_file >> to_sort[0];
        input_file >> to_sort[1];
        input_file >> to_sort[2];
        std::sort(to_sort.begin(), to_sort.end());
        triangle[0].first_side = to_sort[0];
        triangle[0].second_side = to_sort[1];
        triangle[0].third_side = to_sort[2];
    }
    return data;
}

int CountClasses(vector<vector<HashValue>> &data) {
    FixedSet set;
    set.Initialize(10);

    for (auto &triangle : data) {
        triangle[0].hash = SecondHash(triangle[0]);
        set.Upsert(&triangle);
    }

    return set.GetClasses();
}

template<class T, class RandomGenerator>
vector<vector<HashValue>> RandomData(int size, RandomGenerator& generator, T& random_seed) {

    size = random_seed(generator) % 10000 + 1;

    vector<vector<HashValue>> data(size, vector<HashValue>(1));

    vector<int> to_sort(3);
    for (auto &triangle : data) {
        to_sort[0] = random_seed(generator);
        to_sort[1] = random_seed(generator);
        to_sort[2] = random_seed(generator);
        std::sort(to_sort.begin(), to_sort.end());
        triangle[0].first_side = to_sort[0];
        triangle[0].second_side = to_sort[1];
        triangle[0].third_side = to_sort[2];
    }
    return data;
}

template<class T, class RandomGenerator>
void TimeTest(int seed, RandomGenerator &generator, T& random_seed) {
    vector<vector<HashValue>> data = RandomData(seed, generator, random_seed);
    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();
    int classes_number = CountClasses(data);
    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t_end - t_start).count()
              << " ms\n";
    std::cout << classes_number << "\n\n";
    if (classes_number < 1) {
        throw;
    }
}

int main() {
    /*
    vector<vector<HashValue>> data = Input();
    auto result = CountClasses(data);
    std::cout << result << "\n";
    std::mt19937 generator(42 * time(0));
    std::uniform_int_distribution<int> random_seed(1, 1000000000);
    for (int i = 1; i < 1e5; ++i) {
        for (int j = 1; j < 10; ++j) {
            std::cout << "time: " << i << " " << j << "\n";
            TimeTest(j, generator, random_seed);
        }
    }
    */
    return 0;
}
