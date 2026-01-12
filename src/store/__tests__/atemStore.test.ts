import { describe, it, expect, beforeEach } from 'vitest';
import { useAtemStore } from '../atemStore';

describe('atemStore', () => {
  beforeEach(() => {
    // Reset store to initial state
    const store = useAtemStore.getState();
    store.setConfig({ ip: '', connected: false, connecting: false });
    store.setError(null);
    store.setMappings({});
    store.setCameraControl('focus', 50);
    store.setCameraControl('aperture', 50);
    store.setCameraControl('gain', 50);
    store.setCameraControl('zoom', 50);
  });

  describe('Connection config', () => {
    it('should initialize with disconnected state', () => {
      const state = useAtemStore.getState();
      expect(state.config.connected).toBe(false);
      expect(state.config.connecting).toBe(false);
      expect(state.config.ip).toBe('');
    });

    it('should update IP address', () => {
      useAtemStore.getState().setConfig({ ip: '192.168.10.240' });
      expect(useAtemStore.getState().config.ip).toBe('192.168.10.240');
    });

    it('should update connecting state', () => {
      useAtemStore.getState().setConfig({ connecting: true });
      expect(useAtemStore.getState().config.connecting).toBe(true);
    });

    it('should update connected state', () => {
      useAtemStore.getState().setConfig({ connected: true });
      expect(useAtemStore.getState().config.connected).toBe(true);
    });

    it('should clear error when connected', () => {
      useAtemStore.getState().setError('Connection failed');
      expect(useAtemStore.getState().error).toBe('Connection failed');

      useAtemStore.getState().setConfig({ connected: true });
      expect(useAtemStore.getState().error).toBeNull();
    });

    it('should preserve error when not connected', () => {
      useAtemStore.getState().setError('Connection failed');
      useAtemStore.getState().setConfig({ ip: '192.168.1.1' });
      expect(useAtemStore.getState().error).toBe('Connection failed');
    });
  });

  describe('Error handling', () => {
    it('should set error message', () => {
      useAtemStore.getState().setError('Failed to connect to ATEM');
      expect(useAtemStore.getState().error).toBe('Failed to connect to ATEM');
    });

    it('should clear error message', () => {
      useAtemStore.getState().setError('Some error');
      useAtemStore.getState().setError(null);
      expect(useAtemStore.getState().error).toBeNull();
    });
  });

  describe('Gimbal to camera mappings', () => {
    it('should set all mappings at once', () => {
      const mappings = {
        'gimbal-1': { port: 1, cameraType: 1 as const },
        'gimbal-2': { port: 2, cameraType: 0 as const },
      };
      useAtemStore.getState().setMappings(mappings);
      expect(useAtemStore.getState().mappings).toEqual(mappings);
    });

    it('should set individual gimbal mapping', () => {
      useAtemStore.getState().setGimbalMapping('gimbal-1', 3, 1);
      const mappings = useAtemStore.getState().mappings;
      expect(mappings['gimbal-1']).toEqual({ port: 3, cameraType: 1 });
    });

    it('should update existing gimbal mapping', () => {
      useAtemStore.getState().setGimbalMapping('gimbal-1', 1, 0);
      useAtemStore.getState().setGimbalMapping('gimbal-1', 2, 1);
      expect(useAtemStore.getState().mappings['gimbal-1']).toEqual({ port: 2, cameraType: 1 });
    });

    it('should preserve other mappings when setting one', () => {
      useAtemStore.getState().setGimbalMapping('gimbal-1', 1, 0);
      useAtemStore.getState().setGimbalMapping('gimbal-2', 2, 1);
      const mappings = useAtemStore.getState().mappings;
      expect(mappings['gimbal-1']).toEqual({ port: 1, cameraType: 0 });
      expect(mappings['gimbal-2']).toEqual({ port: 2, cameraType: 1 });
    });

    it('should remove gimbal mapping', () => {
      useAtemStore.getState().setGimbalMapping('gimbal-1', 1, 0);
      useAtemStore.getState().setGimbalMapping('gimbal-2', 2, 1);
      useAtemStore.getState().removeGimbalMapping('gimbal-1');

      const mappings = useAtemStore.getState().mappings;
      expect(mappings['gimbal-1']).toBeUndefined();
      expect(mappings['gimbal-2']).toEqual({ port: 2, cameraType: 1 });
    });
  });

  describe('Camera controls', () => {
    it('should initialize with default values', () => {
      const controls = useAtemStore.getState().cameraControls;
      expect(controls.focus).toBe(50);
      expect(controls.aperture).toBe(50);
      expect(controls.gain).toBe(50);
      expect(controls.zoom).toBe(50);
    });

    it('should update focus control', () => {
      useAtemStore.getState().setCameraControl('focus', 75);
      expect(useAtemStore.getState().cameraControls.focus).toBe(75);
    });

    it('should update aperture control', () => {
      useAtemStore.getState().setCameraControl('aperture', 30);
      expect(useAtemStore.getState().cameraControls.aperture).toBe(30);
    });

    it('should update gain control', () => {
      useAtemStore.getState().setCameraControl('gain', 80);
      expect(useAtemStore.getState().cameraControls.gain).toBe(80);
    });

    it('should update zoom control', () => {
      useAtemStore.getState().setCameraControl('zoom', 100);
      expect(useAtemStore.getState().cameraControls.zoom).toBe(100);
    });

    it('should clamp focus to 0-100', () => {
      useAtemStore.getState().setCameraControl('focus', 150);
      expect(useAtemStore.getState().cameraControls.focus).toBe(100);

      useAtemStore.getState().setCameraControl('focus', -20);
      expect(useAtemStore.getState().cameraControls.focus).toBe(0);
    });

    it('should clamp aperture to 0-100', () => {
      useAtemStore.getState().setCameraControl('aperture', 200);
      expect(useAtemStore.getState().cameraControls.aperture).toBe(100);
    });

    it('should preserve other controls when updating one', () => {
      useAtemStore.getState().setCameraControl('focus', 20);
      useAtemStore.getState().setCameraControl('aperture', 80);

      const controls = useAtemStore.getState().cameraControls;
      expect(controls.focus).toBe(20);
      expect(controls.aperture).toBe(80);
      expect(controls.gain).toBe(50); // unchanged
      expect(controls.zoom).toBe(50); // unchanged
    });
  });
});
