import { useState } from 'react'
import Heap from './Components/Heap'
import Header from './Components/Header'
import { MemoryBlock } from './Components/MemoryBlock'
const blocks: MemoryBlock[] = [
    new MemoryBlock(1, 256, 'ALLOC'), 
    new MemoryBlock(2, 256, 'FREE'),
    new MemoryBlock(1, 256, 'ALLOC'), 
    new MemoryBlock(1, 256, 'FREE'), 
]
function App() {
  const [heapSize, setHeapSize] = useState(1024)    
  return (
    <div className="flex flex-col h-screen w-full bg-slate-900  ">
        <Header size={heapSize}></Header>
        <Heap size={heapSize} blocks={blocks}></Heap>
    </div>
  )
}

export default App
