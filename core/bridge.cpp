#include <string>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#else
    #define EMSCRIPTEN_KEEPALIVE
#endif
#include "FirstFitHeap.h"

int heap_size = 1024;
FirstFitHeap *heap = nullptr; 

extern "C" {
    
    EMSCRIPTEN_KEEPALIVE
    const char* wasmGetHeap() {
        if (heap == nullptr) {
            heap = new FirstFitHeap(heap_size);
        }
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
        heap = new FirstFitHeap(1024, json);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmResetHeap() {
        delete heap;
        heap = new FirstFitHeap(1024);
    }   
}
