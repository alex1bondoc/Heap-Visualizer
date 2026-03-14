#include <string>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#else
    #define EMSCRIPTEN_KEEPALIVE
#endif
#include "Heap.h"

int heap_size = 1024;
Heap *heap = Heap::getInstance(heap_size);

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    const char* getHeap() {
        static std::string json;
        json = serialize(*heap);
        return json.c_str();
    }
    EMSCRIPTEN_KEEPALIVE
    const void doMalloc(const int size) {
        heap->myMalloc(size);
    }
    EMSCRIPTEN_KEEPALIVE
    const void doFree(const char *address) {
        if (address != nullptr) {
            return;
        }
        unsigned long adressNumerical = std::stoul(address, nullptr, 16);
        void *ptr = reinterpret_cast<void *>(adressNumerical);
        heap->myFree((MemoryBlock *)ptr);
    }
}
