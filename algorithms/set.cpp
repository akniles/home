//
// Created by Admin on 11.11.2019.
//

#include "set.h"

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::Find(const ValueType &value, TNode *start) {
    if (size_ == 0) {
        return root_;
    }
    if (start == nullptr) {
        return Find(value, root_);
    }
    if (start->value < value or value < start->value) {
        if (value < start->value) {
            if (start->left != nullptr) {
                return Find(value, start->left);
            } else {
                return start;
            }
        } else {
            if (start->right != nullptr) {
                return Find(value, start->right);
            } else {
                return start;
            }
        }
    } else {
        return nullptr;
    }
}

template<class ValueType>
void Set<ValueType>::Initialize(size_t size) {
    root_ = CreateNode();
    values_.reserve(size);
    root_->parent = nullptr;
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::GetGrandParent(TNode *node) {
    if (node != nullptr and (node->parent != nullptr)) {
        return node->parent->parent;
    } else {
        return nullptr;
    }
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::GetBrother(TNode *node) {
    if (node == node->parent->left) {
        return node->parent->right;
    } else {
        return node->parent->left;
    }
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::GetUncle(TNode *node) {
    auto parent = GetGrandParent(node);
    if (parent == nullptr) {
        return nullptr;
    } else if (parent->left == node->parent) {
        return parent->right;
    } else {
        return parent->left;
    }
}

template<class ValueType>
bool Set<ValueType>::Check(TNode *node) {
    if (size_ == 0) {
        return true;
    }
    if (node == nullptr) {
        return Check(root_);
    }
    if (node->color and
        ((node->right != nullptr and node->right->color) or (node->left != nullptr and node->left->color))) {
        std::cout << "wrong color " << node->value.value << "\n";
        return false;
    }
    if (node->right == nullptr and node->left == nullptr) {
        return true;
    } else if (node->right == nullptr) {
        return Check(node->left);
    } else if (node->left == nullptr) {
        return Check(node->right);
    } else {
        return Check(node->right) and Check(node->left);
    }
}

template<class ValueType>
void Set<ValueType>::Rotate(TNode *node, int type) {
    TNode *pivot;
    if (!type) {
        pivot = node->left;
    } else {
        pivot = node->right;
    }
    pivot->parent = node->parent;
    if (node->parent != nullptr) {
        if (node->parent->left == node) {
            node->parent->left = pivot;
        } else {
            node->parent->right = pivot;
        }
    }
    if (type == 0) {
        node->left = pivot->right;
        if (pivot->right != nullptr) {
            pivot->right->parent = node;
        }
        node->parent = pivot;
        if (root_ == node) {
            root_ = pivot;
        }
        pivot->right = node;
    } else {
        node->right = pivot->left;
        if (pivot->left != nullptr) {
            pivot->left->parent = node;
        }
        node->parent = pivot;
        if (root_ == node) {
            root_ = pivot;
        }
        pivot->left = node;
    }
}

template<class ValueType>
void Set<ValueType>::RedFatherAndBlackUncle(TNode *node) {
    auto grand_parent = GetGrandParent(node);
    node->parent->color = 0;
    grand_parent->color = 1;
    if (node == node->parent->left and node->parent == grand_parent->left) {
        Rotate(grand_parent, 0);
    } else {
        Rotate(grand_parent, 1);
    }
}

template<class ValueType>
void Set<ValueType>::BlackUncleAndRedFather(TNode *node) {
    auto grand_parent = GetGrandParent(node);
    if (node == node->parent->right and node->parent == grand_parent->left) {
        Rotate(node->parent, 1);
        node = node->left;
    } else if (node == node->parent->left and node->parent == grand_parent->right) {
        Rotate(node->parent, 0);
        node = node->right;
    }
    RedFatherAndBlackUncle(node);
}

template<class ValueType>
void Set<ValueType>::RedUncleAndRedFather(TNode *node) {
    auto uncle = GetUncle(node);
    if (uncle != nullptr and uncle->color == 1) {
        node->parent->color = 0;
        uncle->color = 0;
        auto grand_parent = GetGrandParent(node);
        grand_parent->color = 1;
        CheckStructure(grand_parent);
    } else {
        BlackUncleAndRedFather(node);
    }
}

template<class ValueType>
void Set<ValueType>::BlackFatherInsert(TNode *node) {
    if (node->parent->color == 1) {
        RedUncleAndRedFather(node);
    }
}

template<class ValueType>
void Set<ValueType>::CheckStructure(TNode *node) {
    if (node->parent == nullptr) {
        node->color = 0;
    } else {
        BlackFatherInsert(node);
    }
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::FindRemove(const ValueType &value, TNode *start) const {
    if (size_ == 0) {
        return nullptr;
    }
    if (start == nullptr) {
        return FindRemove(value, root_);
    }
    if (value < start->value) {
        if (start->left != nullptr) {
            return FindRemove(value, start->left);
        }
    } else if (not (value < start->value or start->value < value)) {
        return start;
    } else if (start->right != nullptr) {
        return FindRemove(value, start->right);
    }
    return nullptr;
}

template<class ValueType>
void Set<ValueType>::ReplaceNode(TNode *old, TNode *replacing) {
    replacing->parent = old->parent;
    if (old == root_) {
        root_ = replacing;
        return;
    }
    if (old == old->parent->left) {
        old->parent->left = replacing;
    } else {
        old->parent->right = replacing;
    }
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::GetMaximumNode(TNode *node) {
    if (node->right == nullptr) {
        return node;
    } else {
        return GetMaximumNode(node->right);
    }
}

template<class ValueType>
void Set<ValueType>::BlackBrotherRedSon(TNode *node) {
    auto brother = GetBrother(node);

    brother->color = node->parent->color;
    node->parent->color = 0;

    if (node == node->parent->left) {
        brother->right->color = 0;
        Rotate(node->parent, 1);
    } else {
        brother->left->color = 0;
        Rotate(node->parent, 0);
    }
}

template<class ValueType>
void Set<ValueType>::LeftSonRedRightSonBlack(TNode *node) {
    auto brother = GetBrother(node);
    if (brother->color == 0) {
        if (node == node->parent->left and
            brother->right->color == 0 and
            brother->left->color == 1) {
            brother->color = 1;
            brother->left->color = 0;
            Rotate(brother, 0);
        } else if (node == node->parent->right and
                   brother->left->color == 0 and
                   brother->right->color == 1) {
            brother->color = 1;
            brother->right->color = 0;
            Rotate(brother, 1);
        }
    }
    BlackBrotherRedSon(node);
}

template<class ValueType>
void Set<ValueType>::RedParentBlackKids(TNode *node) {
    auto brother = GetBrother(node);

    if (node->parent->color == 1 and
        brother->color == 0 and
        brother->left->color == 0 and
        brother->right->color == 0) {
        brother->color = 1;
        node->parent->color = 0;
    } else
        LeftSonRedRightSonBlack(node);
}

template<class ValueType>
void Set<ValueType>::AllKidsBlack(TNode *node) {
    auto brother = GetBrother(node);
    if (node->parent->color == 0 and
        brother->color == 0 and
        brother->left->color == 0 and
        brother->right->color == 0) {
        brother->color = 1;
        RootDelete(node->parent);
    } else {
        RedParentBlackKids(node);
    }
}

template<class ValueType>
void Set<ValueType>::RedBrotherDelete(TNode *node) {
    auto brother = GetBrother(node);
    if (brother == nullptr) {
        return;
    }
    if (brother->color == 1) {
        node->parent->color = 1;
        brother->color = 0;
        if (node == node->parent->left) {
            Rotate(node, 1);
        } else {
            Rotate(node, 0);
        }
    }
    AllKidsBlack(node);
}

template<class ValueType>
void Set<ValueType>::RootDelete(TNode *node) {
    if (node->parent != nullptr) {
        RedBrotherDelete(node);
    }
}

template<class ValueType>
void Set<ValueType>::RemoveNode(TNode *node) {
    if (node->left != nullptr and node->right != nullptr) {
        auto maximum_left = GetMaximumNode(node->left);
        node->value = maximum_left->value;
        RemoveNode(maximum_left);
    } else if (node->left == nullptr and node->right == nullptr) {
        if (node->parent != nullptr and node == node->parent->right) {
            node->parent->right = nullptr;
        } else if (node->parent != nullptr and node == node->parent->left) {
            node->parent->left = nullptr;
        }
        delete node;
        --memory_balance;
        --size_;
    } else {
        TNode *child;
        if (node->right == nullptr) {
            child = node->left;
        } else {
            child = node->right;
        }
        if (node->color == 0) {
            if (child->color == 1) {
                child->color = 0;
            } else {
                RootDelete(child);
            }
        }
        ReplaceNode(node, child);
        delete node;
        --memory_balance;
        --size_;
    }
    if (not size_) {
        root_ = nullptr;
    }
}

template<class ValueType>
void Set<ValueType>::InOrder_(TNode *node, vector<ValueType> *values) const {
    if (not size_) {
        return;
    }
    if (node->left != nullptr) {
        InOrder_(node->left, values);
    }
    values->push_back(node->value);
    if (node->right != nullptr) {
        InOrder_(node->right, values);
    }
}

template<class ValueType>
void Set<ValueType>::FreeNodes(TNode *node) {
    if (size_ == 0) {
        return;
    }
    if (node == nullptr) {
        return;
    }
    if (node->right != nullptr) {
        FreeNodes(node->right);
    }
    if (node->left != nullptr) {
        FreeNodes(node->left);
    }
    delete node;
    --memory_balance;
}

template<class ValueType>
Set<ValueType>::Set(const size_t size) {
    Initialize(size);
    values_.resize(0);
}

template<class ValueType>
Set<ValueType>::~Set() {
    FreeNodes(root_);
    if (memory_balance != 0) {
        // std::cout << "\nMem Leak! : " << memory_balance << "\n";
    }
}

template<class ValueType>
void Set<ValueType>::insert(const ValueType &value) {
    if (size_ == 0) {
        Initialize();
    }
    values_.push_back(value);
    auto node = Find(value);
    if (node == nullptr) {
        return;
    }
    if (value < node->value and size_) {
        node->left = CreateNode();
        node->left->parent = node;
        node = node->left;
    } else if (size_) {
        node->right = CreateNode();
        node->right->parent = node;
        node = node->right;
    } else {
        node = root_;
    }
    node->value = value;
    node->color = 1;
    ++size_;
    CheckStructure(node);
}

template<class ValueType>
void Set<ValueType>::erase(const ValueType &value) {
    auto node = FindRemove(value);
    if (node == nullptr) {
        return;
    }
    RemoveNode(node);
}

template<class ValueType>
void Set<ValueType>::PrintTree(TNode *start) const {
    if (size_ == 0) {
        return;
    }
    if (start == nullptr and root_ == nullptr) {
        return;
    } else if (start == nullptr) {
        PrintTree(root_);
    } else {
        if (start->left != nullptr) {
            PrintTree(start->left);
        }
        std::cout << start->value.value << " " << start->color << "\n";
        if (start->right != nullptr) {
            PrintTree(start->right);
        }
    }
}

template<class ValueType>
vector<ValueType> Set<ValueType>::InOrder() const {
    vector<ValueType> values;
    values.reserve(size_);
    InOrder_(root_, &values);
    return values;
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::FindLess(const ValueType &value, TNode *start, ValueType min) const {
    if (min < start->value and start->value < value) {
        min = start->value;
    }
    if (value < start->value) {
        if (start->left != nullptr) {
            return FindLess(value, start->left, min);
        } else {
            ValueType find;
            find = min;
            return FindRemove(find);
        }
    } else {
        if (start->right == nullptr) {
            ValueType find;
            find = min;
            return FindRemove(find);
        } else {
            return FindLess(value, start->right, min);
        }
    }
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::FindMore(const ValueType &value, TNode *start, ValueType max) const {
    if (max > start->value > value) {
        max = start->value;
    }
    if (value > start->value) {
        if (start->right != nullptr) {
            return FindMore(value, start->right, max);
        } else {
            ValueType find;
            find = max;
            return FindRemove(find);
        }
    } else {
        if (start->left == nullptr) {
            ValueType find;
            find = max;
            return FindRemove(find);
        } else {
            return FindMore(value, start->left, max);
        }
    }
}

template<class ValueType>
inline bool Set<ValueType>::empty() const {
    return size_ == 0;
}

template<class ValueType>
void Set<ValueType>::PrintTree() const {
    PrintTree(root_);
}

template<class ValueType>
Set<ValueType>::Set() {
    values_.resize(0);
}

template<class ValueType>
template<class Iter>
Set<ValueType>::Set(Iter begin, Iter end) {
    for (Iter iter = begin; iter != end; ++iter) {
        insert(*iter);
    }
}

template<class ValueType>
Set<ValueType>::Set(const std::initializer_list<ValueType> &list) {
    for (const auto& element : list) {
        insert(element);
    }
}

template<class ValueType>
Set<ValueType>::Set(const Set &set) : values_(set.values_) {
    FreeNodes(root_);
    if (set.size_ == 0) {
        root_ = nullptr;
        return;
    }
    Initialize();
    size_ = set.size_;
    CopyTree(root_, set.root_);
}

template<class ValueType>
size_t Set<ValueType>::size() const {
    return size_;
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::Min(TNode *node) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->left != nullptr) {
        return Min(node->left);
    }
    return node;
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::Max(Set::TNode *node) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->right != nullptr) {
        return Max(node->right);
    }
    return node;
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::Previous(Set::TNode *node) const {
    if (node->left != nullptr) {
        return Max(node->left);
    } else {
        while (node->parent != nullptr and node != node->parent->right) {
            node = node->parent;
        }
        if (node == node->parent->right) {
            return node->parent;
        } else {
            return nullptr;
        }
    }
}

template<class ValueType>
typename Set<ValueType>::TNode *Set<ValueType>::Next(Set::TNode *node) const {
    if (node->right != nullptr) {
        return Min(node->right);
    } else {
        while (node->parent != nullptr and node != node->parent->left) {
            node = node->parent;
        }
        if (node->parent != nullptr and node == node->parent->left) {
            return node->parent;
        } else {
            return nullptr;
        }
    }
}

template<class ValueType>
void Set<ValueType>::CopyTree(Set::TNode *one, Set::TNode *other) {
    one->value = other->value;
    one->color = other->color;

    if (other->right != nullptr) {
        if (one->right == nullptr) {
            one->right = CreateNode();
            one->right->parent = one;
        }
        CopyTree(one->right, other->right);
    }
    if (other->left != nullptr) {
        if (one->left == nullptr) {
            one->left = CreateNode();
            one->left->parent = one;
        }
        CopyTree(one->left, other->left);
    }
}

template<class ValueType>
Set<ValueType>& Set<ValueType>::operator=(const Set &set) {
    if (this == &set) {
        return *this;
    }
    FreeNodes(root_);
    if (not set.size_) {
        size_ = 0;
        return *this;
    }
    Initialize();
    size_ = set.size_;
    CopyTree(root_, set.root_);
    return *this;
}

template<class ValueType>
bool Set<ValueType>::operator!=(const Set &set) {
    return not (Equal(root_, set.root_) and Equal(set.root_, root_));
}

template<class ValueType>
bool Set<ValueType>::Equal(Set::TNode *one, Set::TNode *other) {
    bool left = true, right = true;
    if (not (one->value < other->value or other->value < one->value)) {
        if (one->right != nullptr) {
            if (other->right != nullptr) {
                right = Equal(one->right, other->right);
            } else {
                return false;
            }
        }
        if (one->left != nullptr) {
            if (other->left != nullptr) {
                left = Equal(one->left, other->left);
            } else {
                return false;
            }
        }
        return left and right;
    }
    return false;
}
