import { describe, it, expect, beforeEach } from 'vitest';
import { usePresetsStore } from '../presetsStore';

describe('presetsStore', () => {
  beforeEach(() => {
    // Reset store to initial state
    usePresetsStore.getState().clearPresets();
  });

  describe('Initial state', () => {
    it('should initialize with empty presets', () => {
      const state = usePresetsStore.getState();
      expect(state.presets).toEqual({});
    });
  });

  describe('setPresets', () => {
    it('should set presets for a gimbal', () => {
      const gimbalId = 'gimbal-1';
      const presets = {
        '1': { pitch: 10, yaw: 20, roll: 0 },
        '2': { pitch: -15, yaw: 45, roll: 5 },
      };

      usePresetsStore.getState().setPresets(gimbalId, presets);
      expect(usePresetsStore.getState().presets[gimbalId]).toEqual(presets);
    });

    it('should update existing presets for a gimbal', () => {
      const gimbalId = 'gimbal-1';
      usePresetsStore.getState().setPresets(gimbalId, {
        '1': { pitch: 10, yaw: 20, roll: 0 },
      });

      usePresetsStore.getState().setPresets(gimbalId, {
        '1': { pitch: 30, yaw: 40, roll: 0 },
        '2': { pitch: 50, yaw: 60, roll: 0 },
      });

      const presets = usePresetsStore.getState().presets[gimbalId];
      expect(presets['1']).toEqual({ pitch: 30, yaw: 40, roll: 0 });
      expect(presets['2']).toEqual({ pitch: 50, yaw: 60, roll: 0 });
    });

    it('should preserve presets for other gimbals', () => {
      usePresetsStore.getState().setPresets('gimbal-1', {
        '1': { pitch: 10, yaw: 20, roll: 0 },
      });

      usePresetsStore.getState().setPresets('gimbal-2', {
        '1': { pitch: 100, yaw: 200, roll: 0 },
      });

      const state = usePresetsStore.getState();
      expect(state.presets['gimbal-1']['1']).toEqual({ pitch: 10, yaw: 20, roll: 0 });
      expect(state.presets['gimbal-2']['1']).toEqual({ pitch: 100, yaw: 200, roll: 0 });
    });
  });

  describe('getPresetsForGimbal', () => {
    it('should return presets for existing gimbal', () => {
      const gimbalId = 'gimbal-1';
      const presets = {
        '1': { pitch: 10, yaw: 20, roll: 0 },
        '3': { pitch: 30, yaw: 40, roll: 5 },
      };

      usePresetsStore.getState().setPresets(gimbalId, presets);
      const result = usePresetsStore.getState().getPresetsForGimbal(gimbalId);
      expect(result).toEqual(presets);
    });

    it('should return empty object for non-existent gimbal', () => {
      const result = usePresetsStore.getState().getPresetsForGimbal('non-existent');
      expect(result).toEqual({});
    });

    it('should return empty object for gimbal with no presets', () => {
      usePresetsStore.getState().setPresets('gimbal-1', {});
      const result = usePresetsStore.getState().getPresetsForGimbal('gimbal-1');
      expect(result).toEqual({});
    });
  });

  describe('clearPresets', () => {
    it('should clear all presets', () => {
      usePresetsStore.getState().setPresets('gimbal-1', {
        '1': { pitch: 10, yaw: 20, roll: 0 },
      });
      usePresetsStore.getState().setPresets('gimbal-2', {
        '1': { pitch: 30, yaw: 40, roll: 0 },
      });

      usePresetsStore.getState().clearPresets();
      expect(usePresetsStore.getState().presets).toEqual({});
    });
  });

  describe('Preset position data', () => {
    it('should store all position axes correctly', () => {
      const position = { pitch: 45.5, yaw: -90.25, roll: 15.75 };
      usePresetsStore.getState().setPresets('gimbal-1', { '1': position });

      const stored = usePresetsStore.getState().getPresetsForGimbal('gimbal-1')['1'];
      expect(stored.pitch).toBe(45.5);
      expect(stored.yaw).toBe(-90.25);
      expect(stored.roll).toBe(15.75);
    });

    it('should support up to 9 presets per gimbal', () => {
      const presets: Record<string, { pitch: number; yaw: number; roll: number }> = {};
      for (let i = 1; i <= 9; i++) {
        presets[String(i)] = { pitch: i * 10, yaw: i * 20, roll: 0 };
      }

      usePresetsStore.getState().setPresets('gimbal-1', presets);
      const stored = usePresetsStore.getState().getPresetsForGimbal('gimbal-1');
      expect(Object.keys(stored)).toHaveLength(9);
      expect(stored['9']).toEqual({ pitch: 90, yaw: 180, roll: 0 });
    });
  });
});
