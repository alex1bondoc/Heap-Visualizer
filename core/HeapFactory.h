#include "Heap.h"
#include "BestFitHeap.h"
#include "FirstFitHeap.h"
#include "NextFitHeap.h"

enum class HeapType : int {
    FIRST_FIT = 0,
    NEXT_FIT,
    BEST_FIT, 
};

class HeapFactory {
public:
    static Heap *createHeap(std::string type, int size);
private:
    static BestFitHeap *createBestFitHeap(int size);
    static FirstFitHeap *createFirstFitHeap(int size);
    static NextFitHeap *createNextFitHeap(int size);
};
