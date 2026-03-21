import {useContext, useState} from 'react';
import {MallocFunctionContext, ResetFunctionContext} from "./Contexts";

export function ControlPanel() {
    const [size, setSize] = useState(0)
    const malloc = useContext(MallocFunctionContext);
    const reset = useContext(ResetFunctionContext);
    const handleChange = (event: any) => {
        const value = event.target.value.replace(/\D/g, '');
        if (Number(value) > 1024) {
            setSize(1024);
        }
        else 
            setSize(Number(value));
    }
    return (
        <div className="w-full h-30 justify-center items-center flex">
            <div className="flex flex-col w-70 h-30 items-center justify-center bg-slate-800 border-4 boder-slate-900 rounded-3xl shadow-xl p-6 gap-5 font-sans">
                <div className="flex flex-row w-full h-full items-center justify-center gap-1">
                    <input placeholder="Bytes..." onChange={handleChange} value={size} className="font-mono px-4 h-12 w-32 transition-all bg-slate-700 border-slate-600 outline-none focus:ring-2 focus:ring-amber-400 rounded-xl text-right"></input>
                    <button className="border-2 bg-blue-400 h-12 w-20 rounded-xl " onClick={() => malloc(size)}>Malloc</button>
                </div>
                <button className="border-2 bg-red-400 h-12 w-50 rounded-xl " onClick={() => reset()}>Reset-Heap</button>
            </div>
        </div>
    );
}
