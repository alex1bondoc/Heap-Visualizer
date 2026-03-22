#include "Heap.h"
#include "BestFitHeap.h"
#include "FirstFitHeap.h"
#include "NextFitHeap.h"

enum class HeapType : int {
    FIRST_FIT = 0,
    NEXT_FIT,
    BEST_FIT, 
};

HeapType stringToHeapType(std::string type){
    if (type == "FIRST_FIT") {
        return HeapType::FIRST_FIT;
    }
    else if (type == "NEXT_FIT") {
        return HeapType::NEXT_FIT;
    }
    else if (type == "BEST_FIT") {
        return HeapType::BEST_FIT;
    }
    return HeapType::FIRST_FIT;
}

class HeapFactory {
public:
    static Heap *createHeap(std::string type, int size);
private:
    static BestFitHeap *createBestFitHeap(int size);
    static FirstFitHeap *createFirstFitHeap(int size);
    static NextFitHeap *createNextFitHeap(int size);
};
