import {useState} from 'react';
import type {HeapType} from "./HeapCard";
import {heapTypeLabels} from "./HeapCard";

export function AddHeapForm({addHeap}: {addHeap: (type: HeapType) => void}) {
    const [type, setType] = useState<HeapType>('BEST_FIT');
    return (
        <div className="flex flex-row w-full max-w-5xl mx-auto items-center justify-center gap-2 border-2 border-dashed border-slate-600 rounded-xl p-3 transition-all hover:border-slate-100">
            <select value={type} onChange={(e) => setType(e.target.value as HeapType)}
                className="h-10 px-3 bg-slate-700 text-white border border-slate-600 rounded-xl outline-none focus:ring-2 focus:ring-amber-400">
                {(Object.keys(heapTypeLabels) as HeapType[]).map((key) => (
                    <option key={key} value={key}>{heapTypeLabels[key]}</option>
                ))}
            </select>
            <button className="bg-emerald-500 hover:bg-emerald-700 h-10 px-4 rounded-xl font-bold transition-colors" onClick={() => addHeap(type)}>Add Heap</button>
        </div>
    );
}
