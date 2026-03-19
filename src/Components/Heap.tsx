import {useState, useEffect} from 'react';
import {MemoryBlock} from './MemoryBlock';
import {MemoryBlockUI} from './MemoryBlockUI.tsx';
import {SelectedBlockPanel} from './SelectedBlockPanel';

export default function Heap({size, blocks} : {size: number, blocks: MemoryBlock[]}) {
    const [selectedBlock, setSelectedBlock] = useState<MemoryBlock | null>(null);
    const onHover = (block: MemoryBlock) => {
        console.log(block.id + "enter");
        setSelectedBlock(block);
    }
    const onExit = (block: MemoryBlock) => {
        console.log(block.id + "exit")
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
            {selectedBlock !== null && <SelectedBlockPanel block={selectedBlock}/>}
        </div>
        
    )   
}
