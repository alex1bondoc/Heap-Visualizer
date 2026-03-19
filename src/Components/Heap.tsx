import {useState, useEffect} from 'react';
import {MemoryBlock} from './MemoryBlock';
import {MemoryBlockUI} from './MemoryBlockUI.tsx';

export default function Heap({size, blocks} : {size: number, blocks: MemoryBlock[]}) {
    const [head, setHead] = useState(blocks)
    useEffect(() => {
        setHead(blocks)    
    }, [blocks])

    return (
        <div className="flex flex-col gap-2 w-9/10 max-width-9xl mx-auto p-10">
            <div className="flex w-full h-24 bg-slate-900 rounded-xl border-4 border-slate-100 overflow-hidden shadow-2xl">
                {head.map((block) => {
                    return (
                       <MemoryBlockUI key={block.id} block={block} total={size}/>
                    );
                })}
            </div>
        </div>
        
    )   
}
