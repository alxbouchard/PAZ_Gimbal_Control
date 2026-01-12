import { create } from 'zustand';
import { persist } from 'zustand/middleware';

// All available gamepad actions
export type GamepadAction =
  | 'none'
  | 'toggleTracking'
  | 'goHome'
  | 'setHome'
  | 'toggleSpeedBoost'
  | 'emergencyStop'
  | 'prevGimbal'
  | 'nextGimbal'
  | 'zoomIn'
  | 'zoomOut'
  | 'focusNear'
  | 'focusFar'
  // ATEM Camera controls
  | 'atemAutoFocus'
  | 'atemAutoAperture';

// Axis actions (continuous)
export type AxisAction =
  | 'none'
  | 'pitch'
  | 'yaw'
  | 'roll'
  | 'focus'
  | 'zoom'
  | 'speedMultiplier'
  // ATEM Camera axes
  | 'atemFocus'
  | 'atemZoom';

// Button names for display
export const BUTTON_NAMES: Record<number, string> = {
  0: 'A',
  1: 'B',
  2: 'X',
  3: 'Y',
  4: 'LB',
  5: 'RB',
  6: 'LT',
  7: 'RT',
  8: 'Back',
  9: 'Start',
  10: 'Left Stick',
  11: 'Right Stick',
  12: 'D-pad Up',
  13: 'D-pad Down',
  14: 'D-pad Left',
  15: 'D-pad Right',
  16: 'Xbox/Guide',
};

export const AXIS_NAMES: Record<number, string> = {
  0: 'Left Stick X',
  1: 'Left Stick Y',
  2: 'Right Stick X',
  3: 'Right Stick Y',
};

export const ACTION_LABELS: Record<GamepadAction, string> = {
  none: 'None',
  toggleTracking: 'Toggle Tracking',
  goHome: 'Go Home',
  setHome: 'Set Home Position',
  toggleSpeedBoost: 'Toggle Speed Boost',
  emergencyStop: 'Emergency Stop',
  prevGimbal: 'Previous Gimbal',
  nextGimbal: 'Next Gimbal',
  zoomIn: 'Zoom In',
  zoomOut: 'Zoom Out',
  focusNear: 'Focus Near',
  focusFar: 'Focus Far',
  atemAutoFocus: 'ATEM Auto Focus',
  atemAutoAperture: 'ATEM Auto Aperture',
};

export const AXIS_ACTION_LABELS: Record<AxisAction, string> = {
  none: 'None',
  pitch: 'Pitch Control',
  yaw: 'Yaw Control',
  roll: 'Roll Control',
  focus: 'Focus Control',
  zoom: 'Zoom Control',
  speedMultiplier: 'Speed Multiplier',
  atemFocus: 'ATEM Focus',
  atemZoom: 'ATEM Zoom',
};

export interface AxisMapping {
  action: AxisAction;
  inverted: boolean;
  sensitivity: number; // 0.1 to 2.0
}

export interface GamepadMapping {
  // Button mappings (button index -> action)
  buttons: Record<number, GamepadAction>;
  // Axis mappings (axis index -> action config)
  axes: Record<number, AxisMapping>;
  // Trigger mappings (treated specially as they're 0-1 range)
  leftTrigger: AxisMapping;
  rightTrigger: AxisMapping;
  // Global settings
  deadzone: number;
  vibrationEnabled: boolean;
}

const defaultMapping: GamepadMapping = {
  buttons: {
    0: 'toggleTracking',    // A
    1: 'goHome',            // B
    2: 'setHome',           // X
    3: 'toggleSpeedBoost',  // Y
    4: 'prevGimbal',        // LB
    5: 'nextGimbal',        // RB
    8: 'none',              // Back
    9: 'emergencyStop',     // Start
    10: 'none',             // Left Stick Click
    11: 'none',             // Right Stick Click
    12: 'zoomIn',           // D-pad Up
    13: 'zoomOut',          // D-pad Down
    14: 'none',             // D-pad Left
    15: 'none',             // D-pad Right
  },
  axes: {
    0: { action: 'yaw', inverted: false, sensitivity: 1.0 },      // Left Stick X
    1: { action: 'pitch', inverted: true, sensitivity: 1.0 },     // Left Stick Y
    2: { action: 'roll', inverted: false, sensitivity: 1.0 },     // Right Stick X
    3: { action: 'focus', inverted: true, sensitivity: 1.0 },     // Right Stick Y
  },
  leftTrigger: { action: 'speedMultiplier', inverted: true, sensitivity: 0.75 },
  rightTrigger: { action: 'speedMultiplier', inverted: false, sensitivity: 1.0 },
  deadzone: 0.15,
  vibrationEnabled: true,
};

