// Type definitions for Electron API exposed via preload
interface ElectronAPI {
  startServer: () => Promise<{ success: boolean; port: number }>;
  isElectron: () => boolean;
  // Configuration persistence
  saveConfig: (filename: string, data: unknown) => Promise<{ success: boolean; error?: string }>;
  loadConfig: (filename: string) => Promise<{ success: boolean; data?: unknown; error?: string }>;
  getConfigPath: () => Promise<string>;
}

declare global {
  interface Window {
    electronAPI?: ElectronAPI;
  }
}

export {};
