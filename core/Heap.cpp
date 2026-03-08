#include "MemoryBlock.cpp"

class Heap {
private:
    MemoryBlock *head;
    int size;

public:
    Heap(int size) : size(size), head(new MemoryBlock(size, Status::FREE)) {};

    MemoryBlock *GetHead() { return head; }
    int GetSize() { return size; }

    void SetHead(MemoryBlock *head) { this->head = head; }
    void SetSize(int size) { this->size = (size + 7) & ~7; }

    MemoryBlock *myMalloc(int size) {
        size = (size + 7) & ~7;

        MemoryBlock *aux = head;
        MemoryBlock *best_fit = nullptr;
        while (aux != nullptr) {
            if (aux->GetSize() >= size && aux->GetStatus() == Status::FREE) {
                if (aux->GetSize() < best_fit->GetSize()) {
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
    MemoryBlock *myCalloc(int size) {
        size = (size + 7) & ~7;

        MemoryBlock *aux = head;
        MemoryBlock *best_fit = nullptr;
        while (aux != nullptr) {
            if (aux->GetSize() >= size && aux->GetStatus() == Status::FREE) {
                if (aux->GetSize() < best_fit->GetSize()) {
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
        if (size <= block->GetSize()) {
            return block->SplitBlock(size);
        }
        else{
            MemoryBlock *next = block->GetNext();
            MemoryBlock *prev = block->GetPrev();
            if (next->GetStatus() == Status::FREE && block->GetSize() + next->GetSize() > size) {
                block->joinNext(size - block->GetSize());
            }
            else if(prev->GetStatus() == Status::FREE && block->GetSize() + prev->GetSize() > size) {
                block->joinPrev(size - block->GetSize());
            }
            else if(prev->GetStatus() == Status::FREE && next->GetStatus() == Status::FREE && block->GetSize() + next->GetSize() + prev->GetSize() > size) {
                if (prev->GetSize() > next->GetSize()) {
                    block->joinNext(next->GetSize());
                    block->joinPrev(size - block->GetSize());
                }
                else {
                    block->joinPrev(prev->GetSize());
                    block->joinNext(size - block->GetSize());
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
        
    }
};
