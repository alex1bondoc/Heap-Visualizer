#include "Heap.cpp"
#include <iostream>
#include <cstring>

int main() {
    Heap heap(1024);
    char str[100] = ""; 
    MemoryBlock *block = nullptr;
    int cnt = 0;
    while (true) {
        std::cin >> str;
        if (strcmp(str, "exit") == 0) {
            break;
        }
        else {
            if (strcmp(str, "malloc") == 0) {
                int size = 0;
                std::cin >> size;
                block = heap.myMalloc(size);
            }
            else if (strcmp(str, "calloc") == 0) {
                int size = 0;
                std::cin >> size;
                block = heap.myMalloc(size);
            }
            else if (strcmp(str, "free") == 0) {
                
                heap.myFree(block);
            }
        }
        MemoryBlock *aux = heap.getHead();
        while (aux != nullptr) {
            std::cout << aux->getSize() << " ";
            aux = aux->getNext();
            cnt ++;
        }
        std::cout << std::endl;
    }
    
    std::cout << cnt << std::endl;
    return 0;
}
