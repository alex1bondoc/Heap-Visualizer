#include "HeapFactory.h"
#include <vector>

class Manager {
private:
    std::vector<Heap *> heaps;
    int heap_size;
public:
    Manager(int heap_size);
    
    std::vector<Heap *> getHeaps() { return heaps; }
    int getHeapSize() { return heap_size; }

    void setHeapSize(int heap_size) { this->heap_size = heap_size; }
    void setHeaps(std::vector<Heap *> heaps) { this->heaps = heaps; }

    void addHeap(std::string type);
    void deleteHeap(int index);
};