interface GamepadStore {
  // State
  mapping: GamepadMapping;
  connectedGamepad: string | null;
  isBinding: { type: 'button' | 'axis' | 'trigger'; index: number } | null;

  // Actions
  setButtonMapping: (buttonIndex: number, action: GamepadAction) => void;
  setAxisMapping: (axisIndex: number, mapping: Partial<AxisMapping>) => void;
  setTriggerMapping: (trigger: 'left' | 'right', mapping: Partial<AxisMapping>) => void;
  setDeadzone: (value: number) => void;
  setVibrationEnabled: (enabled: boolean) => void;
  setConnectedGamepad: (name: string | null) => void;
  startBinding: (type: 'button' | 'axis' | 'trigger', index: number) => void;
  cancelBinding: () => void;
  completeBinding: (inputIndex: number) => void;
  resetToDefaults: () => void;
  swapButtons: (button1: number, button2: number) => void;
}

export const useGamepadStore = create<GamepadStore>()(
  persist(
    (set, get) => ({
      mapping: defaultMapping,
      connectedGamepad: null,
      isBinding: null,

      setButtonMapping: (buttonIndex, action) =>
        set((state) => ({
          mapping: {
            ...state.mapping,
            buttons: { ...state.mapping.buttons, [buttonIndex]: action },
          },
        })),

      setAxisMapping: (axisIndex, newMapping) =>
        set((state) => ({
          mapping: {
            ...state.mapping,
            axes: {
              ...state.mapping.axes,
              [axisIndex]: { ...state.mapping.axes[axisIndex], ...newMapping },
            },
          },
        })),

      setTriggerMapping: (trigger, newMapping) =>
        set((state) => ({
          mapping: {
            ...state.mapping,
            [trigger === 'left' ? 'leftTrigger' : 'rightTrigger']: {
              ...state.mapping[trigger === 'left' ? 'leftTrigger' : 'rightTrigger'],
              ...newMapping,
            },
          },
        })),

      setDeadzone: (value) =>
        set((state) => ({
          mapping: { ...state.mapping, deadzone: value },
        })),

      setVibrationEnabled: (enabled) =>
        set((state) => ({
          mapping: { ...state.mapping, vibrationEnabled: enabled },
        })),

      setConnectedGamepad: (name) => set({ connectedGamepad: name }),

      startBinding: (type, index) => set({ isBinding: { type, index } }),

      cancelBinding: () => set({ isBinding: null }),

      completeBinding: (inputIndex) => {
        const { isBinding, mapping } = get();
        if (!isBinding) return;

        if (isBinding.type === 'button') {
          // Swap the actions if the new button already has an action
          const currentAction = mapping.buttons[isBinding.index] || 'none';
          const targetAction = mapping.buttons[inputIndex] || 'none';

          set((state) => ({
            isBinding: null,
            mapping: {
              ...state.mapping,
              buttons: {
                ...state.mapping.buttons,
                [isBinding.index]: targetAction,
                [inputIndex]: currentAction,
              },
            },
          }));
        }
        // For axes, just complete without swapping
        set({ isBinding: null });
      },

      resetToDefaults: () => set({ mapping: defaultMapping }),

      swapButtons: (button1, button2) =>
        set((state) => {
          const action1 = state.mapping.buttons[button1] || 'none';
          const action2 = state.mapping.buttons[button2] || 'none';
          return {
            mapping: {
              ...state.mapping,
              buttons: {
                ...state.mapping.buttons,
                [button1]: action2,
                [button2]: action1,
              },
            },
          };
        }),
    }),
    {
      name: 'gamepad-mapping',
    }
  )
);
