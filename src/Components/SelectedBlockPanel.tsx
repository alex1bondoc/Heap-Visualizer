import {MemoryBlock} from "./MemoryBlock";
import {useState, useContext} from 'react';
import {FreeFunctionContext, ReallocFunctionContext} from "./Contexts";
export function SelectedBlockPanel({block}:{block: MemoryBlock}) { 
    const free = useContext(FreeFunctionContext);
    const realloc = useContext(ReallocFunctionContext);
    const [size, setSize] = useState(0);
    const handleChange = (event: any) => {
        const value = event.target.value;
        setSize(value);
    }
    return (
        <div className="w-100 h-25 flex flex-col bg-white">
            <p>Status: {block.status}</p>
            <p>Size: {block.size}</p>
            <button onClick={() => free(block.id)} className="bg-amber-400">Free Block</button>
            <input onChange={handleChange} value={size}></input>
            <button onClick={() =>  realloc(block.id, size)} className="bg-amber-400"></button>
        </div>
    )
}
