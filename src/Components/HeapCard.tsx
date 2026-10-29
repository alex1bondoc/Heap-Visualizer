import Heap from "./Heap";
import {MemoryBlock} from "./MemoryBlock";
import {ControlPanel} from "./ControlPanel";
import {MallocFunctionContext, FreeFunctionContext, ReallocFunctionContext, ResetFunctionContext} from "./Contexts";

export type HeapType = 'BEST_FIT' | 'FIRST_FIT' | 'NEXT_FIT' | 'BUDDY' | 'SEGREGATED';

export const heapTypeLabels: Record<HeapType, string> = {
    BEST_FIT: 'Best Fit',
    FIRST_FIT: 'First Fit',
    NEXT_FIT: 'Next Fit',
    BUDDY: 'Buddy',
    SEGREGATED: 'Segregated Lists',
};

export function HeapCard({index, type, size, blocks, malloc, free, realloc, reset, erase}: {
    index: number,
    type: HeapType,
    size: number,
    blocks: MemoryBlock[],
    malloc: (size: number) => void,
    free: (address: string) => void,
    realloc: (address: string, size: number) => void,
    reset: () => void,
    erase: () => void,
}) {
    return (
        <div className="flex flex-col w-full max-w-5xl mx-auto bg-slate-800/50 border-2 border-slate-600 rounded-xl p-2 sm:p-3 gap-2 transition-all hover:border-slate-100 hover:shadow-[0_0_15px_rgba(241,245,249,0.35)]">
            <div className="flex flex-row items-center justify-between gap-2">
                <h2 className="text-lg font-bold text-white">Heap #{index + 1} <span className="text-slate-400 font-normal">· {heapTypeLabels[type]}</span></h2>
                <button className="bg-red-500 hover:bg-red-700 h-8 px-3 shrink-0 rounded-lg text-sm font-bold transition-colors" onClick={() => erase()}>Erase Heap</button>
            </div>
            <ReallocFunctionContext value={realloc}>
                <FreeFunctionContext value={free}>
                    <Heap size={size} blocks={blocks}></Heap>
                </FreeFunctionContext>
            </ReallocFunctionContext>
            <ResetFunctionContext value={reset}>
                <MallocFunctionContext value={malloc}>
                    <ControlPanel maxSize={size}></ControlPanel>
                </MallocFunctionContext>
            </ResetFunctionContext>
        </div>
    );
}
