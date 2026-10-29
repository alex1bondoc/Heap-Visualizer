#pragma once
#include <string>
#include <string_view>
#include <vector>
#include "Heap.h"

class Manager {
private:
    std::vector<Heap *> heaps{};
    int heap_size{};
public:
    Manager(int heap_size);
    ~Manager();
    
    std::vector<Heap *>& getHeaps() { return heaps; }
    int getHeapSize() const { return heap_size; }

    void setHeapSize(int heap_size) { this->heap_size = heap_size; }
    void setHeaps(std::vector<Heap *> heaps) { this->heaps = heaps; }

    void addHeap(std::string_view type, std::string_view config = "");
    void deleteHeap(int index);
    void clearHeaps();

    std::string serialize() const;
};
