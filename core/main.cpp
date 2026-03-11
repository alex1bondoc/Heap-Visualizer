#include "Heap.h"
#include <iostream>
#include <cstring>
#include <vector>


int main() {
    Heap *heap = Heap::getInstance(1024);
    char str[100] = ""; 
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
                allocated_blocks.push_back(heap->myCalloc(size));
            }
            else if (strcmp(str, "free") == 0) {
                int index = 0;
                std::cin >> index;
                MemoryBlock *aux = allocated_blocks[index];
                allocated_blocks.erase(allocated_blocks.begin() + index);
                heap->myFree(aux);
            }
        }
        MemoryBlock *aux = heap->getHead();
        while (aux != nullptr) {
            std::string stat= "";
            if (aux->getStatus() == Status::FREE) {
                stat = "FREE";
            }
            else stat = "ALLOC";
            std::cout << aux->getSize() << " " << stat << " ";
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
