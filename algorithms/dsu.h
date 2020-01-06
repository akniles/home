#include <vector>

#ifndef ALGORITHMS_DSU_H
#define ALGORITHMS_DSU_H

using std::vector;

class DSU {
private:
    struct Node {
        size_t parent = 0;
        size_t key = 0;
        bool alive = true;
        size_t height = 0;
        size_t last_alive_parent = 0;
    };
    vector<Node> sets_;
    size_t size_;

public:
    explicit DSU(size_t size = 1);

    bool Check(size_t one, size_t other);

    size_t Find(size_t index);

    void Join(const size_t &one, const size_t &other);

    void Kill(const size_t &one);

    size_t Judge(size_t one, size_t other);

    void Birth(size_t father);
};


#endif //ALGORITHMS_DSU_H
