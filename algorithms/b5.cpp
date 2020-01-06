#include <algorithm>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

using std::vector;
using std::string;
using std::pair;

std::mt19937 generator(42);
std::uniform_int_distribution<int> random_seed(1, 1e9);

template<class KeyType>
class Treap {
public:
    Treap();

    ~Treap() = default;

    void Insert(const KeyType &key);

    void ImplicitSplit(const KeyType &key, Treap<KeyType> *first, Treap<KeyType> *second);

    void Merge(Treap<KeyType> *one);

    void PrintTree() const;

    void Update();

    [[nodiscard]] vector<KeyType> ReturnTree() const;

    void Delete();

private:
    struct Node {
        Node *left;
        Node *right;
        KeyType key;
        size_t subtree_size;
        size_t priority;

        explicit Node(KeyType value);
    };

    Node *root_;
    size_t size_;
    int memory_balance_;

    void Free(Node *node, bool silent = true);

    void SplitNodes(Treap::Node *node, const KeyType &key,
                    Treap::Node **left, Treap::Node **right);

    void ImplicitKeysSplitNodes(Treap::Node *node, const int &key,
                                Treap::Node **left, Treap::Node **right);

    Node *Find(const KeyType &key, Node *start = nullptr);

    void CopyNode(Node *new_node, const Node &old_node);

    void MergeNodes(Node **root, Node *left, Node *right);

    size_t Update(Node *node);

    void PrintTree(Node *node) const;

    void ReturnTree(Node *node, vector<KeyType> *values) const;

    pair<typename Treap<KeyType>::Node *,
            typename Treap<KeyType>::Node *> SplitInPair(Node *node, KeyType key);

    Node *CopyMerge(Node *left, Node *right);

    void InsertKey(Node **node, KeyType key);
};


template<class KeyType>
Treap<KeyType>::Treap() {
    memory_balance_ = 0;
    root_ = nullptr;
    size_ = 0;
}

template<class KeyType>
Treap<KeyType>::Node::Node(const KeyType value) {
    priority = random_seed(generator);
    subtree_size = 1;
    key = value;
    left = nullptr;
    right = nullptr;
}

/*template<class KeyType>
Treap<KeyType>::~Treap() {
    Free(root_);
}*/

template<class KeyType>
void Treap<KeyType>::Update() {
    Update(root_);
}

template<class KeyType>
void Treap<KeyType>::Delete() {
    Free(root_);
    // std::cout << "memory balance: " << memory_balance_ << "\n";
}

template<class KeyType>
void Treap<KeyType>::Insert(const KeyType &key) {
    if (root_ == nullptr) {
        root_ = new Node(key);
        ++size_;
        ++memory_balance_;
        return;
    }
    InsertKey(&root_, key);
    ++size_;
}

template<class KeyType>
void Treap<KeyType>::PrintTree() const {
    PrintTree(root_);
}

template<class KeyType>
void Treap<KeyType>::SplitNodes(Treap::Node *node, const KeyType &key,
                                Treap::Node **left, Treap::Node **right) {
    if (node == nullptr) {
        *left = nullptr;
        *right = nullptr;
    } else if (key < node->key) {
        SplitNodes(node->left, key, left, &node->left);
        *right = node;
    } else {
        SplitNodes(node->right, key, &node->right, right);
        *left = node;
    }
}

template<class KeyType>
pair<typename Treap<KeyType>::Node *,
        typename Treap<KeyType>::Node *> Treap<KeyType>::SplitInPair(Node *node, KeyType key) {
    if (node == nullptr)
        return {nullptr, nullptr};
    if (node->key < key) {
        pair<Node *, Node *> foo = SplitInPair(node->right, key);
        node->right = foo.first;
        Update(node);
        Update(foo.second);
        return {node, foo.second};
    } else {
        pair<Node *, Node *> foo = SplitInPair(node->left, key);
        node->left = foo.second;
        Update(foo.first);
        Update(node);
        return {foo.first, node};
    }
}

