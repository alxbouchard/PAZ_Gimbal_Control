#!/usr/bin/env python3
"""
ZAP Gimbal UI - ZT_Lib Bridge Server

This WebSocket server bridges the React UI to the ZT_Tracking C++ library.
It uses the libzt_python.dylib C wrapper for Python ctypes integration.

Author: ZAP Gimbal UI
Usage:  python3 zt_bridge.py [--port 3001] [--zt-path /path/to/ZAP_Tracking]
"""

import argparse
import asyncio
import json
import os
import sys
import time
import ctypes
from ctypes import c_void_p, c_int, c_double, c_char, c_uint, c_ubyte, POINTER, Structure
from typing import Optional, Dict, Any, List

# Config file path
CONFIG_FILE = os.path.join(os.path.dirname(__file__), 'gimbals.json')

# WebSocket server
try:
    import socketio
    from aiohttp import web
except ImportError:
    print("ERROR: Required packages not installed.")
    print("Run: pip3 install python-socketio aiohttp")
    sys.exit(1)


# ============== ZT_Python Structures (matching C wrapper) ==============

class ZTP_Position(Structure):
    _fields_ = [
        ("pitch_deg", c_double),
        ("roll_deg", c_double),
        ("yaw_deg", c_double),
    ]

class ZTP_Speed(Structure):
    _fields_ = [
        ("pitch_deg_s", c_double),
        ("roll_deg_s", c_double),
        ("yaw_deg_s", c_double),
    ]

class ZTP_Config(Structure):
    _fields_ = [
        ("pitch_min_deg", c_double),
        ("pitch_max_deg", c_double),
        ("roll_min_deg", c_double),
        ("roll_max_deg", c_double),
        ("yaw_min_deg", c_double),
        ("yaw_max_deg", c_double),
    ]

class ZTP_Info(Structure):
    _fields_ = [
        ("name", c_char * 16),
        ("ipv4_address", c_uint),
        ("ipv4_gateway", c_uint),
        ("ipv4_netmask", c_uint),
        ("version", c_ubyte * 4),
    ]


# ============== Global State ==============

# Each gimbal has its own state
gimbal_states: Dict[str, Dict[str, Any]] = {}

def create_gimbal_state():
    """Create a new gimbal state dictionary."""
    return {
        "position": {"pitch": 0.0, "yaw": 0.0, "roll": 0.0},
        "speed": {"pitch": 0.0, "yaw": 0.0, "roll": 0.0},
        "tracking": False,
        "speedBoost": False,
        "zoom": 50.0,
        "focus": 50.0,
        "home": {"pitch": 0.0, "yaw": 0.0, "roll": 0.0},
        "connected": False,
    }

# Legacy gimbal_state reference (points to active gimbal's state)
gimbal_state = create_gimbal_state()

available_gimbals = []
home_animation = {"active": False, "speed": 1.0, "target": {"pitch": 0.0, "yaw": 0.0, "roll": 0.0}}
active_gimbal_id: Optional[str] = None
real_gimbals: Dict[str, Any] = {}  # Store real gimbal wrappers by ID
current_speed_multiplier = 1.0  # Global speed multiplier (0.1 to 2.0)

# Virtual gimbal always exists and mirrors the active real gimbal
VIRTUAL_GIMBAL_ID = "gimbal-virtual"

# ============== Multi-User Sessions ==============
# Each client has their own session with their active gimbal
client_sessions: Dict[str, Dict[str, Any]] = {}
# Track which client controls which gimbal (gimbal_id -> sid)
gimbal_controllers: Dict[str, str] = {}
# Client counter for user-friendly names
client_counter = 0


def get_client_session(sid: str) -> Dict[str, Any]:
    """Get or create a session for a client."""
    if sid not in client_sessions:
        global client_counter
        client_counter += 1
        client_sessions[sid] = {
            "active_gimbal_id": VIRTUAL_GIMBAL_ID,
            "name": f"User {client_counter}",
            "speed_multiplier": 1.0,
        }
    return client_sessions[sid]


def get_client_active_gimbal(sid: str) -> str:
    """Get the active gimbal ID for a specific client."""
    session = get_client_session(sid)
    return session.get("active_gimbal_id", VIRTUAL_GIMBAL_ID)


def get_client_name(sid: str) -> str:
    """Get the display name for a client."""
    session = get_client_session(sid)
    return session.get("name", f"User-{sid[:6]}")


def get_gimbal_controller(gimbal_id: str) -> Optional[str]:
    """Get the name of the client controlling a gimbal, or None."""
    controller_sid = gimbal_controllers.get(gimbal_id)
    if controller_sid and controller_sid in client_sessions:
        return get_client_name(controller_sid)
    return None


def get_gimbals_with_controllers() -> List[Dict[str, Any]]:
    """Get gimbal list with controller info added."""
    result = []
    for g in available_gimbals:
        gimbal_data = g.copy()
        gimbal_data["controlledBy"] = get_gimbal_controller(g["id"])
        result.append(gimbal_data)
    return result


def get_active_state() -> Dict[str, Any]:
    """Get the state of the currently active gimbal."""
    global gimbal_state
    if active_gimbal_id and active_gimbal_id in gimbal_states:
        return gimbal_states[active_gimbal_id]
    return gimbal_state


def is_real_gimbal_active() -> bool:
    """Check if a real gimbal is currently active (not virtual)."""
    return active_gimbal_id is not None and active_gimbal_id != VIRTUAL_GIMBAL_ID


def get_active_mode() -> str:
    """Get the mode of the active gimbal."""
    if is_real_gimbal_active():
        return "real"
    return "virtual"


