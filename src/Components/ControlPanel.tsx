import {useContext, useState} from 'react';
import {MallocFunctionContext, ResetFunctionContext} from "./Contexts";

export function ControlPanel({maxSize}: {maxSize: number}) {
    const [size, setSize] = useState(0)
    const malloc = useContext(MallocFunctionContext);
    const reset = useContext(ResetFunctionContext);
    const handleChange = (event: any) => {
        const value = event.target.value.replace(/\D/g, '');
        if (Number(value) > maxSize) {
            setSize(maxSize);
        }
        else 
            setSize(Number(value));
    }
    return (
        <div className="flex flex-row flex-wrap w-full items-center justify-center gap-2">
            <input placeholder="Bytes..." onChange={handleChange} value={size} className="font-mono px-3 h-9 w-28 transition-all bg-slate-700 text-white border border-slate-600 outline-none focus:ring-2 focus:ring-amber-400 rounded-lg text-right"></input>
            <button className="bg-blue-400 hover:bg-blue-600 h-9 px-4 rounded-lg transition-colors" onClick={() => malloc(size)}>Malloc</button>
            <button className="bg-red-400 hover:bg-red-600 h-9 px-4 rounded-lg transition-colors" onClick={() => reset()}>Reset Heap</button>
        </div>
    );
}
