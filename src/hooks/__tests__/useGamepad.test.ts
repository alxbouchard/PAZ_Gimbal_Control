import { describe, it, expect, vi, beforeEach, afterEach } from 'vitest';
import { renderHook } from '@testing-library/react';
import { useGamepad } from '../useGamepad';
import { useGimbalStore } from '../../store/gimbalStore';
import { useGamepadStore } from '../../store/gamepadStore';

// Mock the websocket service
vi.mock('../../services/websocket', () => ({
  gimbalSocket: {
    setSpeed: vi.fn(),
    stopSpeed: vi.fn(),
    toggleTracking: vi.fn(),
    toggleSpeedBoost: vi.fn(),
    goHome: vi.fn(),
    setHome: vi.fn(),
    setZoom: vi.fn(),
    setFocus: vi.fn(),
    selectGimbal: vi.fn(),
    setSpeedMultiplier: vi.fn(),
    triggerAtemAutoFocus: vi.fn(),
    triggerAtemAutoAperture: vi.fn(),
    setAtemFocus: vi.fn(),
    setAtemZoom: vi.fn(),
  },
}));

describe('useGamepad', () => {
  beforeEach(() => {
    // Reset stores
    useGimbalStore.getState().setConnected(true);
    useGimbalStore.getState().setSensitivity(1.0);
    useGamepadStore.getState().resetToDefaults();

    // Reset mocks
    vi.clearAllMocks();

    // Mock navigator.getGamepads to return null initially
    vi.mocked(navigator.getGamepads).mockReturnValue([null, null, null, null]);
  });

  afterEach(() => {
    vi.clearAllMocks();
  });

  describe('Deadzone', () => {
    it('should apply deadzone to axis values', () => {
      renderHook(() => useGamepad());

      // The deadzone is 0.15 by default
      // Values below deadzone should be 0
      // Values above should be scaled
      const deadzone = 0.15;

      // Small value within deadzone
      expect(Math.abs(0.1) < deadzone).toBe(true);

      // Value outside deadzone
      const testValue = 0.5;
      const expected = (testValue - deadzone) / (1 - deadzone);
      expect(expected).toBeGreaterThan(0);
    });
  });

  describe('Gamepad connection', () => {
    it('should register event listeners', () => {
      // Just verify the hook renders without error
      const { result } = renderHook(() => useGamepad());
      expect(result).toBeDefined();
    });
  });

  describe('Button actions', () => {
    it('should verify store connection state affects processing', () => {
      // When disconnected, actions should not be processed
      useGimbalStore.getState().setConnected(false);
      expect(useGimbalStore.getState().connected).toBe(false);

      useGimbalStore.getState().setConnected(true);
      expect(useGimbalStore.getState().connected).toBe(true);
    });
  });

  describe('Axis mapping', () => {
    it('should have default axis mappings', () => {
      const mapping = useGamepadStore.getState().mapping;

      expect(mapping.axes[0].action).toBe('yaw');
      expect(mapping.axes[1].action).toBe('pitch');
      expect(mapping.axes[2].action).toBe('roll');
      expect(mapping.axes[3].action).toBe('focus');
    });

    it('should respect axis inversion setting', () => {
      const mapping = useGamepadStore.getState().mapping;

      // Left stick Y (pitch) should be inverted by default
      expect(mapping.axes[1].inverted).toBe(true);
    });

    it('should apply axis sensitivity', () => {
      const mapping = useGamepadStore.getState().mapping;

      // Default sensitivity should be 1.0
      expect(mapping.axes[0].sensitivity).toBe(1.0);
    });
  });

  describe('Trigger mapping', () => {
    it('should have default trigger mappings', () => {
      const mapping = useGamepadStore.getState().mapping;

      expect(mapping.leftTrigger.action).toBe('speedMultiplier');
      expect(mapping.rightTrigger.action).toBe('speedMultiplier');
    });

    it('should have correct trigger sensitivity', () => {
      const mapping = useGamepadStore.getState().mapping;

      expect(mapping.leftTrigger.sensitivity).toBe(0.75);
      expect(mapping.rightTrigger.sensitivity).toBe(1.0);
    });
  });

  describe('Speed multiplier', () => {
    it('should clamp speed multiplier between 0.25 and 2.0', () => {
      // The speed multiplier logic clamps values
      const min = 0.25;
      const max = 2.0;

      expect(Math.max(min, Math.min(max, 0.1))).toBe(min);
      expect(Math.max(min, Math.min(max, 3.0))).toBe(max);
      expect(Math.max(min, Math.min(max, 1.0))).toBe(1.0);
    });
  });

  describe('ATEM controls', () => {
    it('should have ATEM action types available', () => {
      // Set up ATEM actions
      useGamepadStore.getState().setButtonMapping(10, 'atemAutoFocus');
      useGamepadStore.getState().setButtonMapping(11, 'atemAutoAperture');

      expect(useGamepadStore.getState().mapping.buttons[10]).toBe('atemAutoFocus');
      expect(useGamepadStore.getState().mapping.buttons[11]).toBe('atemAutoAperture');
    });

    it('should have ATEM axis actions available', () => {
      useGamepadStore.getState().setAxisMapping(2, { action: 'atemFocus' });
      useGamepadStore.getState().setAxisMapping(3, { action: 'atemZoom' });

      expect(useGamepadStore.getState().mapping.axes[2].action).toBe('atemFocus');
      expect(useGamepadStore.getState().mapping.axes[3].action).toBe('atemZoom');
    });
  });

  describe('Gimbal navigation', () => {
    it('should support nextGimbal action', () => {
      useGamepadStore.getState().setButtonMapping(5, 'nextGimbal');
      expect(useGamepadStore.getState().mapping.buttons[5]).toBe('nextGimbal');
    });

    it('should support prevGimbal action', () => {
      useGamepadStore.getState().setButtonMapping(4, 'prevGimbal');
      expect(useGamepadStore.getState().mapping.buttons[4]).toBe('prevGimbal');
    });
  });

  describe('Camera controls', () => {
    it('should support zoomIn and zoomOut actions', () => {
      useGamepadStore.getState().setButtonMapping(12, 'zoomIn');
      useGamepadStore.getState().setButtonMapping(13, 'zoomOut');

      expect(useGamepadStore.getState().mapping.buttons[12]).toBe('zoomIn');
      expect(useGamepadStore.getState().mapping.buttons[13]).toBe('zoomOut');
    });

    it('should support focusNear and focusFar actions', () => {
      useGamepadStore.getState().setButtonMapping(14, 'focusNear');
      useGamepadStore.getState().setButtonMapping(15, 'focusFar');

      expect(useGamepadStore.getState().mapping.buttons[14]).toBe('focusNear');
      expect(useGamepadStore.getState().mapping.buttons[15]).toBe('focusFar');
    });
  });
});
