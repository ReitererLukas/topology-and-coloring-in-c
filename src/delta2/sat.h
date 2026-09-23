
#ifndef SAT_H
#define SAT_H

#include "cadical.hpp"
#include "types.h"

class SAT {
private:
    Node* nodes_;
    Edge* edges_;
    long numberOfNodes_;
    long numberOfEdges_;
    CaDiCaL::Solver solver_;
    int inputColors_;
    int colors_;

    
public:
    SAT(Node* nodes, Edge* edges, long numberOfNodes, long numberOfEdges) : nodes_{nodes}, edges_{edges}, numberOfNodes_{numberOfNodes}, numberOfEdges_{numberOfEdges}, solver_{} {};

    void solve();

};

#endif