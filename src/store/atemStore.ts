import { create } from 'zustand';
import type { AtemConfig, AtemMappings, AtemCameraTypeNum } from '../types';

interface AtemState {
  // Connection
  config: AtemConfig;

  // Error message
  error: string | null;

  // Mappings: gimbalId -> { port, cameraType }
  mappings: AtemMappings;

  // Current camera controls for the active gimbal's ATEM port
  cameraControls: {
    focus: number;
    aperture: number;
    gain: number;
    zoom: number;
  };
}

interface AtemActions {
  setConfig: (config: Partial<AtemConfig>) => void;
  setError: (error: string | null) => void;
  setMappings: (mappings: AtemMappings) => void;
  setGimbalMapping: (gimbalId: string, port: number, cameraType: AtemCameraTypeNum) => void;
  removeGimbalMapping: (gimbalId: string) => void;
  setCameraControl: (control: 'focus' | 'aperture' | 'gain' | 'zoom', value: number) => void;
}

type AtemStore = AtemState & AtemActions;

export const useAtemStore = create<AtemStore>((set) => ({
  // Initial state
  config: {
    ip: '',
    connected: false,
    connecting: false,
  },
  error: null,
  mappings: {},
  cameraControls: {
    focus: 50,
    aperture: 50,
    gain: 50,
    zoom: 50,
  },

  // Actions
  setConfig: (config) =>
    set((state) => ({
      config: { ...state.config, ...config },
      // Clear error when connected
      error: config.connected ? null : state.error,
    })),

  setError: (error) => set({ error }),

  setMappings: (mappings) => set({ mappings }),

  setGimbalMapping: (gimbalId, port, cameraType) =>
    set((state) => ({
      mappings: {
        ...state.mappings,
        [gimbalId]: { port, cameraType },
      },
    })),

  removeGimbalMapping: (gimbalId) =>
    set((state) => {
      const newMappings = { ...state.mappings };
      delete newMappings[gimbalId];
      return { mappings: newMappings };
    }),

  setCameraControl: (control, value) =>
    set((state) => ({
      cameraControls: {
        ...state.cameraControls,
        [control]: Math.max(0, Math.min(100, value)),
      },
    })),
}));
