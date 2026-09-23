
#ifndef SAT_H
#define SAT_H

#include "cadical.hpp"
#include "types.h"
#include <cassert>
#include <set>
#include <limits>
#include <mutex>

class SAT {
private:
    long numberOfNodes_;
    long numberOfEdges_;
    CaDiCaL::Solver solver_;
    int inputColors_;
    int outputColors_;
    std::mutex mutex_;

    
public:
    SAT(long numberOfNodes, long numberOfEdges, int inputColors, int outputColors) : numberOfNodes_{numberOfNodes}, numberOfEdges_{numberOfEdges}, solver_{}, inputColors_{inputColors},  outputColors_{outputColors} {
        assert(outputColors > 0);
        assert(numberOfNodes * outputColors <= std::numeric_limits<int>::max() && "Due to limitations of the solver we can only support up to 2^31 - 1 variables in sat");
    };

    ~SAT() = default;

    void addNode(Node& node);
    void addEdge(Node& node1, std::set<Node>::iterator& node2);
    bool solve();
};

#endif