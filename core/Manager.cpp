#include "Manager.h"

Manager::Manager(int heap_size) : heap_size(heap_size), heaps({}) {};

void Manager::addHeap(std::string type){
    this->heaps.push_back(HeapFactory::createHeap(type, this->heap_size));
}
void Manager::deleteHeap(int index){
    auto it = this->heaps.begin();
    advance(it, index);
    this->heaps.erase(it);
}
