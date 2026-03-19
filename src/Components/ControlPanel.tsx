import {useContext, useState} from 'react';
import {MallocFunctionContext} from "./Contexts";

export function ControlPanel() {
    const [size, setSize] = useState(0)
    const malloc = useContext(MallocFunctionContext);
    const handleChange = (event: any) => {
        const value = event.target.value;
        setSize(value)
    }
    return (
        <div className="flex flex-col w-full h-full mx-auto">
            <button className="border-2 bg-amber-400 h-12 w-20 rounded-xl mx-auto my-3" onClick={() => malloc(size)}>Malloc</button>
            <input onChange={handleChange} value={size} className="h-12 w-25 bg-white rounded-xl mx-auto"></input>
        </div>
    );
}
