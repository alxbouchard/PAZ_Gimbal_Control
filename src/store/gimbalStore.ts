import { create } from 'zustand';
import type { GimbalStore, ControlMapping, GimbalPosition, GimbalSpeed, GimbalInfo, TelemetryData, GimbalMode } from '../types';

const defaultControlMapping: ControlMapping = {
  joystickLeft: {
    x: 'yaw',
    y: 'pitch',
  },
  joystickRight: {
    x: 'roll',
    y: 'focus',
  },
  invertPitch: true,
  invertYaw: true,
  invertRoll: false,
};

export const useGimbalStore = create<GimbalStore>((set, _get) => ({
  // Connection
  connected: false,
  connecting: false,
  serverUrl: 'http://localhost:3001',

  // Current gimbal
  activeGimbalId: null,
  availableGimbals: [],
  gimbalMode: 'unknown' as GimbalMode,

  // Position & movement
  position: { pitch: 0, yaw: 0, roll: 0 },
  speed: { pitch: 0, yaw: 0, roll: 0 },

  // Control states
  controlling: false,
  tracking: false,
  speedBoost: false,

  // Camera controls
  zoom: 50,
  focus: 50,

  // Settings
  sensitivity: 1.0,
  controlMapping: defaultControlMapping,

  // Telemetry history
  telemetryHistory: [],

  // Actions
  setConnected: (connected: boolean) => set({ connected }),
  setConnecting: (connecting: boolean) => set({ connecting }),
  setServerUrl: (serverUrl: string) => set({ serverUrl }),

  setActiveGimbal: (activeGimbalId: string | null) => set({ activeGimbalId }),
  setAvailableGimbals: (availableGimbals: GimbalInfo[]) => set({ availableGimbals }),
  setGimbalMode: (gimbalMode: GimbalMode) => set({ gimbalMode }),

  setPosition: (position: Partial<GimbalPosition>) =>
    set((state) => ({ position: { ...state.position, ...position } })),
  setSpeed: (speed: GimbalSpeed) =>
    set((state) => ({ speed: { ...state.speed, ...speed } })),

  setControlling: (controlling: boolean) => set({ controlling }),
  setTracking: (tracking: boolean) => set({ tracking }),
  setSpeedBoost: (speedBoost: boolean) => set({ speedBoost }),

  setZoom: (zoom: number) => set({ zoom: Math.max(0, Math.min(100, zoom)) }),
  setFocus: (focus: number) => set({ focus: Math.max(0, Math.min(100, focus)) }),

  setSensitivity: (sensitivity: number) => set({ sensitivity }),
  setControlMapping: (mapping: Partial<ControlMapping>) =>
    set((state) => ({
      controlMapping: {
        ...state.controlMapping,
        ...mapping,
        joystickLeft: mapping.joystickLeft
          ? { ...state.controlMapping.joystickLeft, ...mapping.joystickLeft }
          : state.controlMapping.joystickLeft,
        joystickRight: mapping.joystickRight
          ? { ...state.controlMapping.joystickRight, ...mapping.joystickRight }
          : state.controlMapping.joystickRight,
      }
    })),

  addTelemetryData: (data: TelemetryData) =>
    set((state) => ({
      telemetryHistory: [...state.telemetryHistory.slice(-99), data],
    })),
  clearTelemetryHistory: () => set({ telemetryHistory: [] }),
}));
