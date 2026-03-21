import {MemoryBlock} from "./MemoryBlock";
import {useState, useContext} from 'react';
import {FreeFunctionContext, ReallocFunctionContext} from "./Contexts";
export function SelectedBlockPanel({block, onHover, onExit}:{block: MemoryBlock, onHover:(e: React.MouseEvent<HTMLDivElement>, block: MemoryBlock) => void, onExit:() => void}) { 
    const free = useContext(FreeFunctionContext);
    const realloc = useContext(ReallocFunctionContext);
    const [size, setSize] = useState<number | null>(null);
    const handleChange = (event: any) => {
        const value = event.target.value.replace(/\D/g, '');
        if (Number(value) > 1024) {
            setSize(1024);
        }
        else 
            setSize(value);
    }
    return (
        <div className="relative w-80 flex flex-col bg-slate-800 border-4 boder-slate-900 rounded-3xl shadow-xl p-6 gap-5 font-sans">
            <button onClick={() => {onExit()}} 
                className="absolute top-4 right-4 w-5 h-5 flex items-center justify-center
                bg-red-500 hover:bg-red-700 rounded-full font-bold transition-colors"><></></button>
            <p className={`mx-auto ${block.status === 'FREE' ? 'text-emerald-500' : 'text-red-500' }`}>STATUS: {block.status}</p>
            <p className="mx-auto text-slate-100">Size: {block.size} bytes</p>
            { block.status === 'ALLOC' ?
                <button onClick={() => {free(block.id), onExit()}} className="bg-blue-400 mx-auto w-40 h-7 rounded-full">Free Block</button>
            : <></>}
            { block.status === 'ALLOC' ?
                <div className="flex flex-row w-full gap-2 mt-2">
                    <input className="flex-1 w-3/10 bg-slate-700 border border-slate-600 text-white px-3 py-2 rounded-xl outline-none focus:ring-2 focus:ring-amber-400 text-right font-mono transition-all" 
                    onChange={handleChange} 
                    value={size}
                    placeholder="Bytes..."></input> 
                    <button onClick={() =>  {realloc(block.id, size)
                    setSize(0)}} className="w-1/4 bg-blue-400 rounded-xl">Realloc</button>
                </div>
            : <></>}
        </div>
    )
}
