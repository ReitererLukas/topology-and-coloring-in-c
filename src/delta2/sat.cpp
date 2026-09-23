#include "sat.h"
#include <iostream>

void SAT::addNode(Node& node) {
    int id = node.getId(inputColors_);
    
    mutex_.lock();
    for(int color = 0; color < outputColors_; color++) {
        solver_.add(id + (color * (int) numberOfNodes_));
    }
    solver_.add(0);
    mutex_.unlock();
    
    mutex_.lock();
    for(int c1 = 0; c1 < outputColors_; c1++) {
        for(int c2 = c1+1; c2 < outputColors_; c2++) {
            solver_.add(-(id + (c1 * (int) numberOfNodes_)));
            solver_.add(-(id + (c2 * (int) numberOfNodes_)));
            solver_.add(0);
        }
    }
    mutex_.unlock();
}

void SAT::addEdge(Node& node1, std::set<Node>::iterator& node2) {
    mutex_.lock();
    int id1 = node1.getId(inputColors_);
    int id2 = node2->getId(inputColors_);
    for(int color = 0; color < outputColors_; color++) {
        solver_.add(-(id1 + ( color * (int) numberOfNodes_)));
        solver_.add(-(id2 + ( color * (int) numberOfNodes_)));
        solver_.add(0);
    }
    mutex_.unlock();
}

bool SAT::solve() {
    solver_.statistics();
    int ret = solver_.solve();
    std::cout << "SAT solver returned " << ret << std::endl;
    return (ret == 10);

}