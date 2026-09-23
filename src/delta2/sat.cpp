#include "sat.h"
#include <cassert>
#include <iostream>
#include <limits>

void SAT::solve() {
    assert(colors_ > 0);
    assert(numberOfNodes_ * colors_ <= std::numeric_limits<int>::max() && "Due to limitations of the solver we can only support up to 2^31 - 1 variables in sat");

    std::cout << "Creating SAT Node Clauses" << std::endl;
    for(int i = 0; i < numberOfNodes_; i++) {
        Node node = nodes_[i];
        int id = node.getId(inputColors_);
        
        for(int color = 0; color < colors_; color++) {
            solver_.add(id + (color * numberOfNodes_));
        }
        solver_.add(0);
        
        for(int c1 = 0; c1 < colors_; c1++) {
            for(int c2 = c1+1; c2 < colors_; c2++) {
                solver_.add(-(id + (c1 * numberOfNodes_)));
                solver_.add(-(id + (c2 * numberOfNodes_)));
                solver_.add(0);
            }
        }
    }

    std::cout << "Creating SAT Edge Clauses" << std::endl;
}