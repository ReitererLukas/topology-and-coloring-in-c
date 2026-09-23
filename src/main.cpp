#include <iostream>
#include "delta2/delta2.h"


void delta2() {
    Delta2GraphCreator gc = Delta2GraphCreator(10, 3);
    gc.create_nodes();
    // gc.connect_nodes();
    gc.analyzeStructure();

    // int x;
    // std::cin >> x;

}

int main() {
    delta2();
    return 0;
}