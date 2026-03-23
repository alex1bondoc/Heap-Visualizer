#include <iostream>
#include <cstring>
#include <sstream>
#include "Manager.h"
int main() {
    Manager manager = Manager(1024);
    manager.addHeap("FIRST_FIT");
    manager.addHeap("NEXT_FIT");
    manager.addHeap("BEST_FIT");
    for (auto heap : manager.getHeaps()) {
        std::cout << *heap << std::endl;
    }
    return 0;
}
