import Heap from './Components/Heap';
import Header from './Components/Header';
import { MemoryBlock } from './Components/MemoryBlock';
import type { Status } from './Components/MemoryBlock';
import {useState, useEffect} from 'react';
// @ts-ignore
import createHeapModule from './heap.js';

function App() {
    const heapSize = 1024
    const [mallocSize, setMallocSize] = useState<number>(0);
    const [blocks, setBlocks] = useState<MemoryBlock[]>([])
    const [wasmInstance, setWasmInstance] = useState<any>(null)
    const [address, setAddress] = useState<string>('')
    useEffect(() => {
        createHeapModule({
            locateFile: () => "heap.wasm"
        }).then((instance: any) => {
            setWasmInstance(instance);
            refreshBlocks(instance);
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
        const serializedHeap = instance.ccall('wasmGetHeap', ['string'], [], []);
        const jsonString = instance.UTF8ToString(serializedHeap);
        const json = JSON.parse(jsonString);
        setBlocks(json.map((block: any) => {return new MemoryBlock(block.id, block.size, block.status as Status)}));
    }
    const malloc = (instance: any) => {      
        if (instance === null) {
            return;
        }
        instance.ccall('wasmMalloc', null, ['number'], [mallocSize]);
        refreshBlocks(instance);
    }
    const free = (instance: any, hexAddress: string) => {
        if (instance === null) {
            return;
        }
        instance.ccall('wasmFree', null, ['string'], [hexAddress]);
        refreshBlocks(instance);
    }
    const realloc = (instance: any, hexAddress: string, newSize: number) => {
        if (instance === null) {
            return;
        }
        instance.ccall('wasmRealloc', null, ['string', 'number'], [hexAddress, newSize]);
        refreshBlocks(instance);
    }
    const handleChange = (event: any) => {
        const value = event.target.value;
        setMallocSize(value);
    }
    const handleAddressChange = (event: any) => {
        const value = event.target.value;
        setAddress(value);
    }
    
    return (
        <div className="flex flex-col h-screen w-full bg-slate-900  ">
            <Header size={heapSize}></Header>
            <Heap size={heapSize} blocks={blocks}></Heap>
            <button onClick={() => malloc(wasmInstance)} className="border-2 bg-amber-400 h-12 w-20 rounded-xl">malloc</button>
            <button onClick={() => free(wasmInstance, address)} className="border-2 bg-amber-400 h-12 w-20 rounded-xl">Free</button>
            <button onClick={() => realloc(wasmInstance, address, mallocSize)} className="border-2 bg-amber-400 h-12 w-20 rounded-xl">Realloc</button>
            <input className="h-12 w-25 bg-white rounded-xl" value={mallocSize} onChange={handleChange} ></input>
            <input className="h-12 w-25 bg-white rounded-xl" value={address} onChange={handleAddressChange} ></input>
        </div>
    )
}

export default App
