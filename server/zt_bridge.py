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
import os
import sys
import time
import ctypes
from ctypes import c_void_p, c_int, c_double, c_char, c_uint, c_ubyte, POINTER, Structure
from typing import Optional, Dict, Any

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

gimbal_state = {
    "position": {"pitch": 0.0, "yaw": 0.0, "roll": 0.0},
    "speed": {"pitch": 0.0, "yaw": 0.0, "roll": 0.0},
    "tracking": False,
    "speedBoost": False,
    "zoom": 50.0,
    "focus": 50.0,
    "home": {"pitch": 0.0, "yaw": 0.0, "roll": 0.0},
    "connected": False,
}

available_gimbals = []
home_animation = {"active": False, "speed": 1.0, "target": {"pitch": 0.0, "yaw": 0.0, "roll": 0.0}}
active_gimbal_id: Optional[str] = None
virtual_mode = True
current_speed_multiplier = 1.0  # Global speed multiplier (0.1 to 2.0)


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

    if zt_wrapper and not virtual_mode:
        try:
            zt_wrapper.set_speed(pitch, roll, yaw)
        except Exception as e:
            print(f"Error setting speed: {e}")
            # Safety: stop gimbal on error
            try:
                zt_wrapper.stop_speed()
            except:
                pass


def stop_gimbal():
    """Stop all gimbal movement immediately (emergency stop)."""
    global gimbal_state, _prev_speed

    # Reset rate limiting state
    _prev_speed = {"pitch": 0.0, "yaw": 0.0, "roll": 0.0}
    gimbal_state["speed"] = {"pitch": 0, "yaw": 0, "roll": 0}

    if zt_wrapper and not virtual_mode:
        try:
            zt_wrapper.stop_speed()
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

    if zt_wrapper and not virtual_mode:
        try:
            zt_wrapper.set_position(
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

    if zt_wrapper and not virtual_mode:
        try:
            pos = zt_wrapper.get_position()
            if pos:
                gimbal_state["position"] = pos
        except Exception as e:
            print(f"Error getting position: {e}")

    return gimbal_state["position"]


def set_focus(value: float):
    """Set focus position (0-100%)."""
    global gimbal_state
    gimbal_state["focus"] = value

    if zt_wrapper and not virtual_mode:
        try:
            zt_wrapper.set_focus_position(value)
        except Exception as e:
            print(f"Error setting focus: {e}")


def calibrate_focus():
    """Auto-calibrate focus."""
    pass


def toggle_tracking(enabled: bool):
    """Toggle tracking mode."""
    global gimbal_state
    gimbal_state["tracking"] = enabled

    if zt_wrapper and not virtual_mode:
        try:
            zt_wrapper.track_switch()
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
    print(f'Client connected: {sid}')

    # Send initial state
    await sio.emit('gimbal:list', available_gimbals, room=sid)
    if active_gimbal_id:
        await sio.emit('gimbal:selected', active_gimbal_id, room=sid)

    await sio.emit('gimbal:position', gimbal_state["position"], room=sid)
    await sio.emit('gimbal:status', {
        "connected": gimbal_state["connected"],
        "tracking": gimbal_state["tracking"],
        "speedBoost": gimbal_state["speedBoost"],
    }, room=sid)


@sio.event
async def disconnect(sid):
    print(f'Client disconnected: {sid}')


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
    })


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


@sio.on('gimbal:select')
async def handle_select_gimbal(sid, gimbal_id):
    global active_gimbal_id
    print(f'Selected gimbal: {gimbal_id}')
    active_gimbal_id = gimbal_id
    await sio.emit('gimbal:selected', gimbal_id)


# ============== Background Tasks ==============

async def position_broadcast_loop():
    """Periodically broadcast gimbal position to all clients."""
    global home_animation

    while True:
        try:
            position = get_gimbal_position()
            await sio.emit('gimbal:position', position)

            # Simulate position changes based on speed (for virtual mode)
            if virtual_mode:
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
                "temperature": 35 + (hash(time.time()) % 10),
                "batteryLevel": 85,
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
    return web.json_response({
        "status": "ok",
        "timestamp": int(time.time() * 1000),
        "ztConnected": not virtual_mode,
    })


# ============== Main ==============

async def on_startup(app):
    """Start background tasks."""
    asyncio.create_task(position_broadcast_loop())
    asyncio.create_task(telemetry_broadcast_loop())


def init_zt_library(zt_path: str) -> bool:
    """Initialize the ZT_Tracking library."""
    global zt_wrapper, virtual_mode, available_gimbals, active_gimbal_id, gimbal_state

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
            if info:
                gimbal_info = {
                    "id": "gimbal-0",
                    "name": info.get("name", "DJI Gimbal"),
                    "model": f"v{info.get('version', '?')}",
                    "connected": True,
                    "ip": info.get("ip", "EthCAN")
                }
            else:
                gimbal_info = {
                    "id": "gimbal-0",
                    "name": "DJI Gimbal",
                    "model": "DJI RS/Ronin",
                    "connected": True,
                    "ip": "EthCAN"
                }

            available_gimbals.append(gimbal_info)
            active_gimbal_id = "gimbal-0"
            gimbal_state["connected"] = True
            virtual_mode = False

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
    global virtual_mode, active_gimbal_id

    parser = argparse.ArgumentParser(description='ZAP Gimbal UI - ZT Bridge Server')
    parser.add_argument('--port', type=int, default=3001, help='Server port (default: 3001)')
    parser.add_argument('--zt-path', type=str, default=None,
                        help='Path to ZAP_Tracking directory')
    parser.add_argument('--virtual', action='store_true',
                        help='Run in virtual/simulation mode without real gimbal')
    args = parser.parse_args()

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

    # Initialize ZT library (unless virtual mode)
    if not args.virtual and zt_path:
        if init_zt_library(zt_path):
            print("ZT_Lib initialized successfully")
        else:
            print("Running in VIRTUAL MODE (no real gimbal)")
            virtual_mode = True
            available_gimbals.append({
                "id": "gimbal-virtual",
                "name": "Virtual Gimbal",
                "model": "Simulation",
                "connected": True,
                "ip": "127.0.0.1"
            })
            active_gimbal_id = "gimbal-virtual"
            gimbal_state["connected"] = True
    else:
        print("Running in VIRTUAL MODE")
        virtual_mode = True
        available_gimbals.append({
            "id": "gimbal-virtual",
            "name": "Virtual Gimbal",
            "model": "Simulation",
            "connected": True,
            "ip": "127.0.0.1"
        })
        active_gimbal_id = "gimbal-virtual"
        gimbal_state["connected"] = True

    # Setup HTTP routes
    app.router.add_get('/api/status', handle_status)
    app.router.add_get('/api/gimbals', handle_gimbals)
    app.router.add_get('/health', handle_health)

    # Setup startup handler
    app.on_startup.append(on_startup)

    # Print banner
    mode = 'REAL GIMBAL (ZT_Lib)' if not virtual_mode else 'VIRTUAL/SIMULATION'
    print(f"""
  ╔════════════════════════════════════════════════════════╗
  ║     ZAP Gimbal UI - ZT Bridge Server                   ║
  ║     Running on http://localhost:{args.port}                   ║
  ║     Mode: {mode:<44} ║
  ╚════════════════════════════════════════════════════════╝
    """)

    # Run server
    web.run_app(app, host='0.0.0.0', port=args.port, print=None)


if __name__ == '__main__':
    main()