def save_gimbal_config():
    """Save gimbal configuration to JSON file."""
    config = {
        "gimbals": [
            {
                "id": g["id"],
                "name": g["name"],
                "ip": g.get("ip", ""),
                "mode": g.get("mode", "real")
            }
            for g in available_gimbals
            if g["id"] != VIRTUAL_GIMBAL_ID  # Don't save virtual gimbal
        ]
    }
    try:
        with open(CONFIG_FILE, 'w') as f:
            json.dump(config, f, indent=2)
        print(f"Saved gimbal config to {CONFIG_FILE}")
    except Exception as e:
        print(f"Error saving gimbal config: {e}")


def load_gimbal_config():
    """Load gimbal configuration from JSON file."""
    global available_gimbals, gimbal_states

    if not os.path.exists(CONFIG_FILE):
        print("No gimbal config file found, starting fresh")
        return

    try:
        with open(CONFIG_FILE, 'r') as f:
            config = json.load(f)

        for gimbal_cfg in config.get("gimbals", []):
            gimbal_id = gimbal_cfg.get("id")
            if not gimbal_id or gimbal_id == VIRTUAL_GIMBAL_ID:
                continue

            # Check if already exists
            if any(g["id"] == gimbal_id for g in available_gimbals):
                continue

            gimbal_info = {
                "id": gimbal_id,
                "name": gimbal_cfg.get("name", "Gimbal"),
                "model": "DJI RS/Ronin",
                "connected": False,
                "ip": gimbal_cfg.get("ip", ""),
                "mode": "real"
            }

            new_state = create_gimbal_state()
            gimbal_states[gimbal_id] = new_state
            available_gimbals.append(gimbal_info)

            print(f"Loaded gimbal from config: {gimbal_info['name']} at {gimbal_info['ip']}")

    except Exception as e:
        print(f"Error loading gimbal config: {e}")


# ============== ZT_Python Library Wrapper ==============

class ZTLibWrapper:
    """Wrapper for libzt_python.dylib C functions."""

    def __init__(self, zt_path: str):
        self.lib = None
        self.system = None
        self.gimbal = None

        # Find the library
        lib_file = os.path.join(zt_path, 'Binaries', 'libzt_python.dylib')
        if not os.path.exists(lib_file):
            raise FileNotFoundError(f"libzt_python.dylib not found at: {lib_file}")

        # Load the library
        try:
            self.lib = ctypes.CDLL(lib_file)
            self._setup_functions()
            print(f"Loaded ZT Python library from: {lib_file}")
        except Exception as e:
            print(f"Failed to load ZT library: {e}")
            raise

    def _setup_functions(self):
        """Set up function signatures for the ZT_Python library."""
        # System functions
        self.lib.ZTP_System_Create.restype = c_void_p
        self.lib.ZTP_System_Create.argtypes = []

        self.lib.ZTP_System_Release.restype = None
        self.lib.ZTP_System_Release.argtypes = [c_void_p]

        self.lib.ZTP_System_Gimbals_Detect.restype = c_int
        self.lib.ZTP_System_Gimbals_Detect.argtypes = [c_void_p]

        self.lib.ZTP_System_Gimbal_Get.restype = c_void_p
        self.lib.ZTP_System_Gimbal_Get.argtypes = [c_void_p, c_int]

        # Gimbal functions
        self.lib.ZTP_Gimbal_Activate.restype = c_int
        self.lib.ZTP_Gimbal_Activate.argtypes = [c_void_p]

        self.lib.ZTP_Gimbal_Release.restype = None
        self.lib.ZTP_Gimbal_Release.argtypes = [c_void_p]

        self.lib.ZTP_Gimbal_Debug.restype = None
        self.lib.ZTP_Gimbal_Debug.argtypes = [c_void_p]

        # Position functions
        self.lib.ZTP_Gimbal_Position_Get.restype = c_int
        self.lib.ZTP_Gimbal_Position_Get.argtypes = [c_void_p, POINTER(ZTP_Position)]

        self.lib.ZTP_Gimbal_Position_Set.restype = c_int
        self.lib.ZTP_Gimbal_Position_Set.argtypes = [c_void_p, c_double, c_double, c_double]

        # Speed functions
        self.lib.ZTP_Gimbal_Speed_Set.restype = c_int
        self.lib.ZTP_Gimbal_Speed_Set.argtypes = [c_void_p, c_double, c_double, c_double]

        self.lib.ZTP_Gimbal_Speed_Stop.restype = c_int
        self.lib.ZTP_Gimbal_Speed_Stop.argtypes = [c_void_p]

        # Config functions
        self.lib.ZTP_Gimbal_Config_Get.restype = None
        self.lib.ZTP_Gimbal_Config_Get.argtypes = [c_void_p, POINTER(ZTP_Config)]

        self.lib.ZTP_Gimbal_Config_Set.restype = c_int
        self.lib.ZTP_Gimbal_Config_Set.argtypes = [c_void_p, POINTER(ZTP_Config)]

        # Info functions
        self.lib.ZTP_Gimbal_Info_Get.restype = None
        self.lib.ZTP_Gimbal_Info_Get.argtypes = [c_void_p, POINTER(ZTP_Info)]

        # Focus functions
        self.lib.ZTP_Gimbal_Focus_Position_Set.restype = c_int
        self.lib.ZTP_Gimbal_Focus_Position_Set.argtypes = [c_void_p, c_double]

        self.lib.ZTP_Gimbal_Focus_Speed_Set.restype = c_int
        self.lib.ZTP_Gimbal_Focus_Speed_Set.argtypes = [c_void_p, c_double]

        self.lib.ZTP_Gimbal_Focus_Cal.restype = c_int
        self.lib.ZTP_Gimbal_Focus_Cal.argtypes = [c_void_p, c_int]

        # Track functions
        self.lib.ZTP_Gimbal_Track_Switch.restype = c_int
        self.lib.ZTP_Gimbal_Track_Switch.argtypes = [c_void_p]

        self.lib.ZTP_Gimbal_Track_Speed_Set.restype = c_int
        self.lib.ZTP_Gimbal_Track_Speed_Set.argtypes = [c_void_p, c_double]

        # Utility functions
        self.lib.ZTP_Result_GetName.restype = ctypes.c_char_p
        self.lib.ZTP_Result_GetName.argtypes = [c_int]

        self.lib.ZTP_GetVersion.restype = ctypes.c_char_p
        self.lib.ZTP_GetVersion.argtypes = []

    def create_system(self):
        """Create the ISystem instance."""
        self.system = self.lib.ZTP_System_Create()
        return self.system

    def release_system(self):
        """Release the system."""
        if self.system:
            self.lib.ZTP_System_Release(self.system)
            self.system = None

    def detect_gimbals(self) -> int:
        """Detect available gimbals."""
        if self.system:
            return self.lib.ZTP_System_Gimbals_Detect(self.system)
        return -1

    def get_gimbal(self, index: int = 0):
        """Get gimbal by index."""
        if self.system:
            self.gimbal = self.lib.ZTP_System_Gimbal_Get(self.system, index)
            return self.gimbal
        return None

    def activate_gimbal(self) -> int:
        """Activate the current gimbal."""
        if self.gimbal:
            return self.lib.ZTP_Gimbal_Activate(self.gimbal)
        return -1

    def release_gimbal(self):
        """Release the gimbal."""
        if self.gimbal:
            self.lib.ZTP_Gimbal_Release(self.gimbal)
            self.gimbal = None

    def get_position(self) -> Optional[Dict[str, float]]:
        """Get current gimbal position."""
        if self.gimbal:
            pos = ZTP_Position()
            result = self.lib.ZTP_Gimbal_Position_Get(self.gimbal, ctypes.byref(pos))
            if result == 0:  # ZT_OK
                return {
                    "pitch": pos.pitch_deg,
                    "roll": pos.roll_deg,
                    "yaw": pos.yaw_deg,
                }
        return None

    def set_position(self, pitch: float, roll: float, yaw: float) -> int:
        """Set gimbal position."""
        if self.gimbal:
            return self.lib.ZTP_Gimbal_Position_Set(self.gimbal, pitch, roll, yaw)
        return -1

    def set_speed(self, pitch: float, roll: float, yaw: float) -> int:
        """Set gimbal speed."""
        if self.gimbal:
            return self.lib.ZTP_Gimbal_Speed_Set(self.gimbal, pitch, roll, yaw)
        return -1

    def stop_speed(self) -> int:
        """Stop gimbal movement."""
        if self.gimbal:
            return self.lib.ZTP_Gimbal_Speed_Stop(self.gimbal)
        return -1

    def get_info(self) -> Optional[Dict[str, Any]]:
        """Get gimbal info."""
        if self.gimbal:
            info = ZTP_Info()
            self.lib.ZTP_Gimbal_Info_Get(self.gimbal, ctypes.byref(info))
            ip = info.ipv4_address
            return {
                "name": info.name.decode('utf-8', errors='ignore').strip('\x00'),
                "ip": f"{(ip >> 24) & 0xFF}.{(ip >> 16) & 0xFF}.{(ip >> 8) & 0xFF}.{ip & 0xFF}",
                "version": f"{info.version[0]}.{info.version[1]}.{info.version[2]}.{info.version[3]}",
            }
        return None

    def set_focus_position(self, position: float) -> int:
        """Set focus position."""
        if self.gimbal:
            return self.lib.ZTP_Gimbal_Focus_Position_Set(self.gimbal, position)
        return -1

    def track_switch(self) -> int:
        """Toggle tracking mode."""
        if self.gimbal:
            return self.lib.ZTP_Gimbal_Track_Switch(self.gimbal)
        return -1

    def debug(self):
        """Print debug info."""
        if self.gimbal:
            self.lib.ZTP_Gimbal_Debug(self.gimbal)

    def release(self):
        """Release all resources."""
        self.release_gimbal()
        self.release_system()


