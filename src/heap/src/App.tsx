import React from 'react';
import './App.css';

function App() {
  return (
    <div className="min-h-screen bg-orange-600 flex items-center justify-center">
      <div className="bg-white p-10 rounded-3xl shadow-2xl transform hover:rotate-6 transition-transform">
        <h1 className="text-5xl font-black text-slate-900 italic">
          TAILWIND MERGE! 🚀
        </h1>
        <p className="text-orange-600 font-mono mt-4 text-center">
          Daca vezi portocaliu, ai invins sistemul.
        </p>
      </div>
    </div>
  );
}
export default App;
