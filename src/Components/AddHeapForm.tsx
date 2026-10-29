import {useState, useEffect} from 'react';
import type {HeapType} from "./HeapCard";
import {heapTypeLabels} from "./HeapCard";

type SizeClass = {size: number, zone: number};

// Same default as SegregatedHeap::setDefaultConfig: classes 16, 32, 64, each with an eighth of the heap.
function defaultClasses(heapSize: number): SizeClass[] {
    return [16, 32, 64]
        .map((size: number) => ({size, zone: Math.floor(heapSize / 8 / size) * size}))
        .filter((sizeClass: SizeClass) => sizeClass.zone > 0);
}

function toConfig(classes: SizeClass[]) {
    return classes.map((sizeClass: SizeClass) => `${sizeClass.size}:${sizeClass.zone}`).join(',');
}

// Bytes taken by the class zones of a "class:zoneSize,..." config.
export function configSize(config: string) {
    if (!config) {
        return 0;
    }
    return config.split(',').reduce((total: number, entry: string) => total + Number(entry.split(':')[1]), 0);
}

// Same rules SegregatedHeap::parseConfig checks.
function validate(classes: SizeClass[], heapSize: number): string | null {
    if (classes.length === 0) {
        return 'Add at least one class.';
    }
    for (let i = 0; i < classes.length; ++i) {
        const {size, zone} = classes[i];
        if (size <= 0 || size % 8 !== 0) {
            return `Class ${size} B must be a positive multiple of 8.`;
        }
        if (i > 0 && size <= classes[i - 1].size) {
            return 'Classes must be in increasing order.';
        }
        if (zone < size || zone % size !== 0) {
            return `Zone of class ${size} B must be a multiple of ${size}.`;
        }
    }
    if (configSize(toConfig(classes)) > heapSize) {
        return `Zones take more than the heap (${heapSize} B).`;
    }
    return null;
}

const inputStyle = "font-mono px-2 h-8 w-20 bg-slate-700 text-white border border-slate-600 outline-none focus:ring-2 focus:ring-amber-400 rounded-lg text-right text-base sm:text-sm";

export function AddHeapForm({heapSize, addHeap}: {heapSize: number, addHeap: (type: HeapType, config: string) => void}) {
    const [type, setType] = useState<HeapType>('BEST_FIT');
    const [classes, setClasses] = useState<SizeClass[]>(() => defaultClasses(heapSize));
    useEffect(() => {
        setClasses(defaultClasses(heapSize));
    }, [heapSize]);
    const segregated = type === 'SEGREGATED';
    const error = segregated ? validate(classes, heapSize) : null;
    const largeZone = heapSize - configSize(toConfig(classes));
    const update = (index: number, field: keyof SizeClass, value: string) => {
        const number = Number(value.replace(/\D/g, ''));
        setClasses(classes.map((sizeClass: SizeClass, i: number) => i === index ? {...sizeClass, [field]: number} : sizeClass));
    };
    const addClass = () => {
        const last = classes[classes.length - 1];
        const size = last === undefined ? 16 : last.size * 2;
        setClasses([...classes, {size, zone: size * 2}]);
    };
    return (
        <div className="flex flex-col w-full max-w-5xl mx-auto items-center gap-3 border-2 border-dashed border-slate-600 rounded-xl p-2 sm:p-3 transition-all hover:border-slate-100">
            <div className="flex flex-row items-center justify-center gap-2">
                <select value={type} onChange={(e) => setType(e.target.value as HeapType)}
                    className="h-10 px-3 bg-slate-700 text-white border border-slate-600 rounded-xl outline-none focus:ring-2 focus:ring-amber-400">
                    {(Object.keys(heapTypeLabels) as HeapType[]).map((key) => (
                        <option key={key} value={key}>{heapTypeLabels[key]}</option>
                    ))}
                </select>
                <button className="bg-emerald-500 hover:bg-emerald-700 disabled:bg-slate-600 disabled:cursor-not-allowed h-10 px-4 rounded-xl font-bold transition-colors"
                    disabled={error !== null} onClick={() => addHeap(type, segregated ? toConfig(classes) : '')}>Add Heap</button>
            </div>
            {segregated &&
                <div className="flex flex-col items-center gap-2 text-sm text-slate-200">
                    <div className="grid grid-cols-[auto_auto_auto] items-center gap-x-3 gap-y-2">
                        <span className="text-slate-400">Class (B)</span>
                        <span className="text-slate-400">Zone size (B)</span>
                        <span></span>
                        {classes.map((sizeClass: SizeClass, index: number) => (
                            <div key={index} className="contents">
                                <input className={inputStyle} value={sizeClass.size} onChange={(e) => update(index, 'size', e.target.value)}></input>
                                <input className={inputStyle} value={sizeClass.zone} onChange={(e) => update(index, 'zone', e.target.value)}></input>
                                <button className="bg-red-500 hover:bg-red-700 h-8 w-8 rounded-lg font-bold transition-colors"
                                    onClick={() => setClasses(classes.filter((_, i: number) => i !== index))}>×</button>
                            </div>
                        ))}
                    </div>
                    <div className="flex flex-row gap-2">
                        <button className="bg-slate-600 hover:bg-slate-500 h-8 px-3 rounded-lg transition-colors" onClick={addClass}>+ Class</button>
                        <button className="bg-slate-600 hover:bg-slate-500 h-8 px-3 rounded-lg transition-colors" onClick={() => setClasses(defaultClasses(heapSize))}>Default</button>
                    </div>
                    {error !== null
                        ? <p className="text-red-400">{error}</p>
                        : <p className="text-slate-400">{largeZone > 0 ? `Large zone (requests over ${classes[classes.length - 1].size} B): ${largeZone} B` : 'No large zone: requests over the last class will fail.'}</p>}
                </div>
            }
        </div>
    );
}
