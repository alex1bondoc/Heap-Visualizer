import Heap from "./Components/Heap";
import Header from "./Components/Header";
import { MemoryBlock } from "./Components/MemoryBlock";
import type { Status } from "./Components/MemoryBlock";
import { useContext, useState, useEffect} from "react";
// @ts-ignore
import createHeapModule from "./heap.js";
import {MallocFunctionContext, FreeFunctionContext, ReallocFunctionContext, ResetFunctionContext} from "./Components/Contexts";
import {ControlPanel} from "./Components/ControlPanel";

function App() {
    const heapSize = 1024;
    const [blocks, setBlocks] = useState<MemoryBlock[]>([]);
    const [wasmInstance, setWasmInstance] = useState<any>(null);

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
    };
    const malloc = (size: number) => {
        if (wasmInstance === null) {
        return;
        }
        wasmInstance.ccall("wasmMalloc", null, ["number"], [size]);
        refreshBlocks();
    };
    const free = (address: string) => {
        if (wasmInstance === null) {
        return;
        }
        wasmInstance.ccall("wasmFree", null, ["string"], [address]);
        refreshBlocks();
    };
    const realloc = (address: string, size: number) => {
        if (wasmInstance === null) {
            return;
        }
        wasmInstance.ccall("wasmRealloc", null, ["string", "number"], [address, size]);
        refreshBlocks();
    };
    const reset = () => {
        if (wasmInstance === null) {
            return;
        }
        wasmInstance.ccall("wasmResetHeap", null, [], []);
        refreshBlocks();
    }
    return (
        <div className="flex flex-col h-screen w-full bg-slate-900 center ">
            <Header size={heapSize}></Header>
            <ReallocFunctionContext value={realloc}>
                <FreeFunctionContext value={free}>
                    <Heap size={heapSize} blocks={blocks}></Heap>
                </FreeFunctionContext>
            </ReallocFunctionContext>
            <ResetFunctionContext value={reset}>
                <MallocFunctionContext value={malloc}>
                    <ControlPanel></ControlPanel>
                </MallocFunctionContext>
            </ResetFunctionContext>
        </div>
    );
}

export default App;