template<class KeyType>
typename Treap<KeyType>::Node *Treap<KeyType>::CopyMerge(Node *left, Node *right) {
    if (left == nullptr or right == nullptr) {
        if (right == nullptr) {
            return left;
        }
        return right;
    }
    if (left->priority > right->priority) {
        left->right = CopyMerge(left->right, right);
        Update(left);
        return left;
    }
    right->left = CopyMerge(left, right->left);
    Update(right);
    return right;
}

template<class KeyType>
void Treap<KeyType>::InsertKey(Node **node, KeyType key) {
    pair<Node *, Node *> foo = SplitInPair(*node, key);
    auto bar = CopyMerge(foo.first, new Node(key));
    ++memory_balance_;
    root_ = CopyMerge(bar, foo.second);
    Update(*node);
}

template<class KeyType>
void Treap<KeyType>::Free(Treap<KeyType>::Node *node, bool silent) {
    if (node == nullptr) {
        return;
    }
    if (node->left != nullptr) {
        Free(node->left, silent);
    }
    if (not silent) {
        std::cout << node->key << "\n";
    }
    if (node->right != nullptr) {
        Free(node->right, silent);
    }
    --memory_balance_;
    delete[] node;
}

template<class KeyType>
typename Treap<KeyType>::Node *Treap<KeyType>::Find(const KeyType &key, Node *start) {
    if (size_ == 0) {
        return root_;
    }
    if (start == nullptr) {
        return Find(key, root_);
    }
    if (key < start->key) {
        if (start->left != nullptr) {
            return Find(key, start->left);
        } else {
            return start;
        }
    } else {
        if (start->right != nullptr) {
            return Find(key, start->right);
        } else {
            return start;
        }
    }
}

template<class KeyType>
void Treap<KeyType>::CopyNode(Treap<KeyType>::Node *new_node,
                              const Treap<KeyType>::Node &old_node) {
    new_node->key = old_node.key;
    new_node->priority = old_node.priority;
    new_node->subtree_size = old_node.subtree_size;
}

template<class KeyType>
void Treap<KeyType>::MergeNodes(Treap<KeyType>::Node **root,
                                Treap<KeyType>::Node *left, Treap<KeyType>::Node *right) {
    if (left == nullptr or right == nullptr) {
        if (right == nullptr) {
            *root = left;
        } else {
            *root = right;
        }
    } else if (left->priority > right->priority) {
        MergeNodes(&left->right, left->right, right);
        *root = left;
    } else {
        MergeNodes(&right->left, left, right->left);
        *root = right;
    }
    Update(*root);
}

template<class KeyType>
size_t Treap<KeyType>::Update(Treap<KeyType>::Node *node) {
    size_t size = 1;
    if (node == nullptr) {
        return 0;
    }
    if (node->right != nullptr) {
        size += node->right->subtree_size;
    }
    if (node->left != nullptr) {
        size += node->left->subtree_size;
    }
    node->subtree_size = size;
    return size;
}

template<class KeyType>
void Treap<KeyType>::ImplicitKeysSplitNodes(Treap::Node *node, const int &key,
                                            Treap::Node **left, Treap::Node **right) {
    if (node == nullptr) {
        *left = nullptr;
        *right = nullptr;
        return;
    }
    int node_key;
    if (node->left != nullptr) {
        node_key = node->left->subtree_size;
    } else {
        node_key = 0;
    }
    if (node_key < key) {
        ImplicitKeysSplitNodes(node->right, key - node_key - 1, &node->right, right);
        *left = node;
    } else {
        ImplicitKeysSplitNodes(node->left, key, left, &node->left);
        *right = node;
    }
}

template<class KeyType>
void Treap<KeyType>::Merge(Treap<KeyType> *one) {
    MergeNodes(&root_, root_, one->root_);
    if (root_) {
        Update(root_);
        size_ = root_->subtree_size;
        memory_balance_ = root_->subtree_size;
    }
}

template<class KeyType>
void Treap<KeyType>::ImplicitSplit(const KeyType &key, Treap<KeyType> *first,
                                   Treap<KeyType> *second) {
    ImplicitKeysSplitNodes(root_, key, &first->root_, &second->root_);
    if (first->root_) {
        Update(first->root_);
    }
    if (second->root_) {
        Update(second->root_);
    }
}

