/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        'gimbal-bg': '#0a0a0f',
        'gimbal-panel': '#12121a',
        'gimbal-text': '#e2e8f0',
        'gimbal-text-dim': '#64748b',
        'gimbal-border': '#1e1e2e',
        'gimbal-accent': '#3b82f6',
        'gimbal-accent-hover': '#2563eb',
        'gimbal-success': '#22c55e',
        'gimbal-warning': '#f59e0b',
        'gimbal-error': '#ef4444',
      },
      fontFamily: {
        mono: ['JetBrains Mono', 'Fira Code', 'Consolas', 'monospace'],
      },
    },
  },
  plugins: [],
};
