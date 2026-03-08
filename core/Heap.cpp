#include "MemoryBlock.cpp"

class Heap {
private:
    MemoryBlock *head;
    int size;

public:
    Heap(int size) : size(size), head(new MemoryBlock(size, Status::FREE)) {};

    MemoryBlock *getHead() { return head; }
    int getSize() { return size; }

    void setHead(MemoryBlock *head) { this->head = head; }
    void setSize(int size) { this->size = (size + 7) & ~7; }

    MemoryBlock *myMalloc(int size) {
        size = (size + 7) & ~7;

        MemoryBlock *aux = this->getHead();
        MemoryBlock *best_fit = nullptr;
        while (aux != nullptr) {
            if (aux->getSize() >= size && aux->getStatus() == Status::FREE) {
                if (best_fit == nullptr || aux->getSize() < best_fit->getSize()) {
                    best_fit = aux;
                }
            }
            aux = aux->getNext();
        }
        if (best_fit == nullptr) {
            return nullptr;
        }
        best_fit->SplitBlock(size);
        return best_fit;
    }
    MemoryBlock *myCalloc(int size) {
        size = (size + 7) & ~7;

        MemoryBlock *aux = head;
        MemoryBlock *best_fit = nullptr;
        while (aux != nullptr) {
            if (aux->getSize() >= size && aux->getStatus() == Status::FREE) {
                if (aux->getSize() < best_fit->getSize()) {
                    best_fit = aux;
                }
            }
        }
        if (best_fit == nullptr) {
            return nullptr;
        }
        best_fit = best_fit->SplitBlock(size);
        return best_fit;
    }
    MemoryBlock *myRealloc(MemoryBlock *block, int size) {
        size = (size + 7) & ~7;
        if (block == nullptr) {
            return myMalloc(size);
        }
        if (size == 0) {
            myFree(block);
            return nullptr;
        }
        if (size <= block->getSize()) {
            return block->SplitBlock(size);
        }
        else{
            MemoryBlock *next = block->getNext();
            MemoryBlock *prev = block->getPrev();
            if (next->getStatus() == Status::FREE && block->getSize() + next->getSize() > size) {
                block->joinNext(size - block->getSize());
            }
            else if(prev->getStatus() == Status::FREE && block->getSize() + prev->getSize() > size) {
                block->joinPrev(size - block->getSize());
            }
            else if(prev->getStatus() == Status::FREE && next->getStatus() == Status::FREE && block->getSize() + next->getSize() + prev->getSize() > size) {
                if (prev->getSize() > next->getSize()) {
                    block->joinNext(next->getSize());
                    block->joinPrev(size - block->getSize());
                }
                else {
                    block->joinPrev(prev->getSize());
                    block->joinNext(size - block->getSize());
                }
            }
            else{
                MemoryBlock *new_block = myMalloc(size);
                if (new_block == nullptr) {
                    return nullptr;
                }
                myFree(block);
                return new_block;
            }
        }
        return nullptr;
    }
    void myFree(MemoryBlock *block) {
        if (block == nullptr) {
            return;
        }
        block->setStatus(Status::FREE);
        if (block->getNext()->getStatus() == Status::FREE) {
            block->mergeNext();
        }
        if (block->getPrev()->getStatus() == Status::FREE) {
            block->mergePrev();
        }
    }
};
