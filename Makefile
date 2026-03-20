compile-test:
	g++ core/main.cpp core/Heap.cpp core/MemoryBlock.cpp -o core/main 
run-test:
	core/main
memory-test:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind.log core/main
clean-test:
	rm -f core/main
activate-wasm: 
	/bin/fish ~/.emsdk/emsdk_env.fish
compile-wasm: activate-wasm
	emcc core/bridge.cpp core/Heap.cpp core/MemoryBlock.cpp \
                                    -o src/heap.js  \
                                    -s WASM=1 \
                                    -s EXPORT_ES6=1 \
                                    -s ENVIRONMENT=web \
                                    -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "UTF8ToString"]' \
                                    -s ALLOW_MEMORY_GROWTH=1 \
                                    -s MODULARIZE=1 \
                                    -s EXPORT_NAME="createHeapModule"
	mv src/heap.wasm public/heap.wasm