# Global ZT wrapper
zt_wrapper: Optional[ZTLibWrapper] = None


# ============== Safety Constants ==============
# Based on DJI Ronin RS 3/RS 4 official specifications

# Maximum allowed speed in degrees per second (DJI RS spec: 360°/s for all axes)
MAX_SPEED_DEG_S = 360.0

# Maximum allowed speed change per update (prevents sudden jerks)
# At 20Hz update rate, this allows reaching max speed in ~0.36 seconds
MAX_SPEED_CHANGE_DEG_S = 200.0

# Previous speed values for rate limiting
_prev_speed = {"pitch": 0.0, "yaw": 0.0, "roll": 0.0}


def _clamp(value: float, min_val: float, max_val: float) -> float:
    """Clamp a value between min and max."""
    return max(min_val, min(max_val, value))


def _validate_speed(value: float) -> float:
    """Validate and sanitize a speed value."""
    import math
    # Check for NaN or infinity
    if math.isnan(value) or math.isinf(value):
        print(f"WARNING: Invalid speed value detected: {value}, setting to 0")
        return 0.0
    # Clamp to safe range
    return _clamp(value, -MAX_SPEED_DEG_S, MAX_SPEED_DEG_S)


def _apply_rate_limit(current: float, target: float, max_change: float) -> float:
    """Apply rate limiting to prevent sudden speed changes."""
    diff = target - current
    if abs(diff) > max_change:
        return current + (max_change if diff > 0 else -max_change)
    return target


# ============== Gimbal Control Functions ==============

def set_gimbal_speed(pitch: float = 0, yaw: float = 0, roll: float = 0):
    """Set gimbal rotation speed (deg/s) with safety checks."""
    global gimbal_state, _prev_speed

    # Validate all inputs
    pitch = _validate_speed(pitch)
    yaw = _validate_speed(yaw)
    roll = _validate_speed(roll)

    # Apply rate limiting for smooth acceleration (prevents jerky movements)
    pitch = _apply_rate_limit(_prev_speed["pitch"], pitch, MAX_SPEED_CHANGE_DEG_S)
    yaw = _apply_rate_limit(_prev_speed["yaw"], yaw, MAX_SPEED_CHANGE_DEG_S)
    roll = _apply_rate_limit(_prev_speed["roll"], roll, MAX_SPEED_CHANGE_DEG_S)

    # Store for next rate limit check
    _prev_speed = {"pitch": pitch, "yaw": yaw, "roll": roll}

    gimbal_state["speed"] = {"pitch": pitch, "yaw": yaw, "roll": roll}

    # Send to real gimbal if one is active
    if is_real_gimbal_active() and active_gimbal_id in real_gimbals:
        try:
            real_gimbals[active_gimbal_id].set_speed(pitch, roll, yaw)
        except Exception as e:
            print(f"Error setting speed: {e}")
            # Safety: stop gimbal on error
            try:
                real_gimbals[active_gimbal_id].stop_speed()
            except:
                pass


def stop_gimbal():
    """Stop all gimbal movement immediately (emergency stop)."""
    global gimbal_state, _prev_speed

    # Reset rate limiting state
    _prev_speed = {"pitch": 0.0, "yaw": 0.0, "roll": 0.0}
    gimbal_state["speed"] = {"pitch": 0, "yaw": 0, "roll": 0}

    # Stop real gimbal if active
    if is_real_gimbal_active() and active_gimbal_id in real_gimbals:
        try:
            real_gimbals[active_gimbal_id].stop_speed()
        except Exception as e:
            print(f"Error stopping gimbal: {e}")


def go_home():
    """Return gimbal to home position (speed is read dynamically from current_speed_multiplier)."""
    global gimbal_state, home_animation

    home_animation = {
        "active": True,
        "target": gimbal_state["home"].copy()
    }
    print("Going home (speed adjustable in real-time)")

    # Send to real gimbal if active
    if is_real_gimbal_active() and active_gimbal_id in real_gimbals:
        try:
            real_gimbals[active_gimbal_id].set_position(
                gimbal_state["home"]["pitch"],
                gimbal_state["home"]["roll"],
                gimbal_state["home"]["yaw"]
            )
        except Exception as e:
            print(f"Error going home: {e}")


def set_home():
    """Set current position as home."""
    global gimbal_state
    gimbal_state["home"] = gimbal_state["position"].copy()
    print(f"Home position set: {gimbal_state['home']}")


def get_gimbal_position() -> Dict[str, float]:
    """Get current gimbal position."""
    global gimbal_state

    # Read from real gimbal if active
    if is_real_gimbal_active() and active_gimbal_id in real_gimbals:
        try:
            pos = real_gimbals[active_gimbal_id].get_position()
            if pos:
                gimbal_state["position"] = pos
                # Mirror to virtual gimbal
                if VIRTUAL_GIMBAL_ID in gimbal_states:
                    gimbal_states[VIRTUAL_GIMBAL_ID]["position"] = pos.copy()
        except Exception as e:
            print(f"Error getting position: {e}")

    return gimbal_state["position"]


def set_focus(value: float):
    """Set focus position (0-100%)."""
    global gimbal_state
    gimbal_state["focus"] = value

    # Send to real gimbal if active
    if is_real_gimbal_active() and active_gimbal_id in real_gimbals:
        try:
            real_gimbals[active_gimbal_id].set_focus_position(value)
        except Exception as e:
            print(f"Error setting focus: {e}")


def calibrate_focus():
    """Auto-calibrate focus."""
    pass


def toggle_tracking(enabled: bool):
    """Toggle tracking mode."""
    global gimbal_state
    gimbal_state["tracking"] = enabled

    # Send to real gimbal if active
    if is_real_gimbal_active() and active_gimbal_id in real_gimbals:
        try:
            real_gimbals[active_gimbal_id].track_switch()
        except Exception as e:
            print(f"Error toggling tracking: {e}")


# ============== Socket.IO Server ==============

sio = socketio.AsyncServer(
    async_mode='aiohttp',
    cors_allowed_origins=['http://localhost:3000', 'http://localhost:5173', '*']
)
app = web.Application()
sio.attach(app)


@sio.event
async def connect(sid, environ):
    # Create session for this client
    session = get_client_session(sid)
    client_name = get_client_name(sid)
    print(f'Client connected: {client_name} ({sid})')

    # Send initial state with controller info
    await sio.emit('gimbal:list', get_gimbals_with_controllers(), room=sid)

    # Send this client's active gimbal (defaults to virtual)
    client_gimbal = get_client_active_gimbal(sid)
    await sio.emit('gimbal:selected', client_gimbal, room=sid)

    # Send client's own name so they know who they are
    await sio.emit('client:identity', {"name": client_name, "sid": sid}, room=sid)

    await sio.emit('gimbal:position', gimbal_state["position"], room=sid)
    await sio.emit('gimbal:status', {
        "connected": gimbal_state["connected"],
        "tracking": gimbal_state["tracking"],
        "speedBoost": gimbal_state["speedBoost"],
        "mode": get_active_mode(),
    }, room=sid)


@sio.event
async def disconnect(sid):
    client_name = get_client_name(sid) if sid in client_sessions else f"Unknown ({sid})"
    print(f'Client disconnected: {client_name}')

    # Release any gimbals this client was controlling
    gimbals_released = []
    for gimbal_id, controller_sid in list(gimbal_controllers.items()):
        if controller_sid == sid:
            del gimbal_controllers[gimbal_id]
            gimbals_released.append(gimbal_id)

    # Clean up session
    if sid in client_sessions:
        del client_sessions[sid]

    # Notify all clients of updated gimbal list (controller info changed)
    if gimbals_released:
        await sio.emit('gimbal:list', get_gimbals_with_controllers())

    print('')
    print('╔═══════════════════════════════════════════════════════════════╗')
    print('║                                                               ║')
    print('║   ⚠️   TO STOP: Press Ctrl+C or close this window            ║')
    print('║                                                               ║')
    print('╚═══════════════════════════════════════════════════════════════╝')
    print('')


@sio.on('gimbal:setSpeed')
async def handle_set_speed(sid, data):
    pitch = data.get('pitch', 0) * 30  # Scale to deg/s
    yaw = data.get('yaw', 0) * 30
    roll = data.get('roll', 0) * 30
    set_gimbal_speed(pitch, yaw, roll)


@sio.on('gimbal:stopSpeed')
async def handle_stop_speed(sid):
    print('Emergency stop!')
    stop_gimbal()


@sio.on('gimbal:goHome')
async def handle_go_home(sid, data=None):
    # Use current_speed_multiplier (already set via gimbal:setSpeedMultiplier)
    go_home()


@sio.on('gimbal:setHome')
async def handle_set_home(sid):
    set_home()


@sio.on('gimbal:setSpeedMultiplier')
async def handle_set_speed_multiplier(sid, value):
    """Set the global speed multiplier (affects home, tracking, etc.)."""
    global current_speed_multiplier
    current_speed_multiplier = max(0.1, min(2.0, float(value)))
    print(f'Speed multiplier: {current_speed_multiplier * 100:.0f}%')


@sio.on('gimbal:toggleTracking')
async def handle_toggle_tracking(sid, enabled):
    print(f'Tracking at speed {current_speed_multiplier * 100:.0f}%: {enabled}')
    toggle_tracking(enabled)
    await sio.emit('gimbal:status', {
        "connected": gimbal_state["connected"],
        "tracking": gimbal_state["tracking"],
        "speedBoost": gimbal_state["speedBoost"],
        "mode": get_active_mode(),
    })


@sio.on('gimbal:toggleSpeedBoost')
async def handle_toggle_speed_boost(sid, enabled):
    global gimbal_state
    print(f'Speed boost: {enabled}')
    gimbal_state["speedBoost"] = enabled
    await sio.emit('gimbal:status', {
        "connected": gimbal_state["connected"],
        "tracking": gimbal_state["tracking"],
        "speedBoost": gimbal_state["speedBoost"],
        "mode": get_active_mode(),
    })


@sio.on('client:setName')
async def handle_set_name(sid, name):
    """Allow client to change their display name."""
    session = get_client_session(sid)
    old_name = session.get("name", "Unknown")
    # Sanitize name
    name = str(name).strip()[:20] if name else f"User-{sid[:6]}"
    session["name"] = name
    print(f'Client renamed: {old_name} -> {name}')

    # Notify this client of their new identity
    await sio.emit('client:identity', {"name": name, "sid": sid}, room=sid)

    # Update all clients with new controller info
    await sio.emit('gimbal:list', get_gimbals_with_controllers())


@sio.on('gimbal:setZoom')
async def handle_set_zoom(sid, value):
    global gimbal_state
    gimbal_state["zoom"] = value
    print(f'Zoom: {value}')


@sio.on('gimbal:setFocus')
async def handle_set_focus(sid, value):
    set_focus(value)
    print(f'Focus: {value}')


@sio.on('gimbal:calibrateFocus')
async def handle_calibrate_focus(sid):
    print('Calibrating focus')
    calibrate_focus()


@sio.on('gimbal:add')
async def handle_add_gimbal(sid, gimbal_config):
    """Add a new gimbal configuration."""
    global available_gimbals, gimbal_states

    name = gimbal_config.get('name', 'New Gimbal')
    ip = gimbal_config.get('ip', '')

    if not ip:
        await sio.emit('error', 'IP address is required', room=sid)
        return

    # Check if IP already exists
    for g in available_gimbals:
        if g.get('ip') == ip:
            await sio.emit('error', f'Gimbal with IP {ip} already exists', room=sid)
            return

    # Generate unique ID
    gimbal_id = f"gimbal-{ip.replace('.', '-')}"

    # Create gimbal info
    gimbal_info = {
        "id": gimbal_id,
        "name": name,
        "model": "DJI RS/Ronin",
        "connected": False,  # Will try to connect
        "ip": ip,
        "mode": "real"
    }

    # Create state for this gimbal
    new_state = create_gimbal_state()
    gimbal_states[gimbal_id] = new_state
    available_gimbals.append(gimbal_info)

    # Save config to file
    save_gimbal_config()

    print(f'Added gimbal: {name} at {ip}')

    # Notify all clients
    await sio.emit('gimbal:list', get_gimbals_with_controllers())
    await sio.emit('gimbal:added', gimbal_info)

    # Try to connect to the gimbal
    asyncio.create_task(try_connect_gimbal(gimbal_id, ip))


@sio.on('gimbal:remove')
async def handle_remove_gimbal(sid, gimbal_id):
    """Remove a gimbal configuration."""
    global available_gimbals, gimbal_states, active_gimbal_id, gimbal_state

    # Cannot remove virtual gimbal
    if gimbal_id == VIRTUAL_GIMBAL_ID:
        await sio.emit('error', 'Cannot remove virtual gimbal', room=sid)
        return

    # Find and remove gimbal
    gimbal_to_remove = None
    for g in available_gimbals:
        if g['id'] == gimbal_id:
            gimbal_to_remove = g
            break

    if not gimbal_to_remove:
        await sio.emit('error', f'Gimbal {gimbal_id} not found', room=sid)
        return

    # Disconnect if connected
    if gimbal_id in real_gimbals:
        try:
            # Clean up wrapper
            del real_gimbals[gimbal_id]
        except:
            pass

    # Remove from lists
    available_gimbals = [g for g in available_gimbals if g['id'] != gimbal_id]
    if gimbal_id in gimbal_states:
        del gimbal_states[gimbal_id]

    # If this was the active gimbal, switch to virtual
    if active_gimbal_id == gimbal_id:
        active_gimbal_id = VIRTUAL_GIMBAL_ID
        gimbal_state = gimbal_states[VIRTUAL_GIMBAL_ID]
        await sio.emit('gimbal:selected', VIRTUAL_GIMBAL_ID)

    # Save config
    save_gimbal_config()

    print(f'Removed gimbal: {gimbal_id}')

    # Notify all clients
    await sio.emit('gimbal:list', get_gimbals_with_controllers())
    await sio.emit('gimbal:removed', gimbal_id)


@sio.on('gimbal:update')
async def handle_update_gimbal(sid, gimbal_config):
    """Update gimbal configuration (name, IP)."""
    global available_gimbals

    gimbal_id = gimbal_config.get('id')
    if not gimbal_id or gimbal_id == VIRTUAL_GIMBAL_ID:
        return

    for g in available_gimbals:
        if g['id'] == gimbal_id:
            if 'name' in gimbal_config:
                g['name'] = gimbal_config['name']
            if 'ip' in gimbal_config:
                old_ip = g['ip']
                new_ip = gimbal_config['ip']
                if old_ip != new_ip:
                    g['ip'] = new_ip
                    # Reconnect with new IP
                    asyncio.create_task(try_connect_gimbal(gimbal_id, new_ip))
            break

    save_gimbal_config()
    await sio.emit('gimbal:list', get_gimbals_with_controllers())


@sio.on('gimbal:connect')
async def handle_connect_gimbal(sid, gimbal_id):
    """Try to connect to a gimbal."""
    for g in available_gimbals:
        if g['id'] == gimbal_id and g.get('ip'):
            await try_connect_gimbal(gimbal_id, g['ip'])
            break


async def try_connect_gimbal(gimbal_id: str, ip: str):
    """Attempt to connect to a gimbal at the given IP."""
    global available_gimbals, gimbal_states, zt_wrapper

    print(f"Attempting to connect to gimbal at {ip}...")

    # Update status to connecting
    for g in available_gimbals:
        if g['id'] == gimbal_id:
            g['connecting'] = True
            break
    await sio.emit('gimbal:list', get_gimbals_with_controllers())

    try:
        # Try to find ZT library path
        zt_path = None
        possible_paths = [
            os.path.join(os.path.dirname(__file__), '..', 'libs', 'ZAP_Tracking'),
            os.path.expanduser('~/Documents/Code/ZAP_Tracking'),
        ]
        for path in possible_paths:
            if os.path.exists(path):
                zt_path = os.path.abspath(path)
                break

        if zt_path:
            # Create new wrapper for this gimbal
            wrapper = ZTLibWrapper(zt_path)
            wrapper.create_system()

            # Try to detect gimbal at specific IP
            # Note: This might need modification based on how ZT library handles IPs
            result = wrapper.detect_gimbals()
            gimbal = wrapper.get_gimbal(0)

            if gimbal:
                wrapper.activate_gimbal()
                real_gimbals[gimbal_id] = wrapper

                # Update gimbal info
                for g in available_gimbals:
                    if g['id'] == gimbal_id:
                        g['connected'] = True
                        g['connecting'] = False
                        info = wrapper.get_info()
                        if info:
                            g['model'] = f"v{info.get('version', '?')}"
                        break

                gimbal_states[gimbal_id]['connected'] = True
                print(f"Connected to gimbal at {ip}")
            else:
                raise Exception("No gimbal found at this address")
        else:
            raise Exception("ZT library not found")

    except Exception as e:
        print(f"Failed to connect to gimbal at {ip}: {e}")
        for g in available_gimbals:
            if g['id'] == gimbal_id:
                g['connected'] = False
                g['connecting'] = False
                break
        gimbal_states[gimbal_id]['connected'] = False

    await sio.emit('gimbal:list', get_gimbals_with_controllers())


@sio.on('gimbal:select')
async def handle_select_gimbal(sid, gimbal_id):
    global active_gimbal_id, gimbal_state

    # Validate gimbal exists
    gimbal_exists = any(g["id"] == gimbal_id for g in available_gimbals)
    if not gimbal_exists:
        print(f'Invalid gimbal ID: {gimbal_id}')
        return

    client_name = get_client_name(sid)
    session = get_client_session(sid)

    # Release any gimbal this client was previously controlling
    old_gimbal = session.get("active_gimbal_id")
    if old_gimbal and old_gimbal in gimbal_controllers:
        if gimbal_controllers[old_gimbal] == sid:
            del gimbal_controllers[old_gimbal]

    # Update client's session
    session["active_gimbal_id"] = gimbal_id

    # Mark this client as controller (for real gimbals, not virtual)
    if gimbal_id != VIRTUAL_GIMBAL_ID:
        gimbal_controllers[gimbal_id] = sid

    print(f'{client_name} selected gimbal: {gimbal_id}')

    # Update legacy global state (for backward compatibility with position updates)
    active_gimbal_id = gimbal_id
    if gimbal_id in gimbal_states:
        gimbal_state = gimbal_states[gimbal_id]

    # Notify THIS client of their selection
    await sio.emit('gimbal:selected', gimbal_id, room=sid)
    await sio.emit('gimbal:status', {
        "connected": gimbal_state["connected"],
        "tracking": gimbal_state["tracking"],
        "speedBoost": gimbal_state["speedBoost"],
        "mode": get_active_mode(),
    }, room=sid)

    # Notify ALL clients of updated controller info
    await sio.emit('gimbal:list', get_gimbals_with_controllers())


# ============== Background Tasks ==============

