#include <string>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#else
    #define EMSCRIPTEN_KEEPALIVE
#endif
#include "BestFitHeap.h"

int heap_size = 1024;
BestFitHeap *heap = BestFitHeap::getInstance(1024);

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    const char* wasmGetHeap() {
        static std::string json;
        json = serialize(*heap);
        std::cout << json << std::endl;
        return json.c_str();
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmMalloc(const int size) {
        heap->myMalloc(size);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmFree(const char *address) {
        if (address == nullptr) {
            return;
        }
        unsigned long adressNumerical = std::stoul(address, nullptr, 16);
        void *ptr = reinterpret_cast<void *>(adressNumerical);
        heap->myFree((MemoryBlock *)ptr);
    }
    EMSCRIPTEN_KEEPALIVE 
    const void wasmRealloc(const char *address, const int size) {
        if (address == nullptr) {
            
            return;
        }
        unsigned long adressNumerical = std::stoul(address, nullptr, 16);
        void *ptr = reinterpret_cast<void *>(adressNumerical);
        heap->myRealloc((MemoryBlock *)ptr, size);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmReconstructHeap(char *json) {
        delete heap;
        heap = BestFitHeap::getInstance(heap_size, json);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmResetHeap() {
        delete heap;
        heap = BestFitHeap::getInstance(heap_size);
    }   
}
