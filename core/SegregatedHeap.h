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
    int classZonesSize() const;
    int zoneOf(int offset) const;
    int zoneFor(int size) const;
    int allocationSize(int size) const;
    std::string zoneLabel(int zone) const;
    int offsetOf(MemoryBlock *block) const;
    void splitInZone(MemoryBlock *block, int size);
    void mergeInZone(MemoryBlock *block);
public:
    SegregatedHeap(int size, std::string config = "");
    SegregatedHeap(int size, char *json, std::string config = "");

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string serialize() const override;
    std::string getType() const override { return "SEGREGATED"; }
    std::string getConfig() const override;
};
