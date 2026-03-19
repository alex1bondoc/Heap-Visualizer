import {useState} from 'react';
import {MemoryBlock} from './MemoryBlock';
import {SelectedBlockPanel} from './SelectedBlockPanel';

export function MemoryBlockUI({block, total}: {block: MemoryBlock, total: number}) {
    const [hovered, setHovered] = useState(false);
    return (
        <>
            <div style={{width: `${block.getPercentage(total)}%`}}
            className={`h-full flex flex-col items-center justify-center border-r border-slate-700 transition-all ${
            block.status === 'FREE' ? 'bg-emerald-500/30' : 'bg-red-500/30'}`}
            onMouseEnter={() => setHovered(true)}
            onMouseLeave={() => setHovered(false)}
            >
            </div>
        </>
    )
}
