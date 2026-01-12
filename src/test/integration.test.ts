import { describe, it, expect, beforeEach } from 'vitest';
import { useGimbalStore } from '../store/gimbalStore';
import { useAtemStore } from '../store/atemStore';
import { usePresetsStore } from '../store/presetsStore';
import { useShortcutsStore } from '../store/shortcutsStore';
import { useGamepadStore } from '../store/gamepadStore';

/**
 * Integration tests for PAZ Gimbal Control
 *
 * These tests verify that different parts of the application work together correctly.
 */

describe('Integration Tests', () => {
  beforeEach(() => {
    // Reset all stores to initial state
    useGimbalStore.getState().setConnected(false);
    useGimbalStore.getState().setConnecting(false);
    useGimbalStore.getState().setActiveGimbal(null);
    useGimbalStore.getState().setAvailableGimbals([]);
    useGimbalStore.getState().setPosition({ pitch: 0, yaw: 0, roll: 0 });
    useGimbalStore.getState().setSensitivity(1.0);
    useGimbalStore.getState().setTracking(false);
    useGimbalStore.getState().setSpeedBoost(false);
    useGimbalStore.getState().clearTelemetryHistory();

    useAtemStore.getState().setConfig({ ip: '', connected: false, connecting: false });
    useAtemStore.getState().setError(null);
    useAtemStore.getState().setMappings({});

    usePresetsStore.getState().clearPresets();

    useShortcutsStore.getState().resetAllBindings();

    useGamepadStore.getState().resetToDefaults();
  });

  describe('Gimbal and ATEM Integration', () => {
    it('should associate ATEM camera mapping with active gimbal', () => {
      // Set up gimbals
      useGimbalStore.getState().setAvailableGimbals([
        { id: 'gimbal-1', name: 'PTZ 1', model: 'DJI RS3', ip: '192.168.1.10', mode: 'real', connected: true },
        { id: 'gimbal-2', name: 'PTZ 2', model: 'DJI RS3', ip: '192.168.1.11', mode: 'real', connected: true },
      ]);
      useGimbalStore.getState().setActiveGimbal('gimbal-1');

      // Set up ATEM mapping
      useAtemStore.getState().setGimbalMapping('gimbal-1', 1, 1); // Camera 1, MFT
      useAtemStore.getState().setGimbalMapping('gimbal-2', 2, 0); // Camera 2, EF

      // Verify mapping for active gimbal
      const activeGimbalId = useGimbalStore.getState().activeGimbalId;
      const mapping = useAtemStore.getState().mappings[activeGimbalId!];

      expect(mapping).toBeDefined();
      expect(mapping.port).toBe(1);
      expect(mapping.cameraType).toBe(1);
    });

    it('should get correct camera type for active gimbal', () => {
      useGimbalStore.getState().setActiveGimbal('gimbal-1');
      useAtemStore.getState().setGimbalMapping('gimbal-1', 3, 0); // EF type

      const activeGimbalId = useGimbalStore.getState().activeGimbalId;
      const mapping = useAtemStore.getState().mappings[activeGimbalId!];

      expect(mapping.cameraType).toBe(0); // EF
    });
  });

  describe('Gimbal and Presets Integration', () => {
    it('should maintain presets per gimbal', () => {
      // Set up gimbals
      useGimbalStore.getState().setAvailableGimbals([
        { id: 'gimbal-1', name: 'PTZ 1', model: 'DJI RS3', ip: '192.168.1.10', mode: 'real', connected: true },
        { id: 'gimbal-2', name: 'PTZ 2', model: 'DJI RS3', ip: '192.168.1.11', mode: 'real', connected: true },
      ]);

      // Set presets for each gimbal
      usePresetsStore.getState().setPresets('gimbal-1', {
        '1': { pitch: 10, yaw: 20, roll: 0 },
        '2': { pitch: 30, yaw: 40, roll: 0 },
      });

      usePresetsStore.getState().setPresets('gimbal-2', {
        '1': { pitch: -10, yaw: -20, roll: 0 },
      });

      // Verify presets are separate
      const presets1 = usePresetsStore.getState().getPresetsForGimbal('gimbal-1');
      const presets2 = usePresetsStore.getState().getPresetsForGimbal('gimbal-2');

      expect(Object.keys(presets1).length).toBe(2);
      expect(Object.keys(presets2).length).toBe(1);
      expect(presets1['1'].pitch).toBe(10);
      expect(presets2['1'].pitch).toBe(-10);
    });

    it('should switch presets when active gimbal changes', () => {
      useGimbalStore.getState().setActiveGimbal('gimbal-1');
      usePresetsStore.getState().setPresets('gimbal-1', {
        '1': { pitch: 45, yaw: 90, roll: 0 },
      });
      usePresetsStore.getState().setPresets('gimbal-2', {
        '1': { pitch: -45, yaw: -90, roll: 0 },
      });

      // Get presets for gimbal-1
      let activeId = useGimbalStore.getState().activeGimbalId;
      let presets = usePresetsStore.getState().getPresetsForGimbal(activeId!);
      expect(presets['1'].pitch).toBe(45);

      // Switch to gimbal-2
      useGimbalStore.getState().setActiveGimbal('gimbal-2');
      activeId = useGimbalStore.getState().activeGimbalId;
      presets = usePresetsStore.getState().getPresetsForGimbal(activeId!);
      expect(presets['1'].pitch).toBe(-45);
    });
  });

  describe('Keyboard Shortcuts and Store Integration', () => {
    it('should find correct action for default bindings', () => {
      const action = useShortcutsStore.getState().getActionForKey('w', {
        shift: false,
        ctrl: false,
        alt: false,
        meta: false,
      });
      expect(action).toBe('pitchUp');
    });

    it('should handle modifier keys correctly', () => {
      // 'h' without shift = goHome
      const goHome = useShortcutsStore.getState().getActionForKey('h', {
        shift: false,
        ctrl: false,
        alt: false,
        meta: false,
      });
      expect(goHome).toBe('goHome');

      // 'h' with shift = setHome
      const setHome = useShortcutsStore.getState().getActionForKey('h', {
        shift: true,
        ctrl: false,
        alt: false,
        meta: false,
      });
      expect(setHome).toBe('setHome');
    });

    it('should respect custom bindings', () => {
      // Change pitchUp from 'w' to 'i'
      useShortcutsStore.getState().setBinding('pitchUp', { key: 'i' });

      // 'w' should no longer trigger pitchUp
      const wAction = useShortcutsStore.getState().getActionForKey('w', {
        shift: false,
        ctrl: false,
        alt: false,
        meta: false,
      });
      expect(wAction).toBeNull();

      // 'i' should now trigger pitchUp
      const iAction = useShortcutsStore.getState().getActionForKey('i', {
        shift: false,
        ctrl: false,
        alt: false,
        meta: false,
      });
      expect(iAction).toBe('pitchUp');
    });
  });

  describe('Gamepad and Settings Integration', () => {
    it('should apply deadzone to all axes', () => {
      const deadzone = useGamepadStore.getState().mapping.deadzone;

      // Value below deadzone
      const belowDeadzone = deadzone - 0.01;
      const deadzoned = Math.abs(belowDeadzone) < deadzone ? 0 : belowDeadzone;
      expect(deadzoned).toBe(0);

      // Value above deadzone
      const aboveDeadzone = deadzone + 0.1;
      const notDeadzoned = Math.abs(aboveDeadzone) < deadzone ? 0 : aboveDeadzone;
      expect(notDeadzoned).toBeGreaterThan(0);
    });

    it('should combine axis sensitivity with global sensitivity', () => {
      const globalSensitivity = useGimbalStore.getState().sensitivity;
      const axisSensitivity = useGamepadStore.getState().mapping.axes[0].sensitivity;

      const combinedSensitivity = globalSensitivity * axisSensitivity;
      expect(combinedSensitivity).toBe(1.0); // 1.0 * 1.0
    });

    it('should support ATEM gamepad actions', () => {
      // Set up ATEM actions
      useGamepadStore.getState().setButtonMapping(10, 'atemAutoFocus');
      useGamepadStore.getState().setButtonMapping(11, 'atemAutoAperture');
      useGamepadStore.getState().setAxisMapping(2, { action: 'atemFocus' });
      useGamepadStore.getState().setAxisMapping(3, { action: 'atemZoom' });

      const mapping = useGamepadStore.getState().mapping;
      expect(mapping.buttons[10]).toBe('atemAutoFocus');
      expect(mapping.buttons[11]).toBe('atemAutoAperture');
      expect(mapping.axes[2].action).toBe('atemFocus');
      expect(mapping.axes[3].action).toBe('atemZoom');
    });
  });

  describe('Multi-Gimbal Workflow', () => {
    beforeEach(() => {
      // Set up multiple gimbals
      useGimbalStore.getState().setAvailableGimbals([
        { id: 'gimbal-1', name: 'Camera 1', model: 'DJI RS3', ip: '192.168.1.10', mode: 'real', connected: true },
        { id: 'gimbal-2', name: 'Camera 2', model: 'DJI RS3', ip: '192.168.1.11', mode: 'real', connected: true },
        { id: 'gimbal-3', name: 'Camera 3', model: 'DJI RS3', ip: '192.168.1.12', mode: 'real', connected: false },
      ]);
      useGimbalStore.getState().setActiveGimbal('gimbal-1');

      // Set up ATEM for each
      useAtemStore.getState().setGimbalMapping('gimbal-1', 1, 1);
      useAtemStore.getState().setGimbalMapping('gimbal-2', 2, 0);
      useAtemStore.getState().setGimbalMapping('gimbal-3', 3, 1);

      // Set up presets for each
      usePresetsStore.getState().setPresets('gimbal-1', {
        '1': { pitch: 0, yaw: 0, roll: 0 },
      });
      usePresetsStore.getState().setPresets('gimbal-2', {
        '1': { pitch: 45, yaw: 90, roll: 0 },
      });
    });

    it('should cycle through gimbals correctly', () => {
      const gimbals = useGimbalStore.getState().availableGimbals;

      // Start at gimbal-1
      expect(useGimbalStore.getState().activeGimbalId).toBe('gimbal-1');

      // Cycle to next gimbal
      let currentIndex = gimbals.findIndex((g) => g.id === 'gimbal-1');
      let nextIndex = (currentIndex + 1) % gimbals.length;
      useGimbalStore.getState().setActiveGimbal(gimbals[nextIndex].id);
      expect(useGimbalStore.getState().activeGimbalId).toBe('gimbal-2');

      // Cycle again
      currentIndex = nextIndex;
      nextIndex = (currentIndex + 1) % gimbals.length;
      useGimbalStore.getState().setActiveGimbal(gimbals[nextIndex].id);
      expect(useGimbalStore.getState().activeGimbalId).toBe('gimbal-3');

      // Cycle back to first
      currentIndex = nextIndex;
      nextIndex = (currentIndex + 1) % gimbals.length;
      useGimbalStore.getState().setActiveGimbal(gimbals[nextIndex].id);
      expect(useGimbalStore.getState().activeGimbalId).toBe('gimbal-1');
    });

    it('should maintain context when switching gimbals', () => {
      // Verify gimbal-1 context
      let activeId = useGimbalStore.getState().activeGimbalId;
      let atemMapping = useAtemStore.getState().mappings[activeId!];
      let presets = usePresetsStore.getState().getPresetsForGimbal(activeId!);

      expect(atemMapping.port).toBe(1);
      expect(presets['1'].pitch).toBe(0);

      // Switch to gimbal-2
      useGimbalStore.getState().setActiveGimbal('gimbal-2');

      // Verify gimbal-2 context
      activeId = useGimbalStore.getState().activeGimbalId;
      atemMapping = useAtemStore.getState().mappings[activeId!];
      presets = usePresetsStore.getState().getPresetsForGimbal(activeId!);

      expect(atemMapping.port).toBe(2);
      expect(presets['1'].pitch).toBe(45);
    });
  });

  describe('Telemetry Integration', () => {
    it('should track position updates in telemetry', () => {
      // Add telemetry data
      for (let i = 0; i < 10; i++) {
        useGimbalStore.getState().addTelemetryData({
          timestamp: Date.now() + i * 100,
          position: { pitch: i * 5, yaw: i * 10, roll: 0 },
          speed: { pitch: 1, yaw: 2, roll: 0 },
          temperature: 35,
          batteryLevel: 85,
        });
      }

      const history = useGimbalStore.getState().telemetryHistory;
      expect(history.length).toBe(10);
      expect(history[9].position.pitch).toBe(45);
      expect(history[9].position.yaw).toBe(90);
    });

    it('should limit telemetry history to 100 entries', () => {
      // Add more than 100 entries
      for (let i = 0; i < 150; i++) {
        useGimbalStore.getState().addTelemetryData({
          timestamp: Date.now() + i * 100,
          position: { pitch: i, yaw: i * 2, roll: 0 },
          speed: { pitch: 0, yaw: 0, roll: 0 },
          temperature: 35,
          batteryLevel: 85,
        });
      }

      const history = useGimbalStore.getState().telemetryHistory;
      expect(history.length).toBe(100);

      // Should have the most recent entries (50-149)
      expect(history[0].position.pitch).toBe(50);
      expect(history[99].position.pitch).toBe(149);
    });
  });

  describe('Connection State Integration', () => {
    it('should handle both gimbal and ATEM connection states', () => {
      // Both disconnected
      expect(useGimbalStore.getState().connected).toBe(false);
      expect(useAtemStore.getState().config.connected).toBe(false);

      // Connect gimbal only
      useGimbalStore.getState().setConnected(true);
      expect(useGimbalStore.getState().connected).toBe(true);
      expect(useAtemStore.getState().config.connected).toBe(false);

      // Connect ATEM
      useAtemStore.getState().setConfig({ connected: true });
      expect(useGimbalStore.getState().connected).toBe(true);
      expect(useAtemStore.getState().config.connected).toBe(true);

      // Disconnect gimbal
      useGimbalStore.getState().setConnected(false);
      expect(useGimbalStore.getState().connected).toBe(false);
      expect(useAtemStore.getState().config.connected).toBe(true);
    });

    it('should clear ATEM error on successful connection', () => {
      useAtemStore.getState().setError('Connection failed');
      expect(useAtemStore.getState().error).toBe('Connection failed');

      useAtemStore.getState().setConfig({ connected: true });
      expect(useAtemStore.getState().error).toBeNull();
    });
  });

  describe('Control Mapping Integration', () => {
    it('should apply invert settings to controls', () => {
      // Default invert settings
      const mapping = useGimbalStore.getState().controlMapping;
      expect(mapping.invertPitch).toBe(true);
      expect(mapping.invertYaw).toBe(true);
      expect(mapping.invertRoll).toBe(false);

      // Change settings
      useGimbalStore.getState().setControlMapping({
        invertPitch: false,
        invertYaw: false,
        invertRoll: true,
      });

      const updatedMapping = useGimbalStore.getState().controlMapping;
      expect(updatedMapping.invertPitch).toBe(false);
      expect(updatedMapping.invertYaw).toBe(false);
      expect(updatedMapping.invertRoll).toBe(true);
    });

    it('should preserve joystick mapping when changing invert', () => {
      useGimbalStore.getState().setControlMapping({ invertPitch: false });

      const mapping = useGimbalStore.getState().controlMapping;
      expect(mapping.joystickLeft.x).toBe('yaw');
      expect(mapping.joystickLeft.y).toBe('pitch');
    });
  });
});
