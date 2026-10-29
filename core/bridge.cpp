#include <memory>
#include <optional>
#include <string>
#include <vector>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#else
    #define EMSCRIPTEN_KEEPALIVE
#endif
#include "BlockInfo.h"
#include "HeapFactory.h"
#include "Manager.h"

Manager manager{1024};

MemoryBlock *toBlock(const char *address) {
    unsigned long adressNumerical = std::stoul(address, nullptr, 16);
    return reinterpret_cast<MemoryBlock *>(adressNumerical);
}

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void wasmAddHeap(const char *type, const char *config) {
        manager.addHeap(type, config);
    }
    EMSCRIPTEN_KEEPALIVE
    void wasmDeleteHeap(const int index) {
        manager.deleteHeap(index);
    }
    EMSCRIPTEN_KEEPALIVE
    void wasmSetHeapSize(const int size) {
        manager.setHeapSize(size);
    }
    EMSCRIPTEN_KEEPALIVE
    void wasmClearHeaps() {
        manager.clearHeaps();
    }
    EMSCRIPTEN_KEEPALIVE
    const char *wasmGetHeaps() {
        static std::string json;
        json = manager.serialize();
        return json.c_str();
    }
    EMSCRIPTEN_KEEPALIVE
    void wasmMalloc(const int index, const int size) {
        if (Heap *heap = manager.heap(index)) {
            heap->myMalloc(size);
        }
    }
    EMSCRIPTEN_KEEPALIVE
    void wasmFree(const int index, const char *address) {
        if (Heap *heap = manager.heap(index); heap != nullptr && address != nullptr) {
            heap->myFree(toBlock(address));
        }
    }
    EMSCRIPTEN_KEEPALIVE
    void wasmRealloc(const int index, const char *address, const int size) {
        if (Heap *heap = manager.heap(index); heap != nullptr && address != nullptr) {
            heap->myRealloc(toBlock(address), size);
        }
    }
    EMSCRIPTEN_KEEPALIVE
    void wasmReconstructHeap(const char *type, const char *config, const char *json) {
        std::optional<std::vector<BlockInfo>> blocks = parseBlocks(json);
        if (!blocks) {
            return;
        }
        std::unique_ptr<Heap> reconstructed = HeapFactory::createHeap(type, manager.getHeapSize(), *blocks, config);
        if (reconstructed != nullptr) {
            manager.addHeap(std::move(reconstructed));
        }
    }
    EMSCRIPTEN_KEEPALIVE
    void wasmResetHeap(const int index) {
        manager.resetHeap(index);
    }

}
