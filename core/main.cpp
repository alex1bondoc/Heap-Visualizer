#include <iostream>
#include <cstring>
#include <sstream>

#include "Manager.h"

int main() {
    Manager manager = Manager(1024);
    manager.addHeap("FIRST_FIT");
    manager.addHeap("NEXT_FIT");
    manager.addHeap("BEST_FIT");
    std::cout << manager.serialize() << std::endl;
    return 0;
}
