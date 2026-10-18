#include "BuddyHeap.h"

BuddyHeap::BuddyHeap(int size) : Heap(size) {
    this->createTopBlocks();
};

// The saved JSON only has sizes, so the buddy links are rebuilt
// by replaying the same splits that produced those blocks.
BuddyHeap::BuddyHeap(int size, char *json) : Heap(size) {
    this->createTopBlocks();
    Heap saved(size, json);
    MemoryBlock *target = saved.getHead();
    MemoryBlock *current = this->getHead();
    while (target != nullptr && current != nullptr) {
        while (current->getSize() > target->getSize()) {
            this->splitHalf(current);
        }
        current->setStatus(target->getStatus());
        current = current->getNext();
        target = target->getNext();
    }
};

// A heap whose size is not a power of two starts as its binary decomposition
// (e.g. 1000 = 512 + 256 + 128 + 64 + 32 + 8). These top blocks have no buddy,
// so they are never merged with each other.
void BuddyHeap::createTopBlocks() {
    delete this->head;
    this->head = nullptr;
    MemoryBlock *last = nullptr;
    int offset = 0;
    while (offset < this->getSize()) {
        int block_size = 1;
        while (block_size * 2 <= this->getSize() - offset) {
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
}

int roundToPowerOfTwo(int size, int minimum) {
    int power = minimum;
    while (power < size) {
        power <<= 1;
    }
    return power;
}

// block keeps the left half and the new right half becomes its buddy.
// The right half remembers block's old buddy so a later merge can restore it.
void BuddyHeap::splitHalf(MemoryBlock *block) {
    int half = block->getSize() / 2;
    MemoryBlock *right = new MemoryBlock(half, Status::FREE, block->getNext(), block);
    if (block->getNext() != nullptr) {
        block->getNext()->setPrev(right);
    }
    block->setNext(right);
    block->setSize(half);

    right->setParentBuddy(block->getBuddy());
    right->setBuddy(block);
    block->setBuddy(right);
}

bool BuddyHeap::canMerge(MemoryBlock *block) {
    MemoryBlock *buddy = block->getBuddy();
    return buddy != nullptr && buddy->getStatus() == Status::FREE && buddy->getSize() == block->getSize();
}

// Joins block with its buddy into the left one and gives it back
// the buddy it had before they were split.
MemoryBlock *BuddyHeap::mergeWithBuddy(MemoryBlock *block) {
    MemoryBlock *buddy = block->getBuddy();
    MemoryBlock *left = block->getNext() == buddy ? block : buddy;
    MemoryBlock *right = left == block ? buddy : block;
    left->setSize(left->getSize() * 2);
    left->setBuddy(right->getParentBuddy());
    right->removeCurrent();
    return left;
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
    // Grow in place only if, at every level, block is the left half
    // and its buddy (the right half) is completely free.
    int current = block->getSize();
    MemoryBlock *buddy = block->getBuddy();
    MemoryBlock *right_neighbour = block->getNext();
    bool canGrow = true;
    while (current < needed) {
        if (buddy == nullptr || buddy != right_neighbour || buddy->getStatus() != Status::FREE || buddy->getSize() != current) {
            canGrow = false;
            break;
        }
        right_neighbour = buddy->getNext();
        buddy = buddy->getParentBuddy();
        current *= 2;
    }
    if (canGrow) {
        while (block->getSize() < needed) {
            this->mergeWithBuddy(block);
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
    while (this->canMerge(block)) {
        block = this->mergeWithBuddy(block);
    }
}
