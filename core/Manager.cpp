#include "Manager.h"
#include <sstream>
#include "HeapFactory.h"

Manager::Manager(int heap_size) : heap_size(heap_size) {};

Manager::~Manager() {
    for (const Heap* heap : this->heaps) {
        delete heap;
    }; 
    this->heaps.clear();
}

void Manager::addHeap(std::string type, std::string config){
    this->heaps.push_back(HeapFactory::createHeap(type, this->heap_size, config));
}
void Manager::deleteHeap(int index){
    auto it = this->heaps.begin();
    advance(it, index);
    delete *it;
    this->heaps.erase(it);
}
void Manager::clearHeaps() {
    for (const Heap* heap : this->heaps) {
        delete heap;
    }
    this->heaps.clear();
}
std::string Manager::serialize() const {
    std::ostringstream ss;
    int heapCounter = this->heaps.size();
    ss << "[";
    int i{0};
    for (Heap* heap : this->heaps) {
        ss << "{\"type\":\"" << heap->getType() << "\",\"config\":\"" << heap->getConfig() << "\",\"blocks\":" << heap->serialize() << "}";
        if (i != heapCounter - 1) {
            ss << ",";
        }
        i += 1;
    }
    ss << "]";

    return ss.str();

}
