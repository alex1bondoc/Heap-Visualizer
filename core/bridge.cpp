#include <string>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#else
    #define EMSCRIPTEN_KEEPALIVE
#endif
#include "Manager.h"

int heap_size = 1024;
NextFitHeap *heap = nullptr; 

Manager* manager = nullptr;

extern "C" {
    
    EMSCRIPTEN_KEEPALIVE
    const void wasmCreateManager() {
        if (manager == nullptr) {
            manager = new Manager(heap_size);
        }
    }

    EMSCRIPTEN_KEEPALIVE
    const void wasmAddHeap(const char* type) {
        manager->addHeap(std::string(type));
    }

    EMSCRIPTEN_KEEPALIVE
    const void wasmDeleteHeap(const int index) {
        if ((index >= 0 && index < manager->getHeaps().size())){
            return ;
        }
        manager->deleteHeap(index);
    }
    EMSCRIPTEN_KEEPALIVE
    const char* wasmGetHeaps() {
        if (manager == nullptr) {
            wasmCreateManager();
        }
        static std::string json = manager->serialize();
        return json.c_str();
    }
    EMSCRIPTEN_KEEPALIVE
    const char* wasmGetHeap() {
        if (manager->getHeaps().size() == 0) {
            wasmCreateManager();
            wasmAddHeap("BEST_FIT");
        }
        static std::string json;
        json = manager->getHeaps()[0]->serialize();
        std::cout << json << std::endl;
        return json.c_str();
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmMalloc(const int size) {
        manager->getHeaps()[0]->myMalloc(size);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmFree(const char *address) {
        if (address == nullptr) {
            return;
        }
        unsigned long adressNumerical = std::stoul(address, nullptr, 16);
        void *ptr = reinterpret_cast<void *>(adressNumerical);
        manager->getHeaps()[0]->myFree((MemoryBlock *)ptr);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmRealloc(const char *address, const int size) {
        if (address == nullptr) {
            
            return;
        }
        unsigned long adressNumerical = std::stoul(address, nullptr, 16);
        void *ptr = reinterpret_cast<void *>(adressNumerical);
        manager->getHeaps()[0]->myRealloc((MemoryBlock *)ptr, size);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmReconstructHeap(char *json) {
        delete heap;
        heap = new NextFitHeap(1024, json);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmResetHeap() {
        delete heap;
        heap = new NextFitHeap(1024);
    }

}
