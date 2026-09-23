#include "types.h"

Node Node::getReversedNode() const {
    return Node(right_right_, right_, center_, left_, left_left_);
}

void Node::reverse() {
    uchar tmp = right_right_;
    right_right_ = left_left_;
    left_left_ = tmp;

    tmp = right_;
    right_ = left_;
    left_ = tmp;
}

Node Node::shiftLeft(uchar color) const {
    return Node(left_, center_, right_, right_right_, color);
}

Node Node::shiftRight(uchar color) const {
    return Node(color, left_left_, left_, center_, right_);
}

// id is unique for every Node
long Node::getId() const {
    long id = (left_left_ + 1) * 100;
    id = (id + left_) * 100;
    id = (id + center_) * 100;
    id = (id + right_) * 100;
    id = (id + right_right_) * 100;
    return id;
}