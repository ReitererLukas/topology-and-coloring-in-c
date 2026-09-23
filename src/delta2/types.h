#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uchar;
typedef unsigned short ushort;

struct Node {
    uchar left_left_;
    uchar left_;
    uchar center_;
    uchar right_;
    uchar right_right_;

    Node(uchar left_left, uchar left, uchar center, uchar right, uchar right_right) : left_left_{left_left}, left_{left}, center_{center}, right_{right}, right_right_{right_right} {};
    
    Node(Node* node, uchar left_left, uchar right_right) : left_left_{left_left}, left_{node->left_left_}, center_{node->left_}, right_{node->center_}, right_right_{right_right} {};

    friend bool operator<(const Node& lhs, const Node& rhs) {
        if (lhs.left_left_ == rhs.left_left_) {
            if (lhs.left_ == rhs.left_) {
                if (lhs.center_ == rhs.center_) {
                    if (lhs.right_ == rhs.right_) {
                        if (lhs.right_right_ == rhs.right_right_) {
                            return false;
                        }
                        return lhs.right_right_< rhs.right_right_;
                    }
                    return lhs.right_ < rhs.right_;
                }
                return lhs.center_ < rhs.center_;
            }
            return lhs.left_ < rhs.left_;
        }
        return lhs.left_left_ < rhs.left_left_;
    }

    Node getReversedNode() const;
    void reverse();
    Node shiftLeft(uchar color) const;
    Node shiftRight(uchar color) const;
    int getId(int inputColors) const;

};

struct Edge {
    Node node1_;
    Node node2_;

    Edge(Node node1, Node node2) : node1_{node1}, node2_{node2} {};
};

struct ParamContainer {
    void* classObject_;
    int center_;

    ParamContainer(void* classObject, int center) : classObject_{classObject}, center_{center} {};
};

#endif