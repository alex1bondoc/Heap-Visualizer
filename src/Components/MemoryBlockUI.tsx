import {MemoryBlock} from './MemoryBlock';

export function MemoryBlockUI({block, total, zoneStart, onHover}: {block: MemoryBlock, total: number, zoneStart: boolean, onHover: (e: React.MouseEvent<HTMLDivElement>, block: MemoryBlock) => void, onExit: () => void}) {
    return (
        <div style={{width: `${block.getPercentage(total)}%`, minWidth: '6px'}}
        className={`h-full flex flex-col items-center justify-center border border-slate-700 transition-all ${zoneStart ? 'border-l-2 border-l-slate-100' : ''} ${
        block.status === 'FREE' ? 'bg-emerald-500/30 hover:bg-emerald-800' : 'bg-red-500/30 hover:bg-red-800'}`} 
        onClick={(e) => onHover(e, block)}
        >
        </div>
    )
}
