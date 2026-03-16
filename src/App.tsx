import Heap from './Components/Heap';
import Header from './Components/Header';
import { MemoryBlock } from './Components/MemoryBlock';
import type { Status } from './Components/MemoryBlock';
import {useState, useEffect} from 'react';
// @ts-ignore
import createHeapModule from './heap.js';

function App() {
    const heapSize = 1024
    const [blocks, setBlocks] = useState<MemoryBlock[]>([])
    const [wasmInstance, setWasmInstance] = useState<any>(null)
  
    useEffect(() => {
        createHeapModule({
            locateFile: () => "heap.wasm"
        }).then((instance: any) => {
            setWasmInstance(instance);
        });
    }, []);

    useEffect(() => {
        if (wasmInstance === null) {
            return;
        }
        refreshBlocks(wasmInstance);
    }, [wasmInstance]);

    const refreshBlocks = (instance: any) => {
        if (wasmInstance === null) {
            return;
        }
        const serializedHeap = instance._wasmGetHeap();
        const jsonString = instance.UTF8ToString(serializedHeap);
        const json = JSON.parse(jsonString);
        setBlocks(json.map((block: any) => {return new MemoryBlock(block.id, block.size, block.status as Status)}));
    }
    const malloc = (instance: any) => {      
        if (instance === null) {
            return;
        }
        instance._wasmMalloc(128);
        refreshBlocks(instance);
    }
    const free = (instance: any, hexAddres: string) => {
        if (instance === null) {
            return;
        }
        instance._wasmFree(hexAddres);
        refreshBlocks(instance);
    }
    return (
        <div className="flex flex-col h-screen w-full bg-slate-900  ">
            <Header size={heapSize}></Header>
            <Heap size={heapSize} blocks={blocks}></Heap>
            <button onClick={() => malloc(wasmInstance)} className="border-2 bg-amber-400 h-12 w-20 rounded-xl">malloc</button>
        </div>
    )
}

export default App
