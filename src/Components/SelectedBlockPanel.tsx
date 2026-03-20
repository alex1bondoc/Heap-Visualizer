import {MemoryBlock} from "./MemoryBlock";
import {useState, useContext} from 'react';
import {FreeFunctionContext, ReallocFunctionContext} from "./Contexts";
export function SelectedBlockPanel({block, onHover, onExit}:{block: MemoryBlock, onHover:(e: React.MouseEvent<HTMLDivElement>, block: MemoryBlock) => void, onExit:() => void}) { 
    const free = useContext(FreeFunctionContext);
    const realloc = useContext(ReallocFunctionContext);
    const [size, setSize] = useState(0);
    const handleChange = (event: any) => {
        const value = event.target.value;
        setSize(value);
    }
    return (
        <div className="relative w-80 flex flex-col bg-slate-800 border-4 boder-slate-900 rounded-3xl shadow-xl p-6 gap-5 font-sans">
            <button onClick={() => {onExit()}} className="bg-red-800 rounded-4xl w-5">X</button>
            <p className="mx-auto">Status: {block.status}</p>
            <p className="mx-auto">Size: {block.size}</p>
            <button onClick={() => {free(block.id), onExit()}} className="bg-amber-400">Free Block</button>
            <input className="mx-auto" onChange={handleChange} value={size}></input>
            <button onClick={() =>  realloc(block.id, size)} className="bg-amber-400">Realloc</button>
        </div>
    )
}
