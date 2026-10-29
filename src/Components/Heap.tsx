import {useState} from 'react';
import {MemoryBlock, formatBytes} from './MemoryBlock';
import {MemoryBlockUI} from './MemoryBlockUI.tsx';
import {SelectedBlockPanel} from './SelectedBlockPanel';

type Zone = {label: string, size: number};

function getZones(blocks: MemoryBlock[]): Zone[] {
    const zones: Zone[] = [];
    blocks.forEach((block: MemoryBlock) => {
        if (block.zone === undefined) {
            return;
        }
        const last = zones[zones.length - 1];
        if (last !== undefined && last.label === block.zone) {
            last.size += block.size;
        }
        else {
            zones.push({label: block.zone, size: block.size});
        }
    });
    return zones;
}

export default function Heap({size, blocks} : {size: number, blocks: MemoryBlock[]}) {
    const [selectedBlock, setSelectedBlock] = useState<MemoryBlock | null>(null);
    const [position, setPosition] = useState<{x: number, y:number}>({x: 0, y: 0});
    const zones = getZones(blocks);
    const onHover = (e: React.MouseEvent<HTMLDivElement>, block : MemoryBlock) => {
        const rec = e.currentTarget.getBoundingClientRect();
        // Keep the 320px wide panel inside the window for blocks near the edges.
        const half = 160 + 8;
        const x = Math.min(Math.max(rec.left + rec.width / 2, half), window.innerWidth - half);
        setPosition({x, y: rec.bottom});
        setSelectedBlock(block);

    }
    const onExit = () => {
        setSelectedBlock(null);
    }
    return (
        <div className="flex flex-col w-full">
            <div className="flex w-full h-12 bg-slate-900 rounded-lg border-2 border-slate-100 overflow-hidden shadow-2xl">
                {blocks.map((block: MemoryBlock, index: number) => {
                    const zoneStart = index > 0 && block.zone !== undefined && block.zone !== blocks[index - 1].zone;
                    return (
                       <MemoryBlockUI key={block.id} block={block} total={size} zoneStart={zoneStart} onHover={onHover} onExit={onExit}/>
                    );
                })}
            </div>
            {zones.length > 0 &&
                <div className="flex w-full text-xs text-slate-400 font-mono mt-1">
                    {zones.map((zone: Zone) => (
                        <div key={zone.label} style={{width: `${(zone.size / size) * 100}%`}} className="text-center truncate border-l border-slate-600 first:border-l-0">
                            {zone.label === 'LARGE' ? `large · ${formatBytes(zone.size)}` : `${zone.label} B`}
                        </div>
                    ))}
                </div>
            }
            {selectedBlock !== null &&
                <>
                    {/* On phones the panel is a bottom sheet over a backdrop that closes it. */}
                    <div className="fixed inset-0 z-40 bg-black/50 sm:hidden" onClick={onExit}></div>
                    <div style={{'--x': `${position.x}px`, '--y': `${position.y + 4}px`} as React.CSSProperties}
                        className="fixed inset-x-0 bottom-0 z-50 p-3 sm:p-0 sm:inset-x-auto sm:bottom-auto sm:left-(--x) sm:top-(--y) sm:-translate-x-1/2">
                        <SelectedBlockPanel block={selectedBlock} maxSize={size} onHover={onHover} onExit={onExit}/>
                    </div>
                </>
            }
        </div>
        
    )   
}
