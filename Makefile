SHELL := /bin/bash
compile-test:
	g++ -std=c++20 core/main.cpp core/MemoryBlock.cpp core/Heap.cpp core/BestFitHeap.cpp core/HeapFactory.cpp core/FirstFitHeap.cpp core/NextFitHeap.cpp core/Manager.cpp core/BuddyHeap.cpp core/SegregatedHeap.cpp core/BlockInfo.cpp -o core/main 
run-test:
	core/main
memory-test:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind.log core/main
clean-test:
	rm -f core/main
compile-wasm:
	. ~/.emsdk/emsdk_env.sh > /dev/null && em++ -std=c++20 core/bridge.cpp core/MemoryBlock.cpp core/Heap.cpp core/BestFitHeap.cpp core/HeapFactory.cpp core/FirstFitHeap.cpp core/NextFitHeap.cpp core/Manager.cpp core/BuddyHeap.cpp core/SegregatedHeap.cpp core/BlockInfo.cpp\
                                    -o src/heap.js  \
                                    -s WASM=1 \
                                    -s EXPORT_ES6=1 \
                                    -s ENVIRONMENT=web \
                                    -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "UTF8ToString"]' \
                                    -s ALLOW_MEMORY_GROWTH=1 \
                                    -s MODULARIZE=1 \
                                    -s EXPORT_NAME="createHeapModule"
	mv src/heap.wasm public/heap.wasm
