#include "Heap.h"
#include <iostream>
#include <cstring>
#include <vector>


int main() {
    int heap_size = 1024;
    Heap *heap = Heap::getInstance(heap_size);
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
                MemoryBlock *new_block = heap->myMalloc(size);
                if (new_block != nullptr) {
                    allocated_blocks.push_back(new_block);
                }
                std::cout << serialize(*heap) << std::endl;
            }
            else if (strcmp(str, "calloc") == 0) {
                int size = 0;
                std::cin >> size;
                MemoryBlock *new_block = heap->myCalloc(size);
                if (new_block != nullptr) {
                    allocated_blocks.push_back(new_block);
                }
            }
            else if (strcmp(str, "free") == 0) {
                int index = 0;
                std::cin >> index;
                MemoryBlock *aux = allocated_blocks[index];
                allocated_blocks.erase(allocated_blocks.begin() + index);
                heap->myFree(aux);
            }
            else if (strcmp(str, "realloc") == 0) {
                int index = 0;
                int new_size = 0;
                std::cin >> index >> new_size;
                MemoryBlock *aux = nullptr;
                if (index >= allocated_blocks.size()) {
                    aux = nullptr;
                }
                else {
                    aux = allocated_blocks[index];
                    allocated_blocks.erase(allocated_blocks.begin() + index);
                }
                MemoryBlock *new_block = heap->myRealloc(aux, new_size);
                std::cout << new_block->getSize() << " " << std::endl;
                if (new_block != nullptr) {
                    std::cout << "Test" << std::endl;
                    allocated_blocks.push_back(new_block);
                }
            } 
        }
        std::cout << *heap <<std::endl;
        for (auto it : allocated_blocks) {
            std::cout << it->getSize() << " ";
        }
        std::cout << std::endl;
    }
    
    delete heap;
    return 0;
}
