import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import Heap from './Components/Heap'
import Header from './Components/Header'
function App() {
  const [count, setCount] = useState(0)

  return (
    <div className="min-h-screen bg-slate-900 p-10 flex flex-col items-center justify-center font-sans">
        <Header></Header>
        <Heap></Heap>
    </div>
  )
}

export default App
