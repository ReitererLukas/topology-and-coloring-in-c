#include <iostream>
#include "delta2/delta2.h"

void delta2(int input_colors) {
    Delta2GraphCreator gc = Delta2GraphCreator(input_colors);
    gc.construct();
    gc.solveSAT();

}

void delta2loop(int upper_bound) {

    for(int input_colors = 4; input_colors < upper_bound; input_colors++) {
        std::cout << std::endl << std::endl << "===================" << std::endl << "Starting colors: " << input_colors << std::endl;
        Delta2GraphCreator gc = Delta2GraphCreator(input_colors);
        gc.construct();
        gc.solveSAT();
    }
}

int main() {
    // delta2(20);
    delta2loop(20);
    return 0;
}