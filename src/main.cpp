#include <iostream>
#include "delta2/delta2.h"
#include <chrono>

void delta2(int input_colors) {
    std::cout << std::endl << std::endl << "===================" << std::endl << "Starting colors: " << input_colors << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    
    Delta2GraphCreator gc = Delta2GraphCreator(input_colors);
    gc.construct();
    gc.solveSAT();

    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << " ms" << std::endl;
}

void delta2loop(int upper_bound) {

    for(int input_colors = 4; input_colors < upper_bound; input_colors++) {
       delta2(input_colors);
    }
}

int main() {
    delta2(5);
    // delta2loop(20);
    return 0;
}