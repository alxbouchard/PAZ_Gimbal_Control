# PAZ Gimbal Control

Professional web-based control interface for DJI Ronin RS gimbals with ZAP Tracking system integration.

## Features

- **3D Virtual Gimbal Visualization** - Real-time 3D model replicating DJI Ronin RS behavior
- **Dual Joystick Controls** - Touch and mouse-friendly virtual joysticks
- **Virtual Gimbal Mode** - Test and demo without physical hardware
- **Real-time Telemetry** - Position graphs and statistics
- **Gamepad Support** - Xbox controller compatible
- **Keyboard Shortcuts** - Fully customizable keyboard controls
- **DJI Ronin RS Specs** - Accurate 360 deg/s speed limits, mechanical limits, continuous pan rotation

## Quick Start

### Prerequisites

- Node.js 18+
- Python 3.8+
- npm

### Installation (One Command)

```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/PAZ_Gimbal_Control.git
cd PAZ_Gimbal_Control

# Run the setup script
./setup.sh
```

The setup script will:
1. Install Node.js dependencies
2. Install Python dependencies
3. Build native libraries (KMSbase + ZAP_Tracking)
4. Verify the installation

### Running the Application

```bash
# Start with hardware (requires ZT_Agent)
npm start

# Start in virtual mode (no hardware required)
npm run virtual

# Development mode (frontend only)
npm run dev
```

The app will be available at `http://localhost:5173`

## Project Structure

```
PAZ_Gimbal_Control/
├── src/                    # React frontend
│   ├── components/
│   │   ├── ControlPanel/   # Joysticks, sliders, quick actions
│   │   ├── VirtualGimbal/  # 3D visualization (Three.js)
│   │   ├── Dashboard/      # Telemetry and status
│   │   └── pages/          # Settings, shortcuts
│   ├── hooks/              # Custom React hooks
│   ├── services/           # WebSocket client
│   └── store/              # Zustand state management
├── server/                 # Python backend
│   └── zt_bridge.py        # WebSocket server + ZT_Agent bridge
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
- **Native**: C++ (KMSbase, ZAP_Tracking)

## Development

```bash
# Build for production
npm run build

# Run tests
npm test

# Preview production build
npm run preview
```

## License

MIT License - See LICENSE file for details.

## Credits

- ZAP Tracking system by KMS - Martin Dubois, P.Eng
- Professional gimbal control UI
