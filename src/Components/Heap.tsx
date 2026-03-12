import {useState, useEffect} from 'react';
import {MemoryBlock} from './MemoryBlock';

const blocks: MemoryBlock[] = [
    new MemoryBlock(1, 256, 'ALLOC'), 
    new MemoryBlock(2, 256, 'FREE'),
    new MemoryBlock(1, 256, 'ALLOC'), 
    new MemoryBlock(1, 256, 'FREE'), 
]

export default function Heap({size} : {size: number}) {
    const [head, setHead] = useState(blocks)
    useEffect(() => {
        setHead(blocks)    
    }, [blocks])

    return (
        <div className="flex flex-col gap-2 w-9/10 max-width-9xl mx-auto p-10">
            <div className="flex w-full h-24 bg-slate-900 rounded-xl border-4 border-slate-100 overflow-hidden shadow-2xl">
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
