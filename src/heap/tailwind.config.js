/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        'heap-free': '#10b981',   // Verde smarald
        'heap-used': '#ef4444',   // Roșu intens
        'heap-bg': '#0f172a',     // Slate închis pentru fundal
      }
    },
  },
  plugins: [],
}
