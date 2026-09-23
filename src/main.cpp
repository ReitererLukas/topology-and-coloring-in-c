#include <iostream>
#include "delta2/delta2.h"

void delta2() {
    int NUMBER_OF_STARTING_COLORS = 20;

    Delta2GraphCreator gc = Delta2GraphCreator(NUMBER_OF_STARTING_COLORS, 3);
    gc.create_nodes();
    gc.solveSAT();


    // int x;
    // std::cin >> x;

}

int main() {
    delta2();
    return 0;
}