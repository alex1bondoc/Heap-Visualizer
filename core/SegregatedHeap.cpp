#include "SegregatedHeap.h"
#include <sstream>

// Layout: about half of the heap holds one zone per size class
// (16, 32, ... bytes, as many classes as fit), the rest is the zone for large requests.
// Blocks never cross a zone boundary, so a zone can only be used by its own class.
SegregatedHeap::SegregatedHeap(int size) : Heap(size) {
    delete this->head;
    this->head = nullptr;
    MemoryBlock *last = nullptr;
    int count = this->classCount();
    for (int zone = 0; zone <= count; ++zone) {
        int zone_size = zone < count ? this->zoneSize() : size - count * this->zoneSize();
        if (zone_size == 0) {
            continue;
        }
        MemoryBlock *block = new MemoryBlock(zone_size, Status::FREE, nullptr, last);
        if (last == nullptr) {
            this->head = block;
        }
        else {
            last->setNext(block);
        }
        last = block;
    }
};
SegregatedHeap::SegregatedHeap(int size, char *json) : Heap(size, json) {};

// Largest number of classes whose biggest class still fits in its zone.
int SegregatedHeap::classCount() {
    int count = MAX_CLASS_COUNT;
    while (count > 0 && (MIN_CLASS << (count - 1)) > this->getSize() / 2 / count) {
        --count;
    }
    return count;
}

int SegregatedHeap::zoneSize() {
    int count = this->classCount();
    if (count == 0) {
        return 0;
    }
    int largest = MIN_CLASS << (count - 1);
    return this->getSize() / 2 / count / largest * largest;
}

int SegregatedHeap::zoneOf(int offset) {
    int count = this->classCount();
    if (offset >= count * this->zoneSize()) {
        return count;
    }
    return offset / this->zoneSize();
}

int SegregatedHeap::zoneFor(int size) {
    int count = this->classCount();
    for (int zone = 0; zone < count; ++zone) {
        if (size <= (MIN_CLASS << zone)) {
            return zone;
        }
    }
    return count;
}

int SegregatedHeap::allocationSize(int size) {
    int zone = this->zoneFor(size);
    if (zone == this->classCount()) {
        return (size + 7) & ~7;
    }
    return MIN_CLASS << zone;
}

std::string SegregatedHeap::zoneLabel(int zone) {
    if (zone == this->classCount()) {
        return "LARGE";
    }
    return std::to_string(MIN_CLASS << zone);
}

int SegregatedHeap::offsetOf(MemoryBlock *block) {
    int offset = 0;
    MemoryBlock *aux = this->getHead();
    while (aux != nullptr && aux != block) {
        offset += aux->getSize();
        aux = aux->getNext();
    }
    return offset;
}

void SegregatedHeap::splitInZone(MemoryBlock *block, int size) {
    if (block->getSize() > size) {
        MemoryBlock *rest = new MemoryBlock(block->getSize() - size, Status::FREE, block->getNext(), block);
        if (block->getNext() != nullptr) {
            block->getNext()->setPrev(rest);
        }
        block->setNext(rest);
        block->setSize(size);
        this->mergeInZone(rest);
    }
}

// Like mergeNext/mergePrev, but stops at zone boundaries.
void SegregatedHeap::mergeInZone(MemoryBlock *block) {
    int offset = this->offsetOf(block);
    int zone = this->zoneOf(offset);
    MemoryBlock *next = block->getNext();
    if (next != nullptr && next->getStatus() == Status::FREE && this->zoneOf(offset + block->getSize()) == zone) {
        block->setSize(block->getSize() + next->getSize());
        next->removeCurrent();
    }
    MemoryBlock *prev = block->getPrev();
    if (prev != nullptr && prev->getStatus() == Status::FREE && this->zoneOf(offset - prev->getSize()) == zone) {
        prev->setSize(prev->getSize() + block->getSize());
        block->removeCurrent();
    }
}

MemoryBlock *SegregatedHeap::myMalloc(int size) {
    if (size <= 0) {
        return nullptr;
    }
    int zone = this->zoneFor(size);
    int needed = this->allocationSize(size);
    int offset = 0;
    MemoryBlock *aux = this->getHead();
    while (aux != nullptr) {
        if (this->zoneOf(offset) == zone && aux->getStatus() == Status::FREE && aux->getSize() >= needed) {
            aux->setStatus(Status::ALLOC);
            this->splitInZone(aux, needed);
            return aux;
        }
        offset += aux->getSize();
        aux = aux->getNext();
    }
    return nullptr;
}

MemoryBlock *SegregatedHeap::myCalloc(int size) {
    return this->myMalloc(size);
}

MemoryBlock *SegregatedHeap::myRealloc(MemoryBlock *block, int size) {
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
    int zone = this->zoneOf(this->offsetOf(block));
    int needed = this->allocationSize(size);
    if (this->zoneFor(size) == zone) {
        if (needed <= block->getSize()) {
            this->splitInZone(block, needed);
            return block;
        }
        MemoryBlock *next = block->getNext();
        int next_offset = this->offsetOf(block) + block->getSize();
        if (next != nullptr && next->getStatus() == Status::FREE && this->zoneOf(next_offset) == zone
            && block->getSize() + next->getSize() >= needed) {
            block->joinNext(needed - block->getSize());
            return block;
        }
    }
    MemoryBlock *new_block = myMalloc(size);
    if (new_block == nullptr) {
        return block;
    }
    myFree(block);
    return new_block;
}

void SegregatedHeap::myFree(MemoryBlock *block) {
    if (block == nullptr || block->getStatus() == Status::FREE) {
        return;
    }
    block->setStatus(Status::FREE);
    this->mergeInZone(block);
}

std::string SegregatedHeap::serialize() {
    std::ostringstream ss;
    MemoryBlock *aux = this->head;
    int offset = 0;
    ss << "[";
    while (aux != nullptr) {
        std::string block = aux->serialize();
        block.pop_back();
        ss << block << ",\"zone\":\"" << this->zoneLabel(this->zoneOf(offset)) << "\"}";
        offset += aux->getSize();
        aux = aux->getNext();
        if (aux != nullptr) {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}
