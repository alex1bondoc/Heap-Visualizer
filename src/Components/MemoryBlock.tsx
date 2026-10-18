export type Status = 'FREE' | 'ALLOC';
import {useState} from 'react';
import {SelectedBlockPanel} from './SelectedBlockPanel';
export class MemoryBlock {
    public id: string;
    public status: Status;
    public size: number;
    public zone?: string;
    public constructor(id: string, size: number, status: Status, zone?: string) {
        this.id = id;
        this.size = size;
        this.status = status;
        this.zone = zone;
    }
    public getPercentage(total: number) :number {
        return (this.size / total) * 100;
    }

    public component({total} : {total: number}) {
        return (
            <div 
                key={this.id} 
                style={{ width: `${this.getPercentage(total)}%` }}
                className={`h-full flex flex-col items-center justify-center border-r border-slate-700 transition-all ${
                    this.status === 'FREE' ? 'bg-emerald-500/30' : 'bg-red-500/30'
                }`}
            >
            </div>
        );
    }
    public toString() {
        var string = ''
        this.status == 'FREE' ? string += "FREE: " : string += "ALLOC: "
        string += this.size
        return string
    }
}


export function formatBytes(bytes: number) {
    if (bytes >= 1 << 20 && bytes % (1 << 20) === 0) {
        return `${bytes / (1 << 20)} MiB`;
    }
    if (bytes >= 1 << 10 && bytes % (1 << 10) === 0) {
        return `${bytes / (1 << 10)} KiB`;
    }
    return `${bytes} B`;
}
