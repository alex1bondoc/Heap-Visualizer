import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import Heap from './Components/Heap'
import Header from './Components/Header'
function App() {
  const [count, setCount] = useState(0)

  return (
    <div className="flex flex-col h-screen w-full bg-slate-900  ">
        <Header></Header>
        <Heap></Heap>
    </div>
  )
}

export default App
