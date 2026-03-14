declare module '*/heap.js' {
    interface HeapModule {
        _getHeap(): number;
    }
    const createHeapModule: (options: any) => Promise<HeapModule>;
    export default createHeapModule;
}
