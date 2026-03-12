import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import Heap from './Components/Heap'

function App() {
  const [count, setCount] = useState(0)

  return (
    <div>
        <Heap></Heap>
    </div>
  )
}

export default App