template<class KeyType>
void Treap<KeyType>::PrintTree(Node *node) const {
    if (node->left != nullptr) {
        PrintTree(node->left);
    }
    std::cout << node->key << "\n";
    if (node->right != nullptr) {
        PrintTree(node->right);
    }
}

template<class KeyType>
vector<KeyType> Treap<KeyType>::ReturnTree() const {
    if (root_ == nullptr) {
        return {};
    }
    vector<KeyType> values;
    values.reserve(root_->subtree_size);
    ReturnTree(root_, &values);
    return values;
}

template<class KeyType>
void Treap<KeyType>::ReturnTree(Node *node, vector<KeyType> *values) const {
    if (node->left != nullptr) {
        ReturnTree(node->left, values);
    }
    values->push_back(node->key);
    if (node->right != nullptr) {
        ReturnTree(node->right, values);
    }
}

struct InputData {
    string ciphered_phrase;
    vector<vector<int>> queries;
};

void ReadInput(InputData *data) {
    std::cin >> data->ciphered_phrase;
    size_t queries_number;
    std::cin >> queries_number;
    data->queries.resize(queries_number, vector<int>(3, 0));
    for (auto &query : data->queries) {
        for (auto &element : query) {
            std::cin >> element;
        }
    }
}

void Process(const InputData &data) {
    Treap<int> treap;
    for (int index = 0; index < static_cast<int>(data.ciphered_phrase.size()); ++index) {
        treap.Insert(index);
    }
    treap.Update();

    for (int index = static_cast<int>(data.queries.size() - 1); index >= 0; --index) {
        Treap<int> swaping;
        Treap<int> first;
        Treap<int> second;
        Treap<int> third;
        Treap<int> forth;
        treap.ImplicitSplit(data.queries[index][0] - 1, &first, &second);
        second.ImplicitSplit(data.queries[index][2], &swaping, &third);
        second = swaping;
        third.ImplicitSplit(data.queries[index][1] - data.queries[index][2] -
                            data.queries[index][0] + 1, &swaping, &forth);
        third = swaping;
        first.Merge(&third);
        first.Merge(&second);
        first.Merge(&forth);
        treap = first;
    }
    vector<int> indexes = treap.ReturnTree();
    for (const auto &index : indexes) {
        std::cout << data.ciphered_phrase[index];
    }
    std::cout << "\n";
}

void Shift(string *word, int start, int end, int shift) {
    string first = word->substr(start, end - start - shift + 1);
    string second = word->substr(end - shift + 1, shift);
    size_t index = 0;
    for (const auto &symbol : second) {
        word->at(index + start) = symbol;
        ++index;
    }
    for (const auto &symbol : first) {
        word->at(index + start) = symbol;
        ++index;
    }
}

void Cipher(string *word, const vector<vector<int>> &queries) {
    for (const auto &query : queries) {
        Shift(word, query[0] - 1, query[1] - 1, query[2]);
    }
}

void StressTest() {
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> random(0, 25);
    string word;
    // string word = "goodluck";
    // size_t size = word.size();
    size_t size = 5000;
    for (size_t index = 0; index < size; ++index) {
        word.push_back('a' + random_seed(generator) % 26);
    }
    InputData data;
    data.ciphered_phrase = word;
    data.queries.resize(size, vector<int>(3, 0));
    for (auto &query : data.queries) {
        query[0] = (random_seed(generator) % (size)) + 1;
        query[1] = (random_seed(generator) % (size)) + 1;
        if (query[0] == query[1]) {
            ++query[1];
            query[2] = 1;
            continue;
        }
        if (query[0] > query[1]) {
            std::swap(query[0], query[1]);
        }
        query[2] = (random_seed(generator) % (query[1] - query[0]));
    }
    std::cout << word << "\n";
    Cipher(&word, data.queries);
    data.ciphered_phrase = word;
    std::clock_t c_start = std::clock();
    Process(data);
    std::clock_t c_end = std::clock();
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n";

}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
/*
    InputData data;
    ReadInput(&data);
    Process(data);*/
    StressTest();
    return 0;
}
