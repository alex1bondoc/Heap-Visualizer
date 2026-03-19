import {useContext, useState} from 'react';
import {WasmInstanceContext, MallocFunctionContext} from "./Contexts";

export function ControlPanel() {
    const wasmInstace = useContext(WasmInstanceContext);
    const [size, setSize] = useState(0)
    const malloc = useContext(MallocFunctionContext);
    const handleChange = (event: any) => {
        const value = event.target.value;
        setSize(value)
    }
    return (
        <div>
            <input onChange={handleChange} value={size} className="h-12 w-25 bg-white rounded-xl"></input>
            <button className="border-2 bg-amber-400 h-12 w-20 rounded-xl" onClick={() => malloc(size)}>Malloc</button>
        </div>
    );
}
