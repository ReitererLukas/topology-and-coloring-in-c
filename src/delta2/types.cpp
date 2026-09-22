#include "types.h"

Node Node::getReversedNode() {
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

Node Node::shiftLeft(uchar color) {
    return Node(left_, center_, right_, right_right_, color);
}

Node Node::shiftRight(uchar color) {
    return Node(color, left_left_, left_, center_, right_);
}