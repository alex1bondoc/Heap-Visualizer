#include "Heap.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>


int main() {
    Heap *heap = Heap::getInstance(1024);
    heap->myMalloc(128);
    std::cout << serialize(*heap) << std::endl;
    std::ostringstream jsonStream;
    jsonStream << serialize(*heap);
    delete heap;
    char json[255];
    strncpy(json, jsonStream.str().c_str(), 255 - 1);
    std::cout << serialize(*Heap::getInstance(1024, json)) << std::endl;
    return 0;
}
