#pragma once
#include "Heap.h"
#include <string>
#include <vector>

class SegregatedHeap : public Heap {
private:
    // classes[i] is the slot size of zone i, zone_sizes[i] how many bytes the zone takes.
    // Whatever is left after the class zones is the zone for large requests.
    std::vector<int> classes;
    std::vector<int> zone_sizes;

    void parseConfig(std::string config);
    void setDefaultConfig();
    void createZones();
    int classZonesSize();
    int zoneOf(int offset);
    int zoneFor(int size);
    int allocationSize(int size);
    std::string zoneLabel(int zone);
    int offsetOf(MemoryBlock *block);
    void splitInZone(MemoryBlock *block, int size);
    void mergeInZone(MemoryBlock *block);
public:
    SegregatedHeap(int size, std::string config = "");
    SegregatedHeap(int size, char *json, std::string config = "");
    SegregatedHeap(const SegregatedHeap&) = delete;
    SegregatedHeap operator=(const SegregatedHeap&) = delete;

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string serialize() override;
    std::string getType() override { return "SEGREGATED"; }
    std::string getConfig() override;
};
