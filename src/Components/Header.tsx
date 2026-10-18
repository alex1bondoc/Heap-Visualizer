import {useState, useEffect} from 'react';

const MIN_HEAP_SIZE = 64;
const MAX_HEAP_SIZE = 1 << 20;

export default function Header({size, resize} : {size: number, resize: (size: number) => void}) {
    const [input, setInput] = useState<string>(String(size));
    useEffect(() => {
        setInput(String(size));
    }, [size]);
    const handleChange = (event: any) => {
        setInput(event.target.value.replace(/\D/g, ''));
    }
    const apply = () => {
        // Heap sizes are kept 8-byte aligned, like the allocations.
        const value = Math.min(Math.max((Number(input) + 7) & ~7, MIN_HEAP_SIZE), MAX_HEAP_SIZE);
        setInput(String(value));
        resize(value);
    }
    return (
        <div className="flex flex-row w-full bg-slate-800 h-13 items-center justify-between">
            <h1 className="text-2xl font-bold text-white m-5 me-">Memory Allocator</h1>
            <div className="flex flex-row items-center gap-2 m-5">
                <span className="text-white font-bold">Heap size</span>
                <input value={input} onChange={handleChange} onKeyDown={(e) => {if (e.key === 'Enter') apply();}}
                    className="font-mono px-3 h-9 w-28 bg-slate-700 text-white border border-slate-600 outline-none focus:ring-2 focus:ring-amber-400 rounded-lg text-right"></input>
                <span className="text-white">bytes</span>
                <button className="bg-amber-400 hover:bg-amber-600 h-9 px-4 rounded-lg font-bold transition-colors"
                    title="Changing the size resets every heap" onClick={apply}>Apply</button>
            </div>
        </div>
    )
}
