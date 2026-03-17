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
    const char* wasmGetHeap() {
        static std::string json;
        json = serialize(*heap);
        return json.c_str();
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmMalloc(const int size) {
        heap->myMalloc(size);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmFree(const char *address) {
        std::cout << address <<std::endl;
        if (address == nullptr) {
            std::cout << "what" << std::endl;
            return;
        }
        unsigned long adressNumerical = std::stoul(address, nullptr, 16);
        void *ptr = reinterpret_cast<void *>(adressNumerical);
        std::cout << (MemoryBlock *)ptr << std::endl;
        heap->myFree((MemoryBlock *)ptr);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmReconstructHeap(char *json) {
        delete heap;
        std::cout << json << std::endl;
        heap->getInstance(heap_size, json);
        std::cout << serialize(*heap) << std::endl;
    }
}
