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
        locateFile: () => "/heap.wasm"
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
    console.log("test")
    const serializedHeap = instance._getHeap();
    const jsonString = instance.UTF8ToString(serializedHeap);
    const json = JSON.parse(jsonString);
    setBlocks(json.map((block: any) => {return new MemoryBlock(block.id, block.size, block.status as Status)}));
  }
  return (
    <div className="flex flex-col h-screen w-full bg-slate-900  ">
        <Header size={heapSize}></Header>
        <Heap size={heapSize} blocks={blocks}></Heap>
    </div>
  )
}

export default App
