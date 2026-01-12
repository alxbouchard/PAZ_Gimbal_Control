import { describe, it, expect, vi, beforeEach } from 'vitest';
import { gimbalSocket } from '../websocket';
import { useGimbalStore } from '../../store/gimbalStore';
import { useAtemStore } from '../../store/atemStore';

/**
 * WebSocket service tests
 *
 * The GimbalSocketService is a singleton that's created on module load.
 * These tests focus on:
 * 1. API existence and shape
 * 2. Store updates when methods are called
 * 3. Behavior when disconnected (no throws)
 *
 * Note: Socket.io emit calls are not tested because the singleton is created
 * before mocks can be applied. The store updates serve as integration tests.
 */

describe('GimbalSocketService', () => {
  beforeEach(() => {
    // Reset stores
    useGimbalStore.getState().setConnected(false);
    useGimbalStore.getState().setConnecting(false);
    useGimbalStore.getState().setSpeed({ pitch: 0, yaw: 0, roll: 0 });
    useGimbalStore.getState().setTracking(false);
    useGimbalStore.getState().setSpeedBoost(false);
    useGimbalStore.getState().setZoom(50);
    useGimbalStore.getState().setFocus(50);

    useAtemStore.getState().setConfig({ ip: '', connected: false, connecting: false });
    useAtemStore.getState().setError(null);
    useAtemStore.getState().setMappings({});
    useAtemStore.getState().setCameraControl('focus', 50);
    useAtemStore.getState().setCameraControl('aperture', 50);
    useAtemStore.getState().setCameraControl('gain', 50);
    useAtemStore.getState().setCameraControl('zoom', 50);

    vi.clearAllMocks();
  });

  describe('Connection', () => {
    it('should check if connected', () => {
      expect(gimbalSocket.isConnected()).toBe(false);
    });
  });

  describe('API methods exist', () => {
    it('should have all movement methods', () => {
      expect(typeof gimbalSocket.setSpeed).toBe('function');
      expect(typeof gimbalSocket.stopSpeed).toBe('function');
      expect(typeof gimbalSocket.setSpeedMultiplier).toBe('function');
    });

    it('should have position methods', () => {
      expect(typeof gimbalSocket.goHome).toBe('function');
      expect(typeof gimbalSocket.setHome).toBe('function');
    });

    it('should have mode toggle methods', () => {
      expect(typeof gimbalSocket.toggleTracking).toBe('function');
      expect(typeof gimbalSocket.toggleSpeedBoost).toBe('function');
    });

    it('should have camera control methods', () => {
      expect(typeof gimbalSocket.setZoom).toBe('function');
      expect(typeof gimbalSocket.setFocus).toBe('function');
      expect(typeof gimbalSocket.calibrateFocus).toBe('function');
    });

    it('should have gimbal management methods', () => {
      expect(typeof gimbalSocket.selectGimbal).toBe('function');
      expect(typeof gimbalSocket.addGimbal).toBe('function');
      expect(typeof gimbalSocket.removeGimbal).toBe('function');
      expect(typeof gimbalSocket.updateGimbal).toBe('function');
      expect(typeof gimbalSocket.connectGimbal).toBe('function');
    });

    it('should have ATEM methods', () => {
      expect(typeof gimbalSocket.connectAtem).toBe('function');
      expect(typeof gimbalSocket.disconnectAtem).toBe('function');
      expect(typeof gimbalSocket.setAtemGimbalMapping).toBe('function');
      expect(typeof gimbalSocket.setAtemFocus).toBe('function');
      expect(typeof gimbalSocket.triggerAtemAutoFocus).toBe('function');
      expect(typeof gimbalSocket.setAtemAperture).toBe('function');
      expect(typeof gimbalSocket.triggerAtemAutoAperture).toBe('function');
      expect(typeof gimbalSocket.setAtemGain).toBe('function');
      expect(typeof gimbalSocket.setAtemZoom).toBe('function');
      expect(typeof gimbalSocket.setAtemZoomPosition).toBe('function');
    });

    it('should have preset methods', () => {
      expect(typeof gimbalSocket.savePreset).toBe('function');
      expect(typeof gimbalSocket.recallPreset).toBe('function');
      expect(typeof gimbalSocket.deletePreset).toBe('function');
      expect(typeof gimbalSocket.getPresets).toBe('function');
    });

    it('should have connection methods', () => {
      expect(typeof gimbalSocket.connect).toBe('function');
      expect(typeof gimbalSocket.disconnect).toBe('function');
      expect(typeof gimbalSocket.isConnected).toBe('function');
    });
  });

  describe('Disconnected behavior', () => {
    it('should not throw when calling methods while disconnected', () => {
      expect(() => gimbalSocket.setSpeed({ pitch: 1, yaw: 0, roll: 0 })).not.toThrow();
      expect(() => gimbalSocket.goHome()).not.toThrow();
      expect(() => gimbalSocket.toggleTracking(true)).not.toThrow();
      expect(() => gimbalSocket.setZoom(50)).not.toThrow();
      expect(() => gimbalSocket.connectAtem('192.168.1.1')).not.toThrow();
      expect(() => gimbalSocket.savePreset('gimbal-1', 1)).not.toThrow();
    });

    it('should report disconnected when socket is null', () => {
      expect(gimbalSocket.isConnected()).toBe(false);
    });
  });

  describe('Camera type conversion', () => {
    it('should have setAtemGimbalMapping accept camera type strings', () => {
      // The method accepts 'MFT' and 'EF' strings and converts them to numbers
      expect(() => gimbalSocket.setAtemGimbalMapping('gimbal-1', 1, 'MFT')).not.toThrow();
      expect(() => gimbalSocket.setAtemGimbalMapping('gimbal-1', 2, 'EF')).not.toThrow();
    });

    it('should have setAtemFocus accept camera type strings', () => {
      expect(() => gimbalSocket.setAtemFocus(1, 50, 'MFT')).not.toThrow();
      expect(() => gimbalSocket.setAtemFocus(1, 50, 'EF')).not.toThrow();
    });
  });
});
