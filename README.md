# PAZ Gimbal Control

Professional multi-user control interface for DJI Ronin RS gimbals with ZAP Tracking system integration.

![Version](https://img.shields.io/badge/version-1.5.1-blue)
![Platform](https://img.shields.io/badge/platform-macOS%20ARM64-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)

## Features

- **Multi-User Support** - Multiple operators can control different gimbals simultaneously
- **Master/Client Mode** - One Master hosts the server, Clients connect via network
- **Multi-Gimbal Support** - Control multiple DJI gimbals with instant switching (1, 2, 3, 4...)
- **Controller Visibility** - See who controls which gimbal in real-time
- **3D Virtual Gimbal Visualization** - Real-time 3D model replicating DJI Ronin RS behavior
- **Dual Joystick Controls** - Touch and mouse-friendly virtual joysticks
- **Virtual Gimbal Mode** - Test and demo without physical hardware
- **Real-time Telemetry** - Position graphs and statistics
- **Xbox Gamepad Support** - Full button and axis mapping configuration
- **Keyboard Shortcuts** - Fully customizable keyboard controls
- **DJI Ronin RS Specs** - Accurate 360 deg/s speed limits, mechanical limits, continuous pan rotation
- **Gimbal Manager** - Add, edit, and remove gimbals via EthCAN IP addresses

## Quick Start

### Installation (Zero Setup)

**Requirement:** macOS with Apple Silicon (M1, M2, M3+).

1.  Download the latest `.dmg` from the [Releases Page](https://github.com/alxbouchard/PAZ_Gimbal_Control/releases).
2.  Open the file and drag **PAZ Gimbal Control** to your Applications folder.
3.  Launch the app.

> **Note:** Just right-click > Open the first time if macOS warns about the developer identity.

That's it! No Python or Node.js installation required.

## Multi-User Setup

PAZ Gimbal Control supports multiple operators controlling different gimbals from separate computers on the same network.

### How It Works

1. **Master Computer**: Launches the app and selects **Master** mode
   - Starts the gimbal server
   - Can control gimbals directly
   - Other users connect to this machine

2. **Client Computers**: Launch the app (or browser) and select **Client** mode
   - Enter the Master's IP address (e.g., `192.168.1.50`)
   - Connect to control their assigned gimbals

### Finding the Master's IP Address

On the Master computer, run:
- **macOS/Linux**: `ifconfig` or `ipconfig getifaddr en0`
- **Windows**: `ipconfig`

Look for the local IP (usually starts with `192.168.x.x` or `10.x.x.x`).

### Client Options

Clients can connect in two ways:

1. **Desktop App**: Download and install the app, select "Client" mode
2. **Web Browser**: Open `http://[MASTER-IP]:3001` in any browser

### Controller Visibility

Each gimbal shows who is currently controlling it:
- Your name appears as "User 1", "User 2", etc.
- The gimbal switcher displays the controller's name next to each gimbal
- When a user disconnects, their gimbals become available for others

## Manual Installation (For Developers)

If you want to modify the code:

```bash
# Clone the repository
git clone https://github.com/alxbouchard/PAZ_Gimbal_Control.git
cd PAZ_Gimbal_Control

# Run the setup script (installs Python/Node dependencies)
./setup.sh

# Run in Dev Mode
npm run electron:dev
```

## Project Structure

```
PAZ_Gimbal_Control/
├── src/                    # React frontend
│   ├── components/
│   │   ├── ControlPanel/   # Joysticks, sliders, quick actions
│   │   ├── VirtualGimbal/  # 3D visualization (Three.js)
│   │   ├── Dashboard/      # Telemetry and status
│   │   ├── ConnectionModeSelector.tsx  # Master/Client selection
│   │   └── pages/          # Settings, shortcuts
│   ├── hooks/              # Custom React hooks
│   ├── services/           # WebSocket client
│   └── store/              # Zustand state management
├── server/                 # Python backend
│   └── zt_bridge.py        # WebSocket server + ZT_Agent bridge
├── electron/               # Electron main process
│   ├── main.cjs            # App entry point
│   └── preload.cjs         # IPC bridge
├── libs/                   # Native libraries
│   ├── KMSbase/            # Base library
│   └── ZAP_Tracking/       # Gimbal control library
└── setup.sh                # Installation script
```

## Controls

### Keyboard (Customizable)

| Key | Action |
|-----|--------|
| W/S | Pitch Up/Down |
| A/D | Yaw Left/Right |
| Q/E | Roll Left/Right |
| H | Go Home |
| Shift+H | Set Home |
| T | Toggle Tracking |
| B | Speed Boost |
| Space | Emergency Stop |
| 1-4 | Speed Presets |

### Gamepad (Xbox)

| Input | Action |
|-------|--------|
| Left Stick | Pitch/Yaw |
| Right Stick | Roll/Focus |
| A Button | Toggle Tracking |
| B Button | Go Home |
| X Button | Set Home |
| Y Button | Speed Boost |
| LB/RB | Previous/Next Gimbal |
| LT/RT | Zoom In/Out |

All gamepad mappings can be customized in Settings > Xbox Gamepad.

## DJI Ronin RS Specifications

The virtual gimbal replicates real DJI Ronin RS behavior:

- **Max Speed**: 360 deg/s (all axes)
- **Pitch Range**: -90 deg to +90 deg
- **Roll Range**: -45 deg to +45 deg
- **Pan (Yaw)**: 360 deg continuous rotation
- **Yaw Display**: Normalized to -180 deg to +180 deg

## Running Modes

### Virtual Mode (Demo) - All Platforms
No hardware required. The virtual gimbal simulates all movements.
**Works on any platform (macOS, Linux, Windows).**

```bash
npm run virtual
```

### Hardware Mode - macOS ARM64 Only
Requires ZT_Agent binary and connected DJI gimbal via EthCAN.

```bash
npm start
```

> **Note**: Pre-compiled binaries are included for **macOS ARM64 (Apple Silicon)** only.
> For other platforms (Intel Mac, Linux, Windows), you will need to recompile the native libraries:
> ```bash
> cd libs/KMSbase && ./Build.sh
> cd libs/ZAP_Tracking && ./Build.sh
> ```

## Tech Stack

- **Frontend**: React 18, TypeScript, Vite
- **3D Graphics**: Three.js, React Three Fiber, Drei
- **Styling**: Tailwind CSS, Framer Motion
- **State**: Zustand
- **Charts**: Recharts
- **Real-time**: Socket.io / aiohttp
- **Backend**: Python, aiohttp, python-socketio
- **Desktop**: Electron
- **Native**: C++ (KMSbase, ZAP_Tracking)

## Development

```bash
# Build for production
npm run build

# Run tests
npm test

# Run tests with UI
npm run test:ui

# Preview production build
npm run preview

# Build Electron app (unsigned)
CSC_IDENTITY_AUTO_DISCOVERY=false npm run electron:build
```

## Testing

The project includes a comprehensive test suite with **259 tests** covering:

| Category | Files | Tests | Coverage |
|----------|-------|-------|----------|
| Stores | 5 | 122 | gimbalStore, atemStore, presetsStore, gamepadStore, shortcutsStore |
| Hooks | 2 | 38 | useGamepad, useKeyboardControls |
| Services | 1 | 13 | WebSocket service |
| Components | 3 | 68 | AtemSettings, CameraControls, PresetControls |
| Integration | 1 | 18 | Cross-store workflows |

```bash
# Run all tests
npm test

# Run with coverage
npm test -- --coverage
```

## Changelog

### v1.5.1 (2025-01-12)
- **Test Suite**: Added 259 comprehensive tests (stores, hooks, services, components, integration)
- **Security Fixes**: Fixed all npm vulnerabilities (upgraded vite to 6.4.1, vitest to 4.0.16)
- **Audit Report**: Added detailed code audit report (AUDIT_REPORT.md)

### v1.5.0 (2025-01-11)
- **Position Presets**: Save and recall 9 camera positions per gimbal (Shift+1-9 to recall, Ctrl+Shift+1-9 to save)
- **Configuration Persistence**: ATEM mappings and presets are saved to disk and restored on restart
- **Auto-Center Zoom**: Continuous zoom slider returns to center when released
- **Gamepad Camera Control**: ATEM focus and zoom can be mapped to gamepad axes
- **Improved ATEM Feedback**: Error messages displayed when connection fails

### v1.4.0 (2025-01-11)
- **ATEM Switcher Integration**: Control camera parameters via Blackmagic ATEM
- **Camera Control Panel**: Focus, Aperture, Gain, and Zoom sliders with Auto buttons
- **Gimbal-to-Camera Mapping**: Associate each gimbal with an ATEM camera port (1-8)
- **Camera Type Support**: MFT (Micro Four Thirds) and EF (Canon EF) lens support
- **ATEM Settings UI**: Configure ATEM IP and camera mappings in Settings

### v1.3.0 (2025-01-10)
- **Multi-User Support**: Multiple operators can now control different gimbals
- **Master/Client Mode**: Connection mode selector at startup
- **Controller Visibility**: See who controls each gimbal in real-time
- **Session Management**: Automatic cleanup when users disconnect
- **Client Identity**: Each user gets a unique name (User 1, User 2, etc.)

### v1.2.1
- Improved documentation and version display
- macOS app launcher improvements

### v1.2.0
- Electron app packaging with DMG installer
- Zero-setup installation for end users

### v1.1.0
- Multi-gimbal support with instant switching
- Gimbal manager for adding/removing gimbals

### v1.0.0
- Initial release
- Virtual and hardware gimbal modes
- Xbox gamepad support
- Customizable keyboard shortcuts

## License

MIT License - See LICENSE file for details.

## Credits

- ZAP Tracking system by KMS - Martin Dubois, P.Eng
- Professional gimbal control UI
