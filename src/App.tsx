import Header from "./Components/Header";
import { MemoryBlock } from "./Components/MemoryBlock";
import type { Status } from "./Components/MemoryBlock";
import { useState, useEffect} from "react";
// @ts-ignore
import createHeapModule from "./heap.js";
import {HeapCard} from "./Components/HeapCard";
import type {HeapType} from "./Components/HeapCard";
import {AddHeapForm} from "./Components/AddHeapForm";

type HeapState = {
    type: HeapType;
    blocks: MemoryBlock[];
};

function App() {
    const [heapSize, setHeapSize] = useState<number>(() => {
        const saved = Number(localStorage.getItem("heapSize"));
        return saved > 0 ? saved : 1024;
    });
    const [heaps, setHeaps] = useState<HeapState[]>([]);
    const [wasmInstance, setWasmInstance] = useState<any>(null);

    useEffect(() => {
        createHeapModule({
        locateFile: () => `${import.meta.env.BASE_URL}heap.wasm`,
        }).then((instance: any) => {
            setWasmInstance(instance);
        });
    }, []);
    useEffect(() => {
        if (wasmInstance === null) {
            return;
        }
        getOldHeaps();
        refreshHeaps();
    }, [wasmInstance]);
    const getOldHeaps = () => {
        if (wasmInstance === null) {
            return;
        }
        wasmInstance.ccall("wasmClearHeaps", null, [], []);
        wasmInstance.ccall("wasmSetHeapSize", null, ["number"], [heapSize]);
        const saved = localStorage.getItem("heaps");
        if (saved !== null) {
            const savedHeaps = JSON.parse(saved);
            // Heaps saved with a different heap size cannot be rebuilt.
            const validHeaps = savedHeaps.filter((heap: any) =>
                heap.blocks.reduce((total: number, block: any) => total + block.size, 0) === heapSize);
            validHeaps.forEach((heap: any) => {
                wasmInstance.ccall("wasmReconstructHeap", null, ["string", "string"], [heap.type, JSON.stringify(heap.blocks)]);
            });
            if (savedHeaps.length === 0 || validHeaps.length > 0) {
                return;
            }
        }
        const oldBlocks = localStorage.getItem("blocks");
        if (oldBlocks !== null) {
            wasmInstance.ccall("wasmReconstructHeap", null, ["string", "string"], ["BEST_FIT", oldBlocks]);
            localStorage.removeItem("blocks");
            return;
        }
        wasmInstance.ccall("wasmAddHeap", null, ["string"], ["BEST_FIT"]);
    }
    const refreshHeaps = () => {
        if (wasmInstance === null) {
            return;
        }
        const jsonString = wasmInstance.ccall("wasmGetHeaps", "string", [], []);
        const json = JSON.parse(jsonString);
        setHeaps(json.map((heap: any) => ({
            type: heap.type as HeapType,
            blocks: heap.blocks.map((block: any) => new MemoryBlock(block.id, block.size, block.status as Status, block.zone)),
        })));
        localStorage.setItem("heaps", jsonString);
    };
    const resizeHeaps = (size: number) => {
        if (wasmInstance === null || size === heapSize) {
            return;
        }
        wasmInstance.ccall("wasmClearHeaps", null, [], []);
        wasmInstance.ccall("wasmSetHeapSize", null, ["number"], [size]);
        heaps.forEach((heap: HeapState) => {
            wasmInstance.ccall("wasmAddHeap", null, ["string"], [heap.type]);
        });
        setHeapSize(size);
        localStorage.setItem("heapSize", String(size));
        refreshHeaps();
    };
    const addHeap = (type: HeapType) => {
        if (wasmInstance === null) {
            return;
        }
        wasmInstance.ccall("wasmAddHeap", null, ["string"], [type]);
        refreshHeaps();
    };
    const deleteHeap = (index: number) => {
        if (wasmInstance === null) {
            return;
        }
        wasmInstance.ccall("wasmDeleteHeap", null, ["number"], [index]);
        refreshHeaps();
    };
    const malloc = (index: number, size: number) => {
        if (wasmInstance === null) {
        return;
        }
        wasmInstance.ccall("wasmMalloc", null, ["number", "number"], [index, size]);
        refreshHeaps();
    };
    const free = (index: number, address: string) => {
        if (wasmInstance === null) {
        return;
        }
        wasmInstance.ccall("wasmFree", null, ["number", "string"], [index, address]);
        refreshHeaps();
    };
    const realloc = (index: number, address: string, size: number) => {
        if (wasmInstance === null) {
            return;
        }
        wasmInstance.ccall("wasmRealloc", null, ["number", "string", "number"], [index, address, size]);
        refreshHeaps();
    };
    const reset = (index: number) => {
        if (wasmInstance === null) {
            return;
        }
        wasmInstance.ccall("wasmResetHeap", null, ["number"], [index]);
        refreshHeaps();
    }
    return (
        <div className="flex flex-col h-screen w-full bg-slate-900">
            <Header size={heapSize} resize={resizeHeaps}></Header>
            <div className="flex flex-col w-full flex-1 overflow-y-auto p-4 gap-1">
                {heaps.map((heap: HeapState, index: number) => (
                    <HeapCard
                        key={index}
                        index={index}
                        type={heap.type}
                        size={heapSize}
                        blocks={heap.blocks}
                        malloc={(size: number) => malloc(index, size)}
                        free={(address: string) => free(index, address)}
                        realloc={(address: string, size: number) => realloc(index, address, size)}
                        reset={() => reset(index)}
                        erase={() => deleteHeap(index)}
                    />
                ))}
                <AddHeapForm addHeap={addHeap}></AddHeapForm>
            </div>
        </div>
    );
}

export default App;
