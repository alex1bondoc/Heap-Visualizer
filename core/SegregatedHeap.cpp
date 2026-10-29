#include "SegregatedHeap.h"
#include <sstream>
#include <cstdlib>
#include <string_view>
#include <charconv>
#include <optional>

SegregatedHeap::SegregatedHeap(int size, std::string_view config) : Heap(size) {
    this->parseConfig(config);
    this->createZones();
};
SegregatedHeap::SegregatedHeap(int size, char *json, std::string_view config) : Heap(size, json) {
    this->parseConfig(config);
};

// Config format: "class:zoneSize,class:zoneSize,..." e.g. "16:128,32:128,64:128".
// Classes must be increasing multiples of 8, every zone a multiple of its class,
// and the zones must fit in the heap. Anything else falls back to the default.
void SegregatedHeap::parseConfig(std::string_view config) {
    this->classes.clear();
    this->zone_sizes.clear();
    if (config.empty()) {
        this->setDefaultConfig();
        return;
    }
    int total_size = 0;
    int last_class = 0;
    std::string_view remaining = config;
    while (!remaining.empty()) {
        size_t comma_pos = remaining.find(',');
        std::string_view pair = remaining.substr(0, comma_pos);
        size_t colon_pos = pair.find(':');
        if (colon_pos == std::string_view::npos) {
            this->setDefaultConfig();
            return; 
        }
        std::string_view class_str = pair.substr(0, colon_pos);
        std::string_view zone_size_str = pair.substr(colon_pos + 1);
        int size_class, zone_size;
        auto [ptr1, ec1] = std::from_chars(class_str.data(), class_str.data() + class_str.size(), size_class);
        auto [ptr2, ec2] = std::from_chars(zone_size_str.data(), zone_size_str.data() + zone_size_str.size(), zone_size);
        if (ec1 != std::errc() || ec2 != std::errc() || size_class <= 0 || zone_size <= 0 || size_class % 8 != 0 || zone_size % size_class != 0 || size_class <= last_class || total_size + zone_size > this->getSize()) {
            this->setDefaultConfig();
            return; 
        }
        this->classes.push_back(size_class);
        this->zone_sizes.push_back(zone_size);
        total_size += zone_size;
        last_class = size_class;
        if (comma_pos != std::string_view::npos) {
            remaining = remaining.substr(comma_pos + 1);
        } else {
            break;
        }
    }
}

// Default: classes 16, 32 and 64, each with an eighth of the heap.
void SegregatedHeap::setDefaultConfig() {
    this->classes.clear();
    this->zone_sizes.clear();
    for (int size_class : {16, 32, 64}) {
        int zone_size = this->getSize() / 8 / size_class * size_class;
        if (zone_size > 0) {
            this->classes.push_back(size_class);
            this->zone_sizes.push_back(zone_size);
        }
    }
}

std::string SegregatedHeap::getConfig() const {
    std::ostringstream ss;
    for (size_t i = 0; i < this->classes.size(); ++i) {
        if (i > 0) {
            ss << ",";
        }
        ss << this->classes[i] << ":" << this->zone_sizes[i];
    }
    return ss.str();
}

// One free block per zone, so blocks never cross a zone boundary
// and a zone can only be used by its own class.
void SegregatedHeap::createZones() {
    delete this->head;
    this->head = nullptr;
    MemoryBlock *last = nullptr;
    std::vector<int> sizes = this->zone_sizes;
    if (this->getSize() > this->classZonesSize()) {
        sizes.push_back(this->getSize() - this->classZonesSize());
    }
    for (int zone_size : sizes) {
        MemoryBlock *block = new MemoryBlock(zone_size, Status::FREE, nullptr, last);
        if (last == nullptr) {
            this->head = block;
        }
        else {
            last->setNext(block);
        }
        last = block;
    }
}

int SegregatedHeap::classZonesSize() const {
    int total = 0;
    for (int zone_size : this->zone_sizes) {
        total += zone_size;
    }
    return total;
}

// Zones 0..classes.size()-1 are the class zones, classes.size() is the large zone.
int SegregatedHeap::zoneOf(int offset) const {
    int end = 0;
    for (size_t zone = 0; zone < this->zone_sizes.size(); ++zone) {
        end += this->zone_sizes[zone];
        if (offset < end) {
            return zone;
        }
    }
    return this->classes.size();
}

int SegregatedHeap::zoneFor(int size) const {
    for (size_t zone = 0; zone < this->classes.size(); ++zone) {
        if (size <= this->classes[zone]) {
            return zone;
        }
    }
    return this->classes.size();
}

int SegregatedHeap::allocationSize(int size) const {
    int zone = this->zoneFor(size);
    if (zone == (int)this->classes.size()) {
        return (size + 7) & ~7;
    }
    return this->classes[zone];
}

std::string SegregatedHeap::zoneLabel(int zone) const {
    if (zone == (int)this->classes.size()) {
        return "LARGE";
    }
    return std::to_string(this->classes[zone]);
}

int SegregatedHeap::offsetOf(MemoryBlock *block) const {
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

std::string SegregatedHeap::serialize() const {
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
