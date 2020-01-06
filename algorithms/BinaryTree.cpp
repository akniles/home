#include <iostream>
#include <vector>

using std::vector;

class BinaryTree {
private:
    struct node {
        int field;
        struct node *left;
        struct node *right;
    };
    vector<int> tree_sequence_;
    size_t size_ = 0;
    size_t index_ = 0;


public:
    void GetIndexValue() {

    }
};


