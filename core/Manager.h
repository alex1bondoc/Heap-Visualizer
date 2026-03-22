#include "HeapFactory.h"
#include <list>

class Manager {
private:
    std::list<Heap *> heaps;
    int heap_size;
public:
    Manager(int heap_size);
    
    std::list<Heap *> getHeaps() { return heaps; }
    int getHeapSize() { return heap_size; }

    void setHeapSize(int heap_size) { this->heap_size = heap_size; }
    void setHeaps(std::list<Heap *> heaps) { this->heaps = heaps; }

    void addHeap(std::string type);
    void deleteHeap(int index);
};
