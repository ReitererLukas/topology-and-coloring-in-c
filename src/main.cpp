#include <iostream>
#include "delta2/delta2.h"

void delta2() {
    Delta2GraphCreator gc = Delta2GraphCreator(20, 3);
    gc.construct();
    gc.solveSAT();


    // int x;
    // std::cin >> x;

}

int main() {
    delta2();
    return 0;
}