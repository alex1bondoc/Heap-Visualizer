#include "Heap.h"
#include <sstream>
#include <cstring>

Heap::Heap(int size) : size(size), head(new MemoryBlock(size, Status::FREE)) {};
Heap::Heap(int size, char *json) : size(size), head(nullptr){
    char *p = strtok(json, "[]");
    char *p2 = strtok(p, ",");
    MemoryBlock *aux = nullptr;
    Status current_status;
    while (p2 != nullptr) {
        if (strstr(p2, "status") != nullptr){
            if (strstr(p2, "FREE") != nullptr) {
                current_status = Status::FREE;
            }
            else {
                current_status = Status::ALLOC;
            }
        }
        else if (strstr(p2, "size") != nullptr){
            int x = 0, power = 1;
            bool foundNumber = false;
            for (int i = strlen(p2) - 1; i >= 0; --i) {
                if (p2[i] == ' ' && foundNumber) {
                        break;
                }
                else if (p2[i] <= '9' && p2[i] >= '0') {
                    foundNumber = true;
                    x += power * (p2[i] - '0');
                    power *= 10;
                }
            }
            if (aux == nullptr) {
                this->head = new MemoryBlock(x, current_status);
                aux = head;
            }
            else {
                MemoryBlock *newMemoryBlock = new MemoryBlock(x, current_status, nullptr, aux);
                aux->setNext(newMemoryBlock);
                aux = aux->getNext();
            }
        }
        p2 = strtok(nullptr, ",");
    }
}

Heap::~Heap() {
    MemoryBlock *aux = head;
    while (head != nullptr) {
        aux = head->getNext();
        delete head;
        head = aux;
    }
}

MemoryBlock *Heap::myMalloc(int size) {
    return nullptr;
}
MemoryBlock *Heap::myCalloc(int size) {
    return nullptr;
}
MemoryBlock *Heap::myRealloc(MemoryBlock *block, int size) {
    return nullptr;
}
void Heap::myFree(MemoryBlock *block) {
}

std::ostream& operator<<(std::ostream& os, const Heap& heap) {
    std::cout << "Heap " << std::endl;
    MemoryBlock *aux = heap.head;
    while (aux != nullptr) {
        os << *aux;
        aux = aux->getNext();
    }
    os << std::endl;
    return os;
}

std::string Heap::serialize() {
    std::ostringstream ss;
    MemoryBlock *aux = this->head;
    ss << "[";
    while (aux != nullptr) {
        ss << aux->serialize();
        aux = aux->getNext();
        if (aux != nullptr) {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}
