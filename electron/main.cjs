const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const { spawn } = require('child_process');
const http = require('http');

let mainWindow;
let pythonProcess = null;
const SERVER_PORT = 3001;
const PY_DIST_FOLDER = "server"; // Relative to resources/app/
const PY_MODULE = "zt_bridge.py";

// Determine path to python script
// In dev: ./server/zt_bridge.py
// In prod (packaged): resources/app/server/zt_bridge.py
const isDev = !app.isPackaged;
const projectRoot = isDev ? path.join(__dirname, '..') : path.join(process.resourcesPath, 'app');
const scriptPath = path.join(projectRoot, PY_DIST_FOLDER, PY_MODULE);

function log(msg) {
    console.log(`[Electron]: ${msg}`);
}

const fs = require('fs');

function getPythonPath() {
    const possiblePaths = [
        'python3', // In PATH (works in dev/CLI)
        '/opt/homebrew/bin/python3', // Apple Silicon Homebrew
        '/usr/local/bin/python3',    // Intel Homebrew
        '/usr/bin/python3',          // System (might be a stub)
        '/Library/Frameworks/Python.framework/Versions/Current/bin/python3'
    ];

    for (const p of possiblePaths) {
        if (p === 'python3') continue; // Skip checking file existence for PATH command
        if (fs.existsSync(p)) {
            return p;
        }
    }
    // Fallback to 'python3' and hope for the best if explicit paths fail
    return 'python3';
}

function startPythonServer() {
    if (pythonProcess) {
        log('Python server already running');
        return Promise.resolve(true);
    }

    return new Promise((resolve) => {
        const pythonExe = getPythonPath();
        log(`Starting Python server from: ${scriptPath}`);
        log(`Using Python interpreter: ${pythonExe}`);

        // Explicitly set PATH for the child process to include common locations
        const env = { ...process.env };
        env.PATH = `${env.PATH}:/opt/homebrew/bin:/usr/local/bin:/usr/bin:/bin`;

        const ztPath = path.join(projectRoot, 'libs', 'ZAP_Tracking');

        pythonProcess = spawn(pythonExe, [scriptPath, '--zt-path', ztPath], {
            cwd: projectRoot,
            stdio: ['ignore', 'pipe', 'pipe'],
            env: env
        });

        if (pythonProcess.pid) {
            log(`Python process started (PID: ${pythonProcess.pid})`);
        } else {
            log('Failed to start Python process');
            resolve(false);
            return;
        }

        pythonProcess.stdout.on('data', (data) => {
            console.log(`[Python]: ${data}`);
        });

        pythonProcess.stderr.on('data', (data) => {
            console.error(`[Python ERROR]: ${data}`);
        });

        pythonProcess.on('close', (code) => {
            log(`Python process exited with code ${code}`);
            pythonProcess = null;
        });

        // Wait for server to be ready
        const checkServer = () => {
            http.get(`http://localhost:${SERVER_PORT}/health`, (res) => {
                if (res.statusCode === 200) {
                    log('Python server is ready');
                    resolve(true);
                } else {
                    setTimeout(checkServer, 200);
                }
            }).on('error', () => {
                setTimeout(checkServer, 200);
            });
        };

        // Start checking after a brief delay
        setTimeout(checkServer, 500);
    });
}

// IPC handler for starting the server (called from renderer when Master mode is selected)
ipcMain.handle('start-server', async () => {
    log('Received request to start server (Master mode)');
    const success = await startPythonServer();
    return { success, port: SERVER_PORT };
});

// IPC handler to check if we're in Electron
ipcMain.handle('is-electron', () => true);

// ============== Configuration Persistence ==============
// Store configs in app data directory (~/Library/Application Support/PAZ Gimbal Control/)

function getConfigPath(filename) {
    const configDir = app.getPath('userData');
    return path.join(configDir, filename);
}

// Save configuration to file
ipcMain.handle('save-config', async (event, { filename, data }) => {
    try {
        const configPath = getConfigPath(filename);
        fs.writeFileSync(configPath, JSON.stringify(data, null, 2), 'utf-8');
        log(`Config saved: ${filename}`);
        return { success: true };
    } catch (error) {
        log(`Error saving config ${filename}: ${error.message}`);
        return { success: false, error: error.message };
    }
});

// Load configuration from file
ipcMain.handle('load-config', async (event, { filename }) => {
    try {
        const configPath = getConfigPath(filename);
        if (fs.existsSync(configPath)) {
            const data = JSON.parse(fs.readFileSync(configPath, 'utf-8'));
            log(`Config loaded: ${filename}`);
            return { success: true, data };
        }
        return { success: false, error: 'File not found' };
    } catch (error) {
        log(`Error loading config ${filename}: ${error.message}`);
        return { success: false, error: error.message };
    }
});

// Get config directory path
ipcMain.handle('get-config-path', async () => {
    return app.getPath('userData');
});

function killPythonServer() {
    if (pythonProcess) {
        log('Killing Python process...');
        pythonProcess.kill();
        pythonProcess = null;
    }
}

function createWindow() {
    const preloadPath = path.join(__dirname, 'preload.cjs');

    mainWindow = new BrowserWindow({
        width: 1280,
        height: 800,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
            preload: preloadPath
        },
        // Hide window until content is loaded to avoid white flash
        show: false,
        backgroundColor: '#1a1a1a',
        titleBarStyle: 'hiddenInset' // Mac-style application feel
    });

    log(`Preload script: ${preloadPath}`);

    if (isDev) {
        // In dev, load from Vite dev server (user chooses mode in UI)
        const devUrl = 'http://localhost:5173';
        log(`Loading dev URL: ${devUrl}`);
        mainWindow.loadURL(devUrl);
        mainWindow.show();
        mainWindow.webContents.openDevTools();
    } else {
        // In prod, load static files using file:// protocol
        // The mode selector will be shown first, then server started if Master mode
        const indexPath = path.join(projectRoot, 'dist', 'index.html');
        log(`Loading file: ${indexPath}`);
        mainWindow.loadFile(indexPath);
        mainWindow.on('ready-to-show', () => mainWindow.show());
    }

    mainWindow.on('closed', () => {
        mainWindow = null;
    });
}

// App Lifecycle
app.on('ready', createWindow);

app.on('window-all-closed', () => {
    killPythonServer();
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('quit', () => {
    killPythonServer();
});

app.on('activate', () => {
    if (mainWindow === null) {
        createWindow();
    }
});