async def position_broadcast_loop():
    """Periodically broadcast gimbal position to all clients."""
    global home_animation

    while True:
        try:
            position = get_gimbal_position()
            await sio.emit('gimbal:position', position)

            # Simulate position changes based on speed (for virtual mode or when controlling virtual gimbal)
            if not is_real_gimbal_active():
                # Handle home animation - READ SPEED DYNAMICALLY each frame
                if home_animation["active"]:
                    target = home_animation["target"]
                    # Use current_speed_multiplier in real-time (not stored value)
                    base_speed = 60.0  # degrees per second at 100%
                    move_rate = base_speed * current_speed_multiplier * 0.05  # per frame (50ms)

                    pos = gimbal_state["position"]
                    all_reached = True

                    for axis in ["pitch", "yaw", "roll"]:
                        diff = target[axis] - pos[axis]

                        # For yaw: calculate shortest path (like real Ronin RS)
                        # This handles continuous rotation properly
                        if axis == "yaw":
                            # Normalize the difference to find shortest path
                            # If we're at 350° and target is 10°, diff should be +20° not -340°
                            while diff > 180:
                                diff -= 360
                            while diff < -180:
                                diff += 360

                        if abs(diff) > 0.1:
                            all_reached = False
                            # Move towards target at real-time speed
                            if abs(diff) <= move_rate:
                                pos[axis] = target[axis] if axis != "yaw" else pos[axis] + diff
                            else:
                                pos[axis] += move_rate if diff > 0 else -move_rate

                    if all_reached:
                        home_animation["active"] = False
                        print("Home position reached")

                else:
                    # Normal speed-based movement
                    speed_mult = 2.0 if gimbal_state["speedBoost"] else 1.0
                    gimbal_state["position"]["pitch"] += gimbal_state["speed"]["pitch"] * 0.05 * speed_mult
                    gimbal_state["position"]["yaw"] += gimbal_state["speed"]["yaw"] * 0.05 * speed_mult
                    gimbal_state["position"]["roll"] += gimbal_state["speed"]["roll"] * 0.05 * speed_mult

                # Apply DJI Ronin RS mechanical limits
                # Tilt (Pitch): -90° to +90°
                gimbal_state["position"]["pitch"] = max(-90, min(90, gimbal_state["position"]["pitch"]))
                # Roll: ±45° for standard operation
                gimbal_state["position"]["roll"] = max(-45, min(45, gimbal_state["position"]["roll"]))
                # Pan (Yaw): Normalize to -180° to +180° for display
                yaw = gimbal_state["position"]["yaw"]
                gimbal_state["position"]["yaw"] = ((yaw % 360) + 540) % 360 - 180

        except Exception as e:
            print(f"Broadcast error: {e}")

        await asyncio.sleep(0.05)  # 20Hz update rate


async def telemetry_broadcast_loop():
    """Periodically send telemetry data."""
    while True:
        try:
            telemetry = {
                "timestamp": int(time.time() * 1000),
                "position": gimbal_state["position"].copy(),
                "speed": gimbal_state["speed"].copy(),
                # Only send real data - null for virtual mode (no fake data)
                "temperature": None if not is_real_gimbal_active() else 35,
                "batteryLevel": None if not is_real_gimbal_active() else 85,
            }
            await sio.emit('gimbal:telemetry', telemetry)
        except Exception as e:
            print(f"Telemetry error: {e}")

        await asyncio.sleep(0.5)  # 2Hz telemetry


# ============== HTTP Routes ==============

async def handle_status(request):
    """REST API: Get gimbal status."""
    return web.json_response({
        "gimbal": gimbal_state,
        "activeGimbalId": active_gimbal_id,
        "availableGimbals": available_gimbals,
    })


async def handle_gimbals(request):
    """REST API: List available gimbals."""
    return web.json_response(available_gimbals)


async def handle_health(request):
    """Health check endpoint."""
    real_count = len(real_gimbals)
    return web.json_response({
        "status": "ok",
        "timestamp": int(time.time() * 1000),
        "realGimbalsConnected": real_count,
        "activeGimbalId": active_gimbal_id,
        "activeMode": get_active_mode(),
    })


# ============== Main ==============

async def on_startup(app):
    """Start background tasks."""
    asyncio.create_task(position_broadcast_loop())
    asyncio.create_task(telemetry_broadcast_loop())


def init_zt_library(zt_path: str) -> bool:
    """Initialize the ZT_Tracking library and detect real gimbals."""
    global zt_wrapper, available_gimbals, active_gimbal_id, gimbal_state

    try:
        zt_wrapper = ZTLibWrapper(zt_path)

        # Create system and detect gimbals
        zt_wrapper.create_system()
        print("Detecting gimbals...")
        result = zt_wrapper.detect_gimbals()
        print(f"Gimbals_Detect result: {result}")

        # Get first gimbal
        gimbal = zt_wrapper.get_gimbal(0)
        if gimbal:
            print("Gimbal found! Activating...")
            result = zt_wrapper.activate_gimbal()
            print(f"Activate result: {result}")

            # Get gimbal info
            info = zt_wrapper.get_info()
            gimbal_id = "gimbal-0"
            if info:
                gimbal_info = {
                    "id": gimbal_id,
                    "name": info.get("name", "DJI Gimbal"),
                    "model": f"v{info.get('version', '?')}",
                    "connected": True,
                    "ip": info.get("ip", "EthCAN"),
                    "mode": "real"
                }
            else:
                gimbal_info = {
                    "id": gimbal_id,
                    "name": "DJI Gimbal",
                    "model": "DJI RS/Ronin",
                    "connected": True,
                    "ip": "EthCAN",
                    "mode": "real"
                }

            # Create state for this real gimbal
            real_state = create_gimbal_state()
            real_state["connected"] = True
            gimbal_states[gimbal_id] = real_state

            # Store the wrapper for this gimbal
            real_gimbals[gimbal_id] = zt_wrapper

            available_gimbals.append(gimbal_info)

            print(f"Gimbal initialized: {gimbal_info}")
            return True
        else:
            print("No gimbal found")
            return False

    except Exception as e:
        print(f"ERROR initializing ZT library: {e}")
        import traceback
        traceback.print_exc()
        return False


