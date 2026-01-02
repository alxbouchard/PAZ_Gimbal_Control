const { app, BrowserWindow } = require('electron');
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

function startPythonServer() {
    log(`Starting Python server from: ${scriptPath}`);

    // Use system python3. In a fully frozen app we might include a python runtime,
    // but for this hybrid approach we assume python3 is available on the Mac.
    pythonProcess = spawn('python3', [scriptPath], {
        cwd: projectRoot, // Set CWD so python finds libs/ relative to itself
        stdio: ['ignore', 'pipe', 'pipe'] // Pipe stdout/stderr
    });

    if (pythonProcess.pid) {
        log(`Python process started (PID: ${pythonProcess.pid})`);
    } else {
        log('Failed to start Python process');
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
}

function killPythonServer() {
    if (pythonProcess) {
        log('Killing Python process...');
        pythonProcess.kill();
        pythonProcess = null;
    }
}

function createWindow() {
    mainWindow = new BrowserWindow({
        width: 1280,
        height: 800,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
        },
        // Hide window until content is loaded to avoid white flash
        show: false,
        backgroundColor: '#1a1a1a',
        titleBarStyle: 'hiddenInset' // Mac-style application feel
    });

    const startUrl = isDev
        ? 'http://localhost:5173'  // Vite dev server
        : `http://localhost:${SERVER_PORT}`; // Python serving compiled static

    log(`Loading URL: ${startUrl}`);

    // Poll for server readiness before loading
    const checkServer = () => {
        http.get(`http://localhost:${SERVER_PORT}/health`, (res) => {
            if (res.statusCode === 200) {
                mainWindow.loadURL(startUrl);
                mainWindow.on('ready-to-show', () => mainWindow.show());
            } else {
                setTimeout(checkServer, 500);
            }
        }).on('error', () => {
            setTimeout(checkServer, 500);
        });
    };

    if (isDev) {
        // In dev, assuming dev servers are already running or we just load vite
        mainWindow.loadURL(startUrl);
        mainWindow.show();
        mainWindow.webContents.openDevTools();
    } else {
        // In prod, wait for Python server
        startPythonServer();
        checkServer();
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
