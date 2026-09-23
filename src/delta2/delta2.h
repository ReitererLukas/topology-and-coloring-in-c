#ifndef DELTA2_H
#define DELTA2_H

#include <vector>
#include "types.h"
#include <set>

class Delta2GraphCreator {
private:
    int inputColors_;
    int outputColors_;
    std::vector<int> aColors;
    std::vector<int> bColors;
    std::vector<int> allColors;
    long numberOfNodes;
    long numberOfEdges;
    Node* nodes_;
    Edge* edges_;

    const static int N_LEFT_LEFT_INDEX = 0;
    const static int N_LEFT_INDEX = 1;
    const static int N_CENTER_INDEX = 2;
    const static int N_RIGHT_INDEX = 3;
    const static int N_RIGHT_RIGHT_INDEX = 4;

public:
    Delta2GraphCreator(int input_colors, int output_colors);
    ~Delta2GraphCreator();

    void create_nodes();
    void connect_nodes();
    void analyzeStructure();

private:
    void canonicalizeNode(Node& node);
    void findNeighborsOfSingleOrientedNode(Node* node, std::set<Node>& neighborNodes);
    void findNeighborsOfSingleNode(Node* node, std::set<Node>& neighborNodes);

};

#endif