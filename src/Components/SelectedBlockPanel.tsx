import {MemoryBlock} from "./MemoryBlock";
import {useState, useContext} from 'react';
import {FreeFunctionContext, ReallocFunctionContext} from "./Contexts";
export function SelectedBlockPanel({block, onHover, onExit}:{block: MemoryBlock, onHover:(block: MemoryBlock) => void, onExit:(block: MemoryBlock) => void}) { 
    const free = useContext(FreeFunctionContext);
    const realloc = useContext(ReallocFunctionContext);
    const [size, setSize] = useState(0);
    const handleChange = (event: any) => {
        const value = event.target.value;
        setSize(value);
    }
    return (
        <div className="w-100 h-35 flex flex-col bg-white border-4" onMouseEnter={() => onHover(block)} onMouseLeave={()=>onExit(block)}>
            <p className="mx-auto">Address: {block.id}</p>
            <p className="mx-auto">Status: {block.status}</p>
            <p className="mx-auto">Size: {block.size}</p>
            <button onClick={() => free(block.id)} className="bg-amber-400">Free Block</button>
            <input className="mx-auto" onChange={handleChange} value={size}></input>
            <button onClick={() =>  realloc(block.id, size)} className="bg-amber-400">Realloc</button>
        </div>
    )
}