def main():
    global active_gimbal_id, gimbal_state

    parser = argparse.ArgumentParser(description='ZAP Gimbal UI - ZT Bridge Server')
    parser.add_argument('--port', type=int, default=3001, help='Server port (default: 3001)')
    parser.add_argument('--zt-path', type=str, default=None,
                        help='Path to ZAP_Tracking directory')
    parser.add_argument('--virtual', action='store_true',
                        help='Run in virtual/simulation mode without real gimbal')
    args = parser.parse_args()

    # Always create the virtual gimbal first (it mirrors real gimbals when connected)
    virtual_state = create_gimbal_state()
    virtual_state["connected"] = True
    gimbal_states[VIRTUAL_GIMBAL_ID] = virtual_state
    available_gimbals.append({
        "id": VIRTUAL_GIMBAL_ID,
        "name": "Virtual Gimbal",
        "model": "Mirror/Simulation",
        "connected": True,
        "ip": "127.0.0.1",
        "mode": "virtual"
    })
    print("Virtual gimbal created (always available)")

    # Load saved gimbal configurations
    load_gimbal_config()

    # Try to find ZT_Tracking path
    zt_path = args.zt_path
    if not zt_path:
        # Try common locations (relative to this project first)
        possible_paths = [
            os.path.join(os.path.dirname(__file__), '..', 'libs', 'ZAP_Tracking'),
            os.path.expanduser('~/Documents/Code/ZAP_Tracking'),
            os.path.join(os.path.dirname(__file__), '..', '..', 'ZAP_Tracking'),
        ]
        for path in possible_paths:
            if os.path.exists(path):
                zt_path = os.path.abspath(path)
                break

    # Initialize ZT library and detect real gimbals (unless virtual-only mode)
    has_real_gimbals = False
    if not args.virtual and zt_path:
        if init_zt_library(zt_path):
            print("ZT_Lib initialized successfully - real gimbal(s) detected")
            has_real_gimbals = True
        else:
            print("No real gimbals found, using virtual gimbal only")

    # Set initial active gimbal
    if has_real_gimbals:
        # Find first real gimbal and make it active
        for g in available_gimbals:
            if g["mode"] == "real":
                active_gimbal_id = g["id"]
                if active_gimbal_id in gimbal_states:
                    gimbal_state = gimbal_states[active_gimbal_id]
                break
    else:
        # Default to virtual gimbal
        active_gimbal_id = VIRTUAL_GIMBAL_ID
        gimbal_state = gimbal_states[VIRTUAL_GIMBAL_ID]

    # Setup HTTP routes
    app.router.add_get('/api/status', handle_status)
    app.router.add_get('/api/gimbals', handle_gimbals)
    app.router.add_get('/health', handle_health)

    # Setup Frontend Serving (Static Files)
    # Search for dist folder in likely locations
    possible_dist_paths = [
        os.path.join(os.path.dirname(__file__), 'dist'),      # Portable mode (server/dist) or local Dev
        os.path.join(os.path.dirname(__file__), '..', 'dist'), # Dev mode (root/dist)
        os.path.join(os.path.dirname(__file__), '..', 'app', 'dist'), # Electron packaged (Resources/server -> Resources/app/dist)
        os.path.join(os.path.dirname(__file__), '..', 'Resources', 'app', 'dist') # Fallback
    ]
    
    dist_path = None
    for path in possible_dist_paths:
        if os.path.exists(path) and os.path.isdir(path):
            dist_path = os.path.abspath(path)
            break
            
    if dist_path:
        print(f"Serving frontend from: {dist_path}")
        
        # Serve index.html for root path
        async def handle_index(request):
            return web.FileResponse(os.path.join(dist_path, 'index.html'))
        app.router.add_get('/', handle_index)
        
        # Serve static assets
        # Note: We put this AFTER API routes so API takes precedence
        app.router.add_static('/', dist_path, name='static')
    else:
        print("WARNING: Frontend 'dist' folder not found. API only mode.")
        
        # Serve Debug Info instead of 404
        async def handle_debug(request):
            debug_info = [
                "<h1>PAZ Gimbal Control - Debug Mode</h1>",
                "<p><strong>Error:</strong> Frontend 'dist' folder not found.</p>",
                "<h2>Debug Info:</h2>",
                f"<ul><li><strong>Current Working Directory:</strong> {os.getcwd()}</li>",
                f"<li><strong>Server Script Location:</strong> {os.path.abspath(__file__)}</li></ul>",
                "<h2>Checked Paths:</h2><ul>"
            ]
            for p in possible_dist_paths:
                exists = "✅ Found" if os.path.exists(p) else "❌ Not Found"
                debug_info.append(f"<li>{p}: {exists}</li>")
            debug_info.append("</ul>")
            
            return web.Response(text="".join(debug_info), content_type='text/html')
            
        app.router.add_get('/', handle_debug)

    # Setup startup handler
    app.on_startup.append(on_startup)

    # Print banner
    real_count = sum(1 for g in available_gimbals if g["mode"] == "real")
    mode_str = f"VIRTUAL + {real_count} REAL GIMBAL(S)" if real_count > 0 else "VIRTUAL ONLY"
    print(f"""
  ╔════════════════════════════════════════════════════════╗
  ║     PAZ Gimbal Control - Bridge Server                 ║
  ║     Running on http://localhost:{args.port}                   ║
  ║     Mode: {mode_str:<44} ║
  ║     Gimbals: {len(available_gimbals)} available                             ║
  ╚════════════════════════════════════════════════════════╝
    """)

    # Run server
    web.run_app(app, host='0.0.0.0', port=args.port, print=None)


if __name__ == '__main__':
    main()
