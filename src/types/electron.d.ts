// Type definitions for Electron API exposed via preload
interface ElectronAPI {
  startServer: () => Promise<{ success: boolean; port: number }>;
  isElectron: () => boolean;
}

declare global {
  interface Window {
    electronAPI?: ElectronAPI;
  }
}

export {};
