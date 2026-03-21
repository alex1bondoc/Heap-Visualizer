import {useContext, useState} from 'react';
import {MallocFunctionContext} from "./Contexts";

export function ControlPanel() {
    const [size, setSize] = useState(0)
    const malloc = useContext(MallocFunctionContext);
    const handleChange = (event: any) => {
        const value = event.target.value.replace(/\D/g, '');
        if (Number(value) > 1024) {
            setSize(1024);
        }
        else 
            setSize(value);
    }
    return (
        <div className="flex flex-row w-full h-full items-center justify-center gap-1">
            <input placeholder="Bytes..." onChange={handleChange} value={size} className="font-mono px-4 h-12 w-32 transition-all bg-slate-700 border-slate-600 outline-none focus:ring-2 focus:ring-amber-400 rounded-xl text-right"></input>
            <button className="border-2 bg-blue-400 h-12 w-20 rounded-xl my-5" onClick={() => malloc(size)}>Malloc</button>
        </div>
    );
}
