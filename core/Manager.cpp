#include "Manager.h"
#include <sstream>

Manager::Manager(int heap_size) : heap_size(heap_size), heaps({}) {};

Manager::~Manager() {
    for (const Heap* heap : this->heaps) {
        delete heap;
    }; 
    this->heaps.clear();
}

void Manager::addHeap(std::string type){
    this->heaps.push_back(HeapFactory::createHeap(type, this->heap_size));
}
void Manager::deleteHeap(int index){
    auto it = this->heaps.begin();
    advance(it, index);
    this->heaps.erase(it);
}
std::string serialize(const Manager& manager) {
    std::ostringstream ss;
    int heapCounter = manager.heaps.size();
    ss << "{ " << "\"NumberOfHeaps: \"" << heapCounter << ", [ ";
    for (Heap* heap : manager.heaps) {
    }

    return ss.str();

}
