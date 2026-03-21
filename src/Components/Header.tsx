export default function Header({size} : {size: number}) {
    return (
        <div className="flex flex-row w-full bg-slate-800 h-13 items-center justify-between">
            <h1 className="text-2xl font-bold text-white m-5 me-">Memory Allocator</h1>
            <h1 className="text-2xl font-bold text-white m-5">{size} Bytes</h1>
        </div>
    )
}
