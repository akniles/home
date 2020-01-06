//
// Created by Admin on 19.11.2019.
//

#ifndef ALGORITHMS_TREAP_H
#define ALGORITHMS_TREAP_H

#include <iostream>
#include <memory>
#include <algorithm>

template<class KeyType, class Comp = std::less<KeyType>>
struct Node {
    Node *left = nullptr;
    Node *right = nullptr;
    KeyType key;
    size_t subtree_size = 1;
    size_t priority = 0;

    inline bool operator<(const Node &other) const {
        return Comp(this->key, other.key);
    }
};


template<class KeyType, class Node = Node<KeyType>>
class Treap {
private:
    Node *root;

    void Insert_(Node* top, Node* node) {
        if (top == nullptr) {
            top = node;
        } else if (node->priority > top->priority) {
            Split(top, node->key, node->left, node->right);
            top = node;
        } else if (node->key < top->key){
            Insert_(top->left, node);
        } else {
            Insert_(top->right, node);
        }
    }

public:

    Treap() {
        root = new Node;
    }

    void Insert(const KeyType &key, const size_t priority) {
        Node node;
        node.key = key;
        node.priority = priority;
        Insert_(root, shared_ptr(node));
    }

    void Split(Node *node, const KeyType &key, Node *left, Node *right) {
        if (node == nullptr) {
            left = nullptr;
            right = nullptr;
        } else if (key < node->key) {
            Split(node->left, key, left, node->left);
            right = node;
        } else {
            Split(node->right, key, node->right, right);
            left = node;
        }
    }
};


#endif //ALGORITHMS_TREAP_H
