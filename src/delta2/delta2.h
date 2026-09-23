#ifndef DELTA2_H
#define DELTA2_H

#include <vector>
#include "types.h"
#include <set>
#include "sat.h"

class Delta2GraphCreator {
private:
    int inputColors_;
    int outputColors_;
    std::vector<int> allColors_;
    long numberOfNodes_;
    long numberOfEdges_;
    // Node* nodes_;
    SAT* sat_;

    const static int N_LEFT_LEFT_INDEX = 0;
    const static int N_LEFT_INDEX = 1;
    const static int N_CENTER_INDEX = 2;
    const static int N_RIGHT_INDEX = 3;
    const static int N_RIGHT_RIGHT_INDEX = 4;

public:
    Delta2GraphCreator(int input_colors, int output_colors);
    ~Delta2GraphCreator();

    void create_nodes();
    bool solveSAT();
    void analyzeStructure();
    
private:
    static long create_nodes_parallel_helper(void* params);
    long connect_node(Node& originalNode) const;
    long create_nodes_parallel(int center) const;
    void canonicalizeNode(Node& node) const;
    void findNeighborsOfSingleOrientedNode(Node& node, std::set<Node>& neighborNodes) const;
    void findNeighborsOfSingleNode(Node& node, std::set<Node>& neighborNodes) const;

};

#endif