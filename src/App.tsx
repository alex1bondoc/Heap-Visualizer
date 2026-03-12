function App() {
  return (
    // Container principal: fundal închis, padding, flexbox pentru centrare
    <div className="min-h-screen bg-slate-900 p-10 flex flex-col items-center justify-center font-sans">
      
        {/* Titlu principal: text mare, alb, bold, distanțare inferioară */}
        <h1 className="text-4xl font-extrabold text-white mb-10 tracking-tight">
          Testare Tailwind CSS v4
        </h1>

        {/* Grid pentru cardurile de test: 2 coloane pe ecrane medii, spațiere */}
        <div className="grid grid-cols-1 md:grid-cols-2 gap-6 w-full max-w-4xl">
            
            {/* Card Test 1: Culori și Rotunjiri */}
            <div className="bg-white p-6 rounded-2xl shadow-xl border border-slate-200">
                <h2 className="text-xl font-bold text-slate-900 mb-2">1. Culori și Rotunjiri</h2>
                <p className="text-slate-600 mb-4">Verifică dacă fundalul este alb, colțurile sunt rotunjite accentuat și există o umbră subtilă.</p>
                <div className="flex gap-2">
                    <span className="px-3 py-1 bg-sky-100 text-sky-700 rounded-full text-sm font-medium">Sky</span>
                    <span className="px-3 py-1 bg-emerald-100 text-emerald-700 rounded-full text-sm font-medium">Emerald</span>
                    <span className="px-3 py-1 bg-rose-100 text-rose-700 rounded-full text-sm font-medium">Rose</span>
                </div>
            </div>

            {/* Card Test 2: Dimensiuni și Spațiere */}
            <div className="bg-white p-6 rounded-2xl shadow-xl border border-slate-200">
                <h2 className="text-xl font-bold text-slate-900 mb-2">2. Dimensiuni și Spațiere</h2>
                <p className="text-slate-600 mb-4">Pătratele de mai jos ar trebui să aibă dimensiuni fixe și spațiu egal între ele.</p>
                <div className="flex gap-3">
                    <div className="w-12 h-12 bg-slate-800 rounded-md"></div>
                    <div className="w-12 h-12 bg-slate-600 rounded-md"></div>
                    <div className="w-12 h-12 bg-slate-400 rounded-md"></div>
                    <div className="w-12 h-12 bg-slate-200 rounded-md"></div>
                </div>
            </div>

            {/* Card Test 3: Tipografie */}
            <div className="bg-white p-6 rounded-2xl shadow-xl border border-slate-200 md:col-span-2">
                <h2 className="text-xl font-bold text-slate-900 mb-3">3. Tipografie</h2>
                <p className="text-3xl font-black text-slate-950 leading-tight mb-2">Text FOARTE mare și gros (font-black).</p>
                <p className="text-sm text-slate-500 font-mono bg-slate-100 p-2 rounded">Text mic, gri, font monospațiat (font-mono).</p>
            </div>

        </div>

        {/* Buton de test pentru stări (hover) */}
        <button className="mt-12 px-8 py-3 bg-indigo-600 text-white font-semibold rounded-lg shadow-md hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2 focus:ring-offset-slate-900 transition-colors duration-200">
            Buton cu Hover effect
        </button>

    </div>
  );
}

export default App;
