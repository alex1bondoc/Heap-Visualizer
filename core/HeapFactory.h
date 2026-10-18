#include "Heap.h"
#include "BestFitHeap.h"
#include "FirstFitHeap.h"
#include "NextFitHeap.h"
#include "BuddyHeap.h"
#include "SegregatedHeap.h"

enum class HeapType : int {
    FIRST_FIT = 0,
    NEXT_FIT,
    BEST_FIT, 
    BUDDY,
    SEGREGATED,
};

class HeapFactory {
public:
    static Heap *createHeap(std::string type, int size);
    static Heap *createHeap(std::string type, int size, char *json);
private:
    static BestFitHeap *createBestFitHeap(int size);
    static FirstFitHeap *createFirstFitHeap(int size);
    static NextFitHeap *createNextFitHeap(int size);
};
