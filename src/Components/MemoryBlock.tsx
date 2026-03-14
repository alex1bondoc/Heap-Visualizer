export class MemoryBlock {
    public id: string;
    public status: 'FREE' | 'ALLOC';
    public size: number;

    public constructor(id: string, size: number, status: any) {
        this.id = id;
        this.size = size;
        this.status = status;
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
        )
    }
    public toString() {
        var string = ''
        this.status == 'FREE' ? string += "FREE: " : string += "ALLOC: "
        string += this.size
        return string
    }
}
