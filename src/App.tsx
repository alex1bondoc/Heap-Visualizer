import Heap from './Components/Heap';
import Header from './Components/Header';
import { MemoryBlock } from './Components/MemoryBlock';
import type { Status } from './Components/MemoryBlock';
import {useState, useEffect} from 'react';
// @ts-ignore
import createHeapModule from './heap.js';

function App() {
  const heapSize = 1024
  const [blocks, setBlocks] = useState([])
  useEffect(() => {
    createHeapModule({
        locateFile: () => "/heap.wasm"
    }).then((instance: any) => {
        const serializedString = instance._getHeap(); 
        const jsonString = instance.UTF8ToString(serializedString);
        const json = JSON.parse(jsonString);
        const newBlocks = json.map((block: any) => {
            return new MemoryBlock(block.id, block.size, block.status as Status);
        })
        setBlocks(newBlocks);
    });
  }, []);
  return (
    <div className="flex flex-col h-screen w-full bg-slate-900  ">
        <Header size={heapSize}></Header>
        <Heap size={heapSize} blocks={blocks}></Heap>
    </div>
  )
}

export default App
