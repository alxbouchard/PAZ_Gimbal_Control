import { describe, it, expect, beforeEach } from 'vitest';
import { useGamepadStore, BUTTON_NAMES, AXIS_NAMES, ACTION_LABELS, AXIS_ACTION_LABELS } from '../gamepadStore';

describe('gamepadStore', () => {
  beforeEach(() => {
    // Reset store to defaults
    useGamepadStore.getState().resetToDefaults();
    useGamepadStore.getState().setConnectedGamepad(null);
    useGamepadStore.getState().cancelBinding();
  });

  describe('Initial state', () => {
    it('should have default button mappings', () => {
      const mapping = useGamepadStore.getState().mapping;
      expect(mapping.buttons[0]).toBe('toggleTracking'); // A
      expect(mapping.buttons[1]).toBe('goHome'); // B
      expect(mapping.buttons[2]).toBe('setHome'); // X
      expect(mapping.buttons[3]).toBe('toggleSpeedBoost'); // Y
    });

    it('should have default axis mappings', () => {
      const mapping = useGamepadStore.getState().mapping;
      expect(mapping.axes[0].action).toBe('yaw'); // Left Stick X
      expect(mapping.axes[1].action).toBe('pitch'); // Left Stick Y
      expect(mapping.axes[2].action).toBe('roll'); // Right Stick X
      expect(mapping.axes[3].action).toBe('focus'); // Right Stick Y
    });

    it('should have default trigger mappings', () => {
      const mapping = useGamepadStore.getState().mapping;
      expect(mapping.leftTrigger.action).toBe('speedMultiplier');
      expect(mapping.rightTrigger.action).toBe('speedMultiplier');
    });

    it('should have default deadzone', () => {
      expect(useGamepadStore.getState().mapping.deadzone).toBe(0.15);
    });

    it('should have vibration enabled by default', () => {
      expect(useGamepadStore.getState().mapping.vibrationEnabled).toBe(true);
    });

    it('should have no connected gamepad initially', () => {
      expect(useGamepadStore.getState().connectedGamepad).toBeNull();
    });

    it('should not be in binding mode initially', () => {
      expect(useGamepadStore.getState().isBinding).toBeNull();
    });
  });

  describe('Button mapping', () => {
    it('should set button mapping', () => {
      useGamepadStore.getState().setButtonMapping(0, 'emergencyStop');
      expect(useGamepadStore.getState().mapping.buttons[0]).toBe('emergencyStop');
    });

    it('should set button to none', () => {
      useGamepadStore.getState().setButtonMapping(0, 'none');
      expect(useGamepadStore.getState().mapping.buttons[0]).toBe('none');
    });

    it('should set ATEM button actions', () => {
      useGamepadStore.getState().setButtonMapping(10, 'atemAutoFocus');
      useGamepadStore.getState().setButtonMapping(11, 'atemAutoAperture');
      expect(useGamepadStore.getState().mapping.buttons[10]).toBe('atemAutoFocus');
      expect(useGamepadStore.getState().mapping.buttons[11]).toBe('atemAutoAperture');
    });
  });

  describe('Axis mapping', () => {
    it('should set axis action', () => {
      useGamepadStore.getState().setAxisMapping(0, { action: 'pitch' });
      expect(useGamepadStore.getState().mapping.axes[0].action).toBe('pitch');
    });

    it('should set axis inverted', () => {
      useGamepadStore.getState().setAxisMapping(0, { inverted: true });
      expect(useGamepadStore.getState().mapping.axes[0].inverted).toBe(true);
    });

    it('should set axis sensitivity', () => {
      useGamepadStore.getState().setAxisMapping(0, { sensitivity: 1.5 });
      expect(useGamepadStore.getState().mapping.axes[0].sensitivity).toBe(1.5);
    });

    it('should set multiple axis properties at once', () => {
      useGamepadStore.getState().setAxisMapping(1, {
        action: 'zoom',
        inverted: true,
        sensitivity: 0.5,
      });
      const axis = useGamepadStore.getState().mapping.axes[1];
      expect(axis.action).toBe('zoom');
      expect(axis.inverted).toBe(true);
      expect(axis.sensitivity).toBe(0.5);
    });

    it('should set ATEM axis actions', () => {
      useGamepadStore.getState().setAxisMapping(2, { action: 'atemFocus' });
      useGamepadStore.getState().setAxisMapping(3, { action: 'atemZoom' });
      expect(useGamepadStore.getState().mapping.axes[2].action).toBe('atemFocus');
      expect(useGamepadStore.getState().mapping.axes[3].action).toBe('atemZoom');
    });
  });

  describe('Trigger mapping', () => {
    it('should set left trigger mapping', () => {
      useGamepadStore.getState().setTriggerMapping('left', { action: 'zoom' });
      expect(useGamepadStore.getState().mapping.leftTrigger.action).toBe('zoom');
    });

    it('should set right trigger mapping', () => {
      useGamepadStore.getState().setTriggerMapping('right', { action: 'focus' });
      expect(useGamepadStore.getState().mapping.rightTrigger.action).toBe('focus');
    });

    it('should set trigger inverted', () => {
      useGamepadStore.getState().setTriggerMapping('left', { inverted: false });
      expect(useGamepadStore.getState().mapping.leftTrigger.inverted).toBe(false);
    });

    it('should set trigger sensitivity', () => {
      useGamepadStore.getState().setTriggerMapping('right', { sensitivity: 2.0 });
      expect(useGamepadStore.getState().mapping.rightTrigger.sensitivity).toBe(2.0);
    });
  });

  describe('Deadzone', () => {
    it('should update deadzone', () => {
      useGamepadStore.getState().setDeadzone(0.25);
      expect(useGamepadStore.getState().mapping.deadzone).toBe(0.25);
    });
  });

  describe('Vibration', () => {
    it('should disable vibration', () => {
      useGamepadStore.getState().setVibrationEnabled(false);
      expect(useGamepadStore.getState().mapping.vibrationEnabled).toBe(false);
    });

    it('should enable vibration', () => {
      useGamepadStore.getState().setVibrationEnabled(false);
      useGamepadStore.getState().setVibrationEnabled(true);
      expect(useGamepadStore.getState().mapping.vibrationEnabled).toBe(true);
    });
  });

  describe('Connected gamepad', () => {
    it('should set connected gamepad name', () => {
      useGamepadStore.getState().setConnectedGamepad('Xbox Wireless Controller');
      expect(useGamepadStore.getState().connectedGamepad).toBe('Xbox Wireless Controller');
    });

    it('should clear connected gamepad', () => {
      useGamepadStore.getState().setConnectedGamepad('Controller');
      useGamepadStore.getState().setConnectedGamepad(null);
      expect(useGamepadStore.getState().connectedGamepad).toBeNull();
    });
  });

  describe('Binding mode', () => {
    it('should start button binding', () => {
      useGamepadStore.getState().startBinding('button', 5);
      expect(useGamepadStore.getState().isBinding).toEqual({ type: 'button', index: 5 });
    });

    it('should start axis binding', () => {
      useGamepadStore.getState().startBinding('axis', 2);
      expect(useGamepadStore.getState().isBinding).toEqual({ type: 'axis', index: 2 });
    });

    it('should start trigger binding', () => {
      useGamepadStore.getState().startBinding('trigger', 0);
      expect(useGamepadStore.getState().isBinding).toEqual({ type: 'trigger', index: 0 });
    });

    it('should cancel binding', () => {
      useGamepadStore.getState().startBinding('button', 5);
      useGamepadStore.getState().cancelBinding();
      expect(useGamepadStore.getState().isBinding).toBeNull();
    });

    it('should complete button binding with swap', () => {
      // Set up initial mappings
      useGamepadStore.getState().setButtonMapping(0, 'toggleTracking');
      useGamepadStore.getState().setButtonMapping(1, 'goHome');

      // Start binding button 0
      useGamepadStore.getState().startBinding('button', 0);
      // Complete by pressing button 1 (should swap)
      useGamepadStore.getState().completeBinding(1);

      // Button 0 should now have what button 1 had
      expect(useGamepadStore.getState().mapping.buttons[0]).toBe('goHome');
      expect(useGamepadStore.getState().isBinding).toBeNull();
    });
  });

  describe('Swap buttons', () => {
    it('should swap button mappings', () => {
      useGamepadStore.getState().setButtonMapping(0, 'toggleTracking');
      useGamepadStore.getState().setButtonMapping(1, 'goHome');

      useGamepadStore.getState().swapButtons(0, 1);

      expect(useGamepadStore.getState().mapping.buttons[0]).toBe('goHome');
      expect(useGamepadStore.getState().mapping.buttons[1]).toBe('toggleTracking');
    });
  });

  describe('Reset to defaults', () => {
    it('should reset all mappings to defaults', () => {
      // Change various settings
      useGamepadStore.getState().setButtonMapping(0, 'emergencyStop');
      useGamepadStore.getState().setAxisMapping(0, { action: 'roll' });
      useGamepadStore.getState().setDeadzone(0.5);
      useGamepadStore.getState().setVibrationEnabled(false);

      // Reset
      useGamepadStore.getState().resetToDefaults();

      const mapping = useGamepadStore.getState().mapping;
      expect(mapping.buttons[0]).toBe('toggleTracking');
      expect(mapping.axes[0].action).toBe('yaw');
      expect(mapping.deadzone).toBe(0.15);
      expect(mapping.vibrationEnabled).toBe(true);
    });
  });

  describe('Constants', () => {
    it('should have button names for standard gamepad buttons', () => {
      expect(BUTTON_NAMES[0]).toBe('A');
      expect(BUTTON_NAMES[1]).toBe('B');
      expect(BUTTON_NAMES[2]).toBe('X');
      expect(BUTTON_NAMES[3]).toBe('Y');
      expect(BUTTON_NAMES[4]).toBe('LB');
      expect(BUTTON_NAMES[5]).toBe('RB');
    });

    it('should have axis names', () => {
      expect(AXIS_NAMES[0]).toBe('Left Stick X');
      expect(AXIS_NAMES[1]).toBe('Left Stick Y');
      expect(AXIS_NAMES[2]).toBe('Right Stick X');
      expect(AXIS_NAMES[3]).toBe('Right Stick Y');
    });

    it('should have action labels', () => {
      expect(ACTION_LABELS.toggleTracking).toBe('Toggle Tracking');
      expect(ACTION_LABELS.atemAutoFocus).toBe('ATEM Auto Focus');
    });

    it('should have axis action labels', () => {
      expect(AXIS_ACTION_LABELS.pitch).toBe('Pitch Control');
      expect(AXIS_ACTION_LABELS.atemFocus).toBe('ATEM Focus');
    });
  });
});
