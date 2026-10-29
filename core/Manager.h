#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "Heap.h"

class Manager {
private:
    std::vector<std::unique_ptr<Heap>> heaps{};
    int heap_size{};
public:
    Manager(int heap_size);

    // nullptr when the index is out of range. The Manager keeps owning the heap.
    Heap *heap(int index) const;
    int getHeapSize() const { return heap_size; }

    void setHeapSize(int heap_size) { this->heap_size = heap_size; }

    void addHeap(std::string_view type, std::string_view config = "");
    void addHeap(std::unique_ptr<Heap> heap);
    void deleteHeap(int index);
    void resetHeap(int index);
    void clearHeaps();

    std::string serialize() const;
};
