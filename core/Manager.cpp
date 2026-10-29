#include "Manager.h"
#include <sstream>
#include "HeapFactory.h"

Manager::Manager(int heap_size) : heap_size(heap_size) {}

Heap *Manager::heap(int index) const {
    if (index < 0 || index >= std::ssize(this->heaps)) {
        return nullptr;
    }
    return this->heaps[index].get();
}

void Manager::addHeap(std::string_view type, std::string_view config) {
    this->heaps.push_back(HeapFactory::createHeap(type, this->heap_size, config));
}
void Manager::addHeap(std::unique_ptr<Heap> heap) {
    this->heaps.push_back(std::move(heap));
}
void Manager::deleteHeap(int index) {
    if (this->heap(index) == nullptr) {
        return;
    }
    this->heaps.erase(this->heaps.begin() + index);
}
// Replaces the heap with a fresh one of the same type and config.
// Assigning the new unique_ptr deletes the old heap.
void Manager::resetHeap(int index) {
    Heap *old_heap = this->heap(index);
    if (old_heap == nullptr) {
        return;
    }
    // getType() views a string literal, so it outlives the old heap;
    // getConfig() builds a new string, which has to be kept as a copy.
    std::string_view type = old_heap->getType();
    std::string config = old_heap->getConfig();
    this->heaps[index] = HeapFactory::createHeap(type, this->heap_size, config);
}
void Manager::clearHeaps() {
    this->heaps.clear();
}
std::string Manager::serialize() const {
    std::ostringstream ss;
    ss << "[";
    for (size_t i = 0; i < this->heaps.size(); ++i) {
        const Heap &heap = *this->heaps[i];
        if (i > 0) {
            ss << ",";
        }
        ss << "{\"type\":\"" << heap.getType() << "\",\"config\":\"" << heap.getConfig() << "\",\"blocks\":" << heap.serialize() << "}";
    }
    ss << "]";
    return ss.str();
}
