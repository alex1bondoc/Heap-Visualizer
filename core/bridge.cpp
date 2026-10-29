#include <string>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#else
    #define EMSCRIPTEN_KEEPALIVE
#endif
#include "Manager.h"
#include "HeapFactory.h"

int heap_size = 1024;
Manager* manager = nullptr;

bool validIndex(const int index) {
    return manager != nullptr && index >= 0 && index < (int)manager->getHeaps().size();
}

MemoryBlock *toBlock(const char *address) {
    unsigned long adressNumerical = std::stoul(address, nullptr, 16);
    return reinterpret_cast<MemoryBlock *>(adressNumerical);
}

extern "C" {
    
    EMSCRIPTEN_KEEPALIVE
    const void wasmCreateManager() {
        if (manager == nullptr) {
            manager = new Manager(heap_size);
        }
    }

    EMSCRIPTEN_KEEPALIVE
    const void wasmAddHeap(const char* type, const char* config) {
        wasmCreateManager();
        manager->addHeap(std::string(type), std::string(config));
    }

    EMSCRIPTEN_KEEPALIVE
    const void wasmDeleteHeap(const int index) {
        if (!validIndex(index)) {
            return;
        }
        manager->deleteHeap(index);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmSetHeapSize(const int size) {
        wasmCreateManager();
        heap_size = size;
        manager->setHeapSize(size);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmClearHeaps() {
        wasmCreateManager();
        manager->clearHeaps();
    }
    EMSCRIPTEN_KEEPALIVE
    const char* wasmGetHeaps() {
        wasmCreateManager();
        static std::string json;
        json = manager->serialize();
        return json.c_str();
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmMalloc(const int index, const int size) {
        if (!validIndex(index)) {
            return;
        }
        manager->getHeaps()[index]->myMalloc(size);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmFree(const int index, const char *address) {
        if (!validIndex(index) || address == nullptr) {
            return;
        }
        manager->getHeaps()[index]->myFree(toBlock(address));
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmRealloc(const int index, const char *address, const int size) {
        if (!validIndex(index) || address == nullptr) {
            return;
        }
        manager->getHeaps()[index]->myRealloc(toBlock(address), size);
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmReconstructHeap(const char *type, const char *config, char *json) {
        wasmCreateManager();
        Heap* reconstructed = HeapFactory::createHeap(std::string(type), heap_size, json, std::string(config));
        if (reconstructed != nullptr) {
            manager->getHeaps().push_back(reconstructed);
        }
    }
    EMSCRIPTEN_KEEPALIVE
    const void wasmResetHeap(const int index) {
        if (!validIndex(index)) {
            return;
        }
        std::vector<Heap *>& heaps = manager->getHeaps();
        std::string type = heaps[index]->getType();
        std::string config = heaps[index]->getConfig();
        delete heaps[index];
        heaps[index] = HeapFactory::createHeap(type, heap_size, config);
    }

}
