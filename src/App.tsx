import Heap from "./Components/Heap";
import Header from "./Components/Header";
import { MemoryBlock } from "./Components/MemoryBlock";
import type { Status } from "./Components/MemoryBlock";
import { useState, useEffect} from "react";
// @ts-ignore
import createHeapModule from "./heap.js";

function App() {
    const heapSize = 1024;
    const [mallocSize, setMallocSize] = useState<number>(0);
    const [blocks, setBlocks] = useState<MemoryBlock[]>([]);
    const [wasmInstance, setWasmInstance] = useState<any>(null);
    const [address, setAddress] = useState<string>("");
    useEffect(() => {
        createHeapModule({
        locateFile: () => "heap.wasm",
        }).then((instance: any) => {
            setWasmInstance(instance);
        });
    }, []);
    useEffect(() => {
        if (wasmInstance === null) {
            return;
        }
        getOldBlocks();
        refreshBlocks();
    }, [wasmInstance]);
    const getOldBlocks = () => {
        if (wasmInstance === null) {
            return;
        }
        const saved = localStorage.getItem("blocks");
        console.log(saved);
        if (saved !== null) {
            wasmInstance.ccall("wasmReconstructHeap", [], ["string"], [saved]);
        }
    }
    const refreshBlocks = () => {
        if (wasmInstance === null) {
            return;
        }
        const serializedHeap = wasmInstance.ccall("wasmGetHeap", ["string"], [], []);
        const jsonString = wasmInstance.UTF8ToString(serializedHeap);
        const json = JSON.parse(jsonString);
        setBlocks(json.map((block: any) => {return new MemoryBlock(block.id, block.size, block.status as Status);}));
        localStorage.setItem("blocks", JSON.stringify(json));
        console.log(json);
    };
    const malloc = () => {
        if (wasmInstance === null) {
        return;
        }
        wasmInstance.ccall("wasmMalloc", null, ["number"], [mallocSize]);
        refreshBlocks();
    };
    const free = () => {
        if (wasmInstance === null) {
        return;
        }
        wasmInstance.ccall("wasmFree", null, ["string"], [address]);
        refreshBlocks();
    };
    const realloc = () => {
        if (wasmInstance === null) {
            return;
        }
        wasmInstance.ccall("wasmRealloc", null, ["string", "number"], [address, mallocSize]);
        refreshBlocks();
    };
    const handleChange = (event: any) => {
        const value = event.target.value;
        setMallocSize(value);
    };
    const handleAddressChange = (event: any) => {
        const value = event.target.value;
        setAddress(value);
    };

    return (
        <div className="flex flex-col h-screen w-full bg-slate-900  ">
        <Header size={heapSize}></Header>
        <Heap size={heapSize} blocks={blocks}></Heap>
        <button
            onClick={() => malloc()}
            className="border-2 bg-amber-400 h-12 w-20 rounded-xl"
        >
            malloc
        </button>
        <button
            onClick={() => free()}
            className="border-2 bg-amber-400 h-12 w-20 rounded-xl"
        >
            Free
        </button>
        <button
            onClick={() => realloc()}
            className="border-2 bg-amber-400 h-12 w-20 rounded-xl"
        >
            Realloc
        </button>
        <input
            className="h-12 w-25 bg-white rounded-xl"
            value={mallocSize}
            onChange={handleChange}
        ></input>
        <input
            className="h-12 w-25 bg-white rounded-xl"
            value={address}
            onChange={handleAddressChange}
        ></input>
        </div>
    );
}

export default App;
