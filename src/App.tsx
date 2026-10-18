import Header from "./Components/Header";
import { MemoryBlock } from "./Components/MemoryBlock";
import type { Status } from "./Components/MemoryBlock";
import { useState, useEffect} from "react";
// @ts-ignore
import createHeapModule from "./heap.js";
import {HeapCard} from "./Components/HeapCard";
import type {HeapType} from "./Components/HeapCard";
import {AddHeapForm, configSize} from "./Components/AddHeapForm";

type HeapState = {
    type: HeapType;
    config: string;
    blocks: MemoryBlock[];
};

// Older saves have no config, but every segregated block knows its zone,
// so the "class:zoneSize" list can be rebuilt from consecutive zones.
function configFromZones(blocks: any[]) {
    const zones: {label: string, size: number}[] = [];
    blocks.forEach((block: any) => {
        const last = zones[zones.length - 1];
        if (last !== undefined && last.label === block.zone) {
            last.size += block.size;
        }
        else {
            zones.push({label: block.zone, size: block.size});
        }
    });
    return zones.filter((zone) => zone.label !== 'LARGE').map((zone) => `${zone.label}:${zone.size}`).join(',');
}

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
                const config = heap.config ?? (heap.type === 'SEGREGATED' ? configFromZones(heap.blocks) : '');
                wasmInstance.ccall("wasmReconstructHeap", null, ["string", "string", "string"], [heap.type, config, JSON.stringify(heap.blocks)]);
            });
            if (savedHeaps.length === 0 || validHeaps.length > 0) {
                return;
            }
        }
        const oldBlocks = localStorage.getItem("blocks");
        if (oldBlocks !== null) {
            wasmInstance.ccall("wasmReconstructHeap", null, ["string", "string", "string"], ["BEST_FIT", "", oldBlocks]);
            localStorage.removeItem("blocks");
            return;
        }
        wasmInstance.ccall("wasmAddHeap", null, ["string", "string"], ["BEST_FIT", ""]);
    }
    const refreshHeaps = () => {
        if (wasmInstance === null) {
            return;
        }
        const jsonString = wasmInstance.ccall("wasmGetHeaps", "string", [], []);
        const json = JSON.parse(jsonString);
        setHeaps(json.map((heap: any) => ({
            type: heap.type as HeapType,
            config: heap.config,
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
            // A segregated layout that no longer fits falls back to the default one.
            const config = configSize(heap.config) <= size ? heap.config : '';
            wasmInstance.ccall("wasmAddHeap", null, ["string", "string"], [heap.type, config]);
        });
        setHeapSize(size);
        localStorage.setItem("heapSize", String(size));
        refreshHeaps();
    };
    const addHeap = (type: HeapType, config: string) => {
        if (wasmInstance === null) {
            return;
        }
        wasmInstance.ccall("wasmAddHeap", null, ["string", "string"], [type, config]);
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
                <AddHeapForm heapSize={heapSize} addHeap={addHeap}></AddHeapForm>
            </div>
        </div>
    );
}

export default App;
