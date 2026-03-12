import React from 'react';
import {useState} from 'react';
import {MemoryBlock} from './MemoryBlock';
import type {Status} from './MemoryBlock';

const blocks: MemoryBlock[] = [
    new MemoryBlock(1, 256, 'ALLOC'), 
    new MemoryBlock(2, 768, 'FREE'),
    new MemoryBlock(1, 256, 'ALLOC'), 
    new MemoryBlock(1, 256, 'ALLOC'), 
]

export default function Heap() {
    const [head, setHead] = useState(blocks)
    const [size, setSize] = useState(1024)
    return (
        <div className="flex flex-col gap-2 w-full max-w-5xl mx-auto p-4">
            <div className="text-slate-400 font-mono text-sm mb-1">
                Heap memory map: {size} bytes
            </div>
            <div className="flex w-full h-24 bg-slate-800 rounded-xl border-2 border-slate-700 overflow-hidden shadow-2xl">
                {head.map((block) => {
                    const width = block.getPercentage(size);
                    return (
                        <div 
                            key={block.id} 
                            style={{ width: `${width}%` }} 
                            className={`h-full flex flex-col items-center justify-center border-r border-slate-700 transition-all ${
                                block.status === 'FREE' ? 'bg-emerald-500/30' : 'bg-red-500/30'
                            }`}
                        >
                            {/* Poți pune ceva text aici mai târziu */}
                        </div>
                    );
                })}
            </div>
        </div>
        
    )   
}
