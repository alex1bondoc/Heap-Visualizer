#include "BuddyHeap.h"

// A heap whose size is not a power of two starts as its binary decomposition
// (e.g. 1000 = 512 + 256 + 128 + 64 + 32 + 8). Each piece is aligned to its size,
// so the offset XOR size rule still finds the right buddy.
BuddyHeap::BuddyHeap(int size) : Heap(size) {
    delete this->head;
    this->head = nullptr;
    MemoryBlock *last = nullptr;
    int offset = 0;
    while (offset < size) {
        int block_size = 1;
        while (block_size * 2 <= size - offset) {
            block_size *= 2;
        }
        MemoryBlock *block = new MemoryBlock(block_size, Status::FREE, nullptr, last);
        if (last == nullptr) {
            this->head = block;
        }
        else {
            last->setNext(block);
        }
        last = block;
        offset += block_size;
    }
};
BuddyHeap::BuddyHeap(int size, char *json) : Heap(size, json) {};

int roundToPowerOfTwo(int size, int minimum) {
    int power = minimum;
    while (power < size) {
        power <<= 1;
    }
    return power;
}

int BuddyHeap::offsetOf(MemoryBlock *block) {
    int offset = 0;
    MemoryBlock *aux = this->getHead();
    while (aux != nullptr && aux != block) {
        offset += aux->getSize();
        aux = aux->getNext();
    }
    return offset;
}

// The buddy of a block of size s at offset o lives at o XOR s,
// so it is always the direct neighbour on one side.
MemoryBlock *BuddyHeap::getBuddy(MemoryBlock *block) {
    if (block->getSize() >= this->getSize()) {
        return nullptr;
    }
    int offset = this->offsetOf(block);
    int buddyOffset = offset ^ block->getSize();
    MemoryBlock *buddy = buddyOffset > offset ? block->getNext() : block->getPrev();
    if (buddy == nullptr || buddy->getSize() != block->getSize()) {
        return nullptr;
    }
    return buddy;
}

void BuddyHeap::splitHalf(MemoryBlock *block) {
    int half = block->getSize() / 2;
    MemoryBlock *buddy = new MemoryBlock(half, Status::FREE, block->getNext(), block);
    if (block->getNext() != nullptr) {
        block->getNext()->setPrev(buddy);
    }
    block->setNext(buddy);
    block->setSize(half);
}

void BuddyHeap::mergeWithNext(MemoryBlock *block) {
    block->setSize(block->getSize() * 2);
    block->getNext()->removeCurrent();
}

MemoryBlock *BuddyHeap::myMalloc(int size) {
    if (size <= 0) {
        return nullptr;
    }
    int needed = roundToPowerOfTwo(size, MIN_BLOCK);
    MemoryBlock *aux = this->getHead();
    MemoryBlock *best_fit = nullptr;
    while (aux != nullptr) {
        if (aux->getStatus() == Status::FREE && aux->getSize() >= needed) {
            if (best_fit == nullptr || aux->getSize() < best_fit->getSize()) {
                best_fit = aux;
            }
        }
        aux = aux->getNext();
    }
    if (best_fit == nullptr) {
        return nullptr;
    }
    while (best_fit->getSize() > needed) {
        this->splitHalf(best_fit);
    }
    best_fit->setStatus(Status::ALLOC);
    return best_fit;
}

MemoryBlock *BuddyHeap::myCalloc(int size) {
    return this->myMalloc(size);
}

MemoryBlock *BuddyHeap::myRealloc(MemoryBlock *block, int size) {
    if (block == nullptr) {
        return myMalloc(size);
    }
    if (block->getStatus() == Status::FREE) {
        return nullptr;
    }
    if (size <= 0) {
        myFree(block);
        return nullptr;
    }
    int needed = roundToPowerOfTwo(size, MIN_BLOCK);
    if (needed <= block->getSize()) {
        while (block->getSize() > needed) {
            this->splitHalf(block);
        }
        return block;
    }
    // Grow in place only while the block is the left half and its right buddy is free.
    int offset = this->offsetOf(block);
    int current = block->getSize();
    MemoryBlock *aux = block->getNext();
    bool canGrow = true;
    while (current < needed) {
        if (offset % (current * 2) != 0 || current * 2 > this->getSize()) {
            canGrow = false;
            break;
        }
        int covered = 0;
        while (aux != nullptr && covered < current && aux->getStatus() == Status::FREE) {
            covered += aux->getSize();
            aux = aux->getNext();
        }
        if (covered != current) {
            canGrow = false;
            break;
        }
        current *= 2;
    }
    if (canGrow) {
        while (block->getSize() < needed) {
            MemoryBlock *buddy = block->getNext();
            while (buddy->getSize() < block->getSize()) {
                // The free right half may still be split into smaller buddies.
                buddy->setSize(buddy->getSize() + buddy->getNext()->getSize());
                buddy->getNext()->removeCurrent();
            }
            this->mergeWithNext(block);
        }
        return block;
    }
    MemoryBlock *new_block = myMalloc(size);
    if (new_block == nullptr) {
        return block;
    }
    myFree(block);
    return new_block;
}

void BuddyHeap::myFree(MemoryBlock *block) {
    if (block == nullptr || block->getStatus() == Status::FREE) {
        return;
    }
    block->setStatus(Status::FREE);
    MemoryBlock *buddy = this->getBuddy(block);
    while (buddy != nullptr && buddy->getStatus() == Status::FREE) {
        if (buddy == block->getPrev()) {
            block = buddy;
        }
        this->mergeWithNext(block);
        buddy = this->getBuddy(block);
    }
}
