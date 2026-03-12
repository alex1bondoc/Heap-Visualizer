import { useState } from 'react'
import Heap from './Components/Heap'
import Header from './Components/Header'
function App() {
  const [heapSize, setHeapSize] = useState(1024)    
  return (
    <div className="flex flex-col h-screen w-full bg-slate-900  ">
        <Header size={heapSize}></Header>
        <Heap size={heapSize}></Heap>
    </div>
  )
}

export default App
