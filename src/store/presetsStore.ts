import { create } from 'zustand';
import type { GimbalPosition } from '../types';

// Preset structure: gimbalId -> { "1": position, "2": position, ... }
type PresetMap = Record<string, GimbalPosition>;
type AllPresets = Record<string, PresetMap>;

interface PresetsState {
  // All presets by gimbal ID
  presets: AllPresets;
}

interface PresetsActions {
  setPresets: (gimbalId: string, presets: PresetMap) => void;
  getPresetsForGimbal: (gimbalId: string) => PresetMap;
  clearPresets: () => void;
}

type PresetsStore = PresetsState & PresetsActions;

export const usePresetsStore = create<PresetsStore>((set, get) => ({
  // Initial state
  presets: {},

  // Actions
  setPresets: (gimbalId, presets) =>
    set((state) => ({
      presets: {
        ...state.presets,
        [gimbalId]: presets,
      },
    })),

  getPresetsForGimbal: (gimbalId) => {
    return get().presets[gimbalId] || {};
  },

  clearPresets: () => set({ presets: {} }),
}));
