#include "types.h"

#include <algorithm>
#include <cassert>
#include <cstdint>

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
int Node::getId(int inputColors) const {
    assert(inputColors >= 2);

    assert(left_left_ < inputColors);
    assert(left_ < inputColors);
    assert(center_ < inputColors);
    assert(right_ < inputColors);
    assert(right_right_ < inputColors);

    assert(left_left_ != left_);
    assert(left_ != center_);
    assert(center_ != right_);
    assert(right_ != right_right_);

    const int side = inputColors - 1;
    const int numberOfArms = side * side;
    const int nodesPerCenter =
        numberOfArms * (numberOfArms + 1) / 2;

    auto removeExcludedValue = [](unsigned value, unsigned excluded) {
        assert(value != excluded);
        return value - (value > excluded ? 1 : 0);
    };

    auto armRank = [&](unsigned near, unsigned outer) {
        const int nearRank = removeExcludedValue(near, center_);
        const int outerRank = removeExcludedValue(outer, near);

        return nearRank * side + outerRank;
    };

    const std::uint64_t leftArm = armRank(left_, left_left_);
    const std::uint64_t rightArm = armRank(right_, right_right_);

    const std::uint64_t lower = std::min(leftArm, rightArm);
    const std::uint64_t upper = std::max(leftArm, rightArm);

    // Perfect rank for an unordered pair with repetition.
    const int pairRank = upper * (upper + 1) / 2 + lower;

    return ((int) center_) * nodesPerCenter + pairRank + 1;
}