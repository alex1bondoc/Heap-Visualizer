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

        if (block->GetSize() >= size) {
            return block;
        }
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
};
