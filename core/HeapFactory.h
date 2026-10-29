#pragma once
#include <string>
#include <string_view>
#include "Heap.h"

class BestFitHeap;
class FirstFitHeap;
class NextFitHeap;
class BuddyHeap;
class SegregatedHeap;

enum class HeapType : int {
    FIRST_FIT = 0,
    NEXT_FIT,
    BEST_FIT, 
    BUDDY,
    SEGREGATED,
};

class HeapFactory {
public:
    static Heap *createHeap(std::string_view type, int size, std::string_view config);
    static Heap *createHeap(std::string_view type, int size, char* json, std::string_view config);
private:
    static BestFitHeap *createBestFitHeap(int size);
    static FirstFitHeap *createFirstFitHeap(int size);
    static NextFitHeap *createNextFitHeap(int size);
    static BuddyHeap *createBuddyHeap(int size);
    static SegregatedHeap *createSegregatedHeap(int size, std::string_view config);
};
