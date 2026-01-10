const { contextBridge, ipcRenderer } = require('electron');

// Expose protected methods to renderer process
contextBridge.exposeInMainWorld('electronAPI', {
    startServer: () => ipcRenderer.invoke('start-server'),
    isElectron: () => true
});
