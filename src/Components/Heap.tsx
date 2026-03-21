import {useState, useEffect} from 'react';
import {MemoryBlock} from './MemoryBlock';
import {MemoryBlockUI} from './MemoryBlockUI.tsx';
import {SelectedBlockPanel} from './SelectedBlockPanel';

export default function Heap({size, blocks} : {size: number, blocks: MemoryBlock[]}) {
    const [selectedBlock, setSelectedBlock] = useState<MemoryBlock | null>(null);
    const [position, setPosition] = useState<{x: number, y:number}>({x: 0, y: 0});
    const onHover = (e: React.MouseEvent<HTMLDivElement>, block : MemoryBlock) => {
        const rec = e.currentTarget.getBoundingClientRect();
        setPosition({x: rec.left + rec.width / 2, y: rec.bottom});
        setSelectedBlock(block);

    }
    const onExit = () => {
        setSelectedBlock(null);
    }
    return (
        <div className="flex flex-col gap-2 w-9/10 max-width-9xl mx-auto p-10">
            <div className="flex w-full h-24 bg-slate-900 rounded-xl border-4 border-slate-100 overflow-hidden shadow-2xl">
                {blocks.map((block: MemoryBlock, index: number) => {
                    return (
                       <MemoryBlockUI key={block.id} block={block} total={size} onHover={onHover} onExit={onExit}/>
                    );
                })}
            </div>
            {selectedBlock !== null && 
                <div style={{position: 'fixed', left: `${position.x}px`, top: `${position.y + 4}px`}} className="z-50 -translate-x-1/2">
                    <SelectedBlockPanel block={selectedBlock} onHover={onHover} onExit={onExit}/>
                </div>
            }
        </div>
        
    )   
}
