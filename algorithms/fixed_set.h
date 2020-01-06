#include <vector>
#include <tools.cpp>

#ifndef ALGORITHMS_FIXED_SET_H
#define ALGORITHMS_FIXED_SET_H

using std::vector;

template<class KeyType, class Comp>
class FixedSet {
private:
    int modulo_;
    vector<vector<KeyType>> hash_map_;
    Comp hash;

public:
    explicit FixedSet(const size_t size = 1000) {
        modulo_ = std::max(2 * size + 17, static_cast<size_t>(300649));
        hash_map_.resize(modulo_, vector<KeyType>(1));
    }

    void Initialize(const size_t size = 100000) {
        modulo_ = size + 17;
        hash_map_.resize(modulo_, vector<KeyType>(1));
    }

    ~FixedSet() = default;

    KeyType Remove(const KeyType value) {
        size_t index = hash(value) % modulo_;
        for (size_t iter = 1; iter < hash_map_[index].size(); ++iter) {
            if (hash_map_[index][iter] == value) {
                KeyType result = hash_map_[index][iter];
                hash_map_[index].erase(hash_map_[index].begin() + iter);
                return result;
            }
        }
    }

    void Add(const KeyType value) {
        size_t index = hash(value) % modulo_;
        hash_map_[index].push_back(value);
    }

    bool Contains(const KeyType value) const {
        size_t index = hash(value) % modulo_;
        if (hash_map_[index].size() == 1) {
            return false;
        } else {
            for (size_t i = 1; i < hash_map_[index].size(); ++i) {
                if (hash_map_[index][i].value == value) {
                    return true;
                }
            }
            return false;
        }
    }
};


#endif // ALGORITHMS_FIXED_SET_H
