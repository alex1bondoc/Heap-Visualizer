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
std::string Manager::serialize() {
    std::ostringstream ss;
    int heapCounter = this->heaps.size();
    ss << "{ " << "\"NumberOfHeaps\": " << heapCounter << ", \"heaps\": [ ";
    int i{0};
    for (Heap* heap : this->heaps) {
        ss << heap->serialize();
        if (i != heapCounter - 1) {
            ss << ",";
        }
        i += 1;
    }
    ss << "] }";

    return ss.str();

}
