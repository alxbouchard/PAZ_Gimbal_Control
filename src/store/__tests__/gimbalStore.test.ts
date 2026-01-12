import { describe, it, expect, beforeEach } from 'vitest';
import { useGimbalStore } from '../gimbalStore';

describe('gimbalStore', () => {
  beforeEach(() => {
    // Reset store to initial state before each test
    const store = useGimbalStore.getState();
    store.setConnected(false);
    store.setConnecting(false);
    store.setActiveGimbal(null);
    store.setAvailableGimbals([]);
    store.setPosition({ pitch: 0, yaw: 0, roll: 0 });
    store.setSpeed({ pitch: 0, yaw: 0, roll: 0 });
    store.setTracking(false);
    store.setSpeedBoost(false);
    store.setZoom(50);
    store.setFocus(50);
    store.setSensitivity(1.0);
    store.clearTelemetryHistory();
  });

  describe('Connection state', () => {
    it('should initialize with disconnected state', () => {
      const state = useGimbalStore.getState();
      expect(state.connected).toBe(false);
      expect(state.connecting).toBe(false);
    });

    it('should update connected state', () => {
      useGimbalStore.getState().setConnected(true);
      expect(useGimbalStore.getState().connected).toBe(true);
    });

    it('should update connecting state', () => {
      useGimbalStore.getState().setConnecting(true);
      expect(useGimbalStore.getState().connecting).toBe(true);
    });

    it('should update server URL', () => {
      useGimbalStore.getState().setServerUrl('http://192.168.1.100:3001');
      expect(useGimbalStore.getState().serverUrl).toBe('http://192.168.1.100:3001');
    });

    it('should update connection mode', () => {
      useGimbalStore.getState().setConnectionMode('master');
      expect(useGimbalStore.getState().connectionMode).toBe('master');
    });

    it('should update client identity', () => {
      const identity = { name: 'Test User', sid: 'test-123' };
      useGimbalStore.getState().setClientIdentity(identity);
      expect(useGimbalStore.getState().clientIdentity).toEqual(identity);
    });
  });

  describe('Gimbal selection', () => {
    it('should set active gimbal', () => {
      useGimbalStore.getState().setActiveGimbal('gimbal-1');
      expect(useGimbalStore.getState().activeGimbalId).toBe('gimbal-1');
    });

    it('should set available gimbals', () => {
      const gimbals = [
        { id: 'gimbal-1', name: 'PTZ 1', model: 'DJI RS3', ip: '192.168.1.10', mode: 'real' as const, connected: true },
        { id: 'gimbal-2', name: 'PTZ 2', model: 'DJI RS3', ip: '192.168.1.11', mode: 'real' as const, connected: false },
      ];
      useGimbalStore.getState().setAvailableGimbals(gimbals);
      expect(useGimbalStore.getState().availableGimbals).toHaveLength(2);
      expect(useGimbalStore.getState().availableGimbals[0].name).toBe('PTZ 1');
    });

    it('should set gimbal mode', () => {
      useGimbalStore.getState().setGimbalMode('real');
      expect(useGimbalStore.getState().gimbalMode).toBe('real');
    });
  });

  describe('Position and movement', () => {
    it('should initialize with zero position', () => {
      const state = useGimbalStore.getState();
      expect(state.position).toEqual({ pitch: 0, yaw: 0, roll: 0 });
    });

    it('should update position partially', () => {
      useGimbalStore.getState().setPosition({ pitch: 45 });
      const state = useGimbalStore.getState();
      expect(state.position.pitch).toBe(45);
      expect(state.position.yaw).toBe(0);
      expect(state.position.roll).toBe(0);
    });

    it('should update full position', () => {
      useGimbalStore.getState().setPosition({ pitch: 30, yaw: 60, roll: -15 });
      const state = useGimbalStore.getState();
      expect(state.position).toEqual({ pitch: 30, yaw: 60, roll: -15 });
    });

    it('should update speed', () => {
      useGimbalStore.getState().setSpeed({ pitch: 10, yaw: -5, roll: 0 });
      expect(useGimbalStore.getState().speed).toEqual({ pitch: 10, yaw: -5, roll: 0 });
    });
  });

  describe('Control states', () => {
    it('should toggle tracking', () => {
      useGimbalStore.getState().setTracking(true);
      expect(useGimbalStore.getState().tracking).toBe(true);

      useGimbalStore.getState().setTracking(false);
      expect(useGimbalStore.getState().tracking).toBe(false);
    });

    it('should toggle speed boost', () => {
      useGimbalStore.getState().setSpeedBoost(true);
      expect(useGimbalStore.getState().speedBoost).toBe(true);
    });

    it('should toggle controlling state', () => {
      useGimbalStore.getState().setControlling(true);
      expect(useGimbalStore.getState().controlling).toBe(true);
    });
  });

  describe('Camera controls', () => {
    it('should update zoom within bounds', () => {
      useGimbalStore.getState().setZoom(75);
      expect(useGimbalStore.getState().zoom).toBe(75);
    });

    it('should clamp zoom to 0-100', () => {
      useGimbalStore.getState().setZoom(150);
      expect(useGimbalStore.getState().zoom).toBe(100);

      useGimbalStore.getState().setZoom(-10);
      expect(useGimbalStore.getState().zoom).toBe(0);
    });

    it('should update focus within bounds', () => {
      useGimbalStore.getState().setFocus(25);
      expect(useGimbalStore.getState().focus).toBe(25);
    });

    it('should clamp focus to 0-100', () => {
      useGimbalStore.getState().setFocus(200);
      expect(useGimbalStore.getState().focus).toBe(100);

      useGimbalStore.getState().setFocus(-50);
      expect(useGimbalStore.getState().focus).toBe(0);
    });
  });

  describe('Settings', () => {
    it('should update sensitivity', () => {
      useGimbalStore.getState().setSensitivity(1.5);
      expect(useGimbalStore.getState().sensitivity).toBe(1.5);
    });

    it('should update control mapping partially', () => {
      useGimbalStore.getState().setControlMapping({ invertPitch: false });
      const mapping = useGimbalStore.getState().controlMapping;
      expect(mapping.invertPitch).toBe(false);
      // Other values should remain
      expect(mapping.invertYaw).toBe(true);
    });

    it('should update joystick mapping', () => {
      useGimbalStore.getState().setControlMapping({
        joystickLeft: { x: 'roll', y: 'pitch' },
      });
      const mapping = useGimbalStore.getState().controlMapping;
      expect(mapping.joystickLeft.x).toBe('roll');
      expect(mapping.joystickLeft.y).toBe('pitch');
    });
  });

  describe('Telemetry', () => {
    it('should add telemetry data', () => {
      const data = {
        timestamp: Date.now(),
        position: { pitch: 10, yaw: 20, roll: 0 },
        speed: { pitch: 1, yaw: 2, roll: 0 },
        temperature: 35,
        batteryLevel: 85,
      };
      useGimbalStore.getState().addTelemetryData(data);
      expect(useGimbalStore.getState().telemetryHistory).toHaveLength(1);
    });

    it('should limit telemetry history to 100 entries', () => {
      const store = useGimbalStore.getState();
      for (let i = 0; i < 150; i++) {
        store.addTelemetryData({
          timestamp: Date.now() + i,
          position: { pitch: i, yaw: 0, roll: 0 },
          speed: { pitch: 0, yaw: 0, roll: 0 },
          temperature: 35,
          batteryLevel: 85,
        });
      }
      expect(useGimbalStore.getState().telemetryHistory).toHaveLength(100);
    });

    it('should clear telemetry history', () => {
      useGimbalStore.getState().addTelemetryData({
        timestamp: Date.now(),
        position: { pitch: 0, yaw: 0, roll: 0 },
        speed: { pitch: 0, yaw: 0, roll: 0 },
        temperature: 35,
        batteryLevel: 85,
      });
      useGimbalStore.getState().clearTelemetryHistory();
      expect(useGimbalStore.getState().telemetryHistory).toHaveLength(0);
    });
  });
});
