export type Status = 'FREE' | 'ALLOC'

export class MemoryBlock {
    public id: number;
    public status: Status;
    public size: number;

    public constructor(id: number, size: number, status: Status) {
        this.id = id;
        this.size = size;
        this.status = status;
    }

    public getPercentage(total: number) :number {
        return (this.size / total) * 100;
    }
}
