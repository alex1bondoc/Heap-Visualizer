#include "Heap.h"
#include <iostream>
#include <cstring>
#include <vector>


int main() {
    Heap *heap = Heap::getInstance(1024);
    char str[100] = ""; 
    MemoryBlock *block = nullptr;
    std::vector<MemoryBlock *> allocated_blocks;
    while (true) {
        std::cin >> str;
        if (strcmp(str, "exit") == 0) {
            break;
        }
        else {
            if (strcmp(str, "malloc") == 0) {
                int size = 0;
                std::cin >> size;
                allocated_blocks.push_back(heap->myMalloc(size));
            }
            else if (strcmp(str, "calloc") == 0) {
                int size = 0;
                std::cin >> size;
                block = heap->myMalloc(size);
            }
            else if (strcmp(str, "free") == 0) {
                
                heap->myFree(block);
            }
        }
        MemoryBlock *aux = heap->getHead();
        while (aux != nullptr) {
            std::cout << aux->getSize() << " ";
            aux = aux->getNext();
        }
        std::cout << std::endl;
        for (auto it : allocated_blocks) {
            std::cout << it->getSize() << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
