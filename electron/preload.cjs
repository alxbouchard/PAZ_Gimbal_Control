const { contextBridge, ipcRenderer } = require('electron');

// Expose protected methods to renderer process
contextBridge.exposeInMainWorld('electronAPI', {
    startServer: () => ipcRenderer.invoke('start-server'),
    isElectron: () => true,
    // Configuration persistence
    saveConfig: (filename, data) => ipcRenderer.invoke('save-config', { filename, data }),
    loadConfig: (filename) => ipcRenderer.invoke('load-config', { filename }),
    getConfigPath: () => ipcRenderer.invoke('get-config-path'),
});
