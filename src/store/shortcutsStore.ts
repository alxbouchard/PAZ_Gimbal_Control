import { create } from 'zustand';
import { persist } from 'zustand/middleware';

// Action IDs for all bindable actions
export type ActionId =
  // Movement
  | 'pitchUp'
  | 'pitchDown'
  | 'yawLeft'
  | 'yawRight'
  | 'rollLeft'
  | 'rollRight'
  // Speed presets
  | 'speedSlow'
  | 'speedNormal'
  | 'speedFast'
  | 'speedMax'
  | 'speedIncrease'
  | 'speedDecrease'
  // Quick actions
  | 'goHome'
  | 'setHome'
  | 'toggleTracking'
  | 'toggleSpeedBoost'
  | 'emergencyStop'
  // Focus & Zoom
  | 'autoFocus'
  | 'zoomIn'
  | 'zoomOut'
  | 'focusNear'
  | 'focusFar'
  // Gimbal selection
  | 'nextGimbal'
  | 'prevGimbal';

export interface ShortcutBinding {
  key: string;
  modifiers?: {
    shift?: boolean;
    ctrl?: boolean;
    alt?: boolean;
    meta?: boolean;
  };
}

export interface ActionDefinition {
  id: ActionId;
  label: string;
  category: string;
  defaultBinding: ShortcutBinding;
}

// Default shortcuts configuration
export const ACTION_DEFINITIONS: ActionDefinition[] = [
  // Movement
  { id: 'pitchUp', label: 'Pitch Up', category: 'Movement', defaultBinding: { key: 'w' } },
  { id: 'pitchDown', label: 'Pitch Down', category: 'Movement', defaultBinding: { key: 's' } },
  { id: 'yawLeft', label: 'Yaw Left', category: 'Movement', defaultBinding: { key: 'a' } },
  { id: 'yawRight', label: 'Yaw Right', category: 'Movement', defaultBinding: { key: 'd' } },
  { id: 'rollLeft', label: 'Roll Left', category: 'Movement', defaultBinding: { key: 'q' } },
  { id: 'rollRight', label: 'Roll Right', category: 'Movement', defaultBinding: { key: 'e' } },

  // Speed Control
  { id: 'speedSlow', label: 'Speed: Slow (25%)', category: 'Speed Control', defaultBinding: { key: '1' } },
  { id: 'speedNormal', label: 'Speed: Normal (50%)', category: 'Speed Control', defaultBinding: { key: '2' } },
  { id: 'speedFast', label: 'Speed: Fast (100%)', category: 'Speed Control', defaultBinding: { key: '3' } },
  { id: 'speedMax', label: 'Speed: Max (200%)', category: 'Speed Control', defaultBinding: { key: '4' } },
  { id: 'speedIncrease', label: 'Increase Speed +10%', category: 'Speed Control', defaultBinding: { key: '=' } },
  { id: 'speedDecrease', label: 'Decrease Speed -10%', category: 'Speed Control', defaultBinding: { key: '-' } },

  // Quick Actions
  { id: 'goHome', label: 'Return to Home', category: 'Quick Actions', defaultBinding: { key: 'h' } },
  { id: 'setHome', label: 'Set Current as Home', category: 'Quick Actions', defaultBinding: { key: 'h', modifiers: { shift: true } } },
  { id: 'toggleTracking', label: 'Toggle Tracking', category: 'Quick Actions', defaultBinding: { key: 't' } },
  { id: 'toggleSpeedBoost', label: 'Toggle Speed Boost', category: 'Quick Actions', defaultBinding: { key: 'b' } },
  { id: 'emergencyStop', label: 'Emergency Stop', category: 'Quick Actions', defaultBinding: { key: ' ' } },

  // Focus & Zoom
  { id: 'autoFocus', label: 'Auto Focus', category: 'Focus & Zoom', defaultBinding: { key: 'f' } },
  { id: 'zoomIn', label: 'Zoom In', category: 'Focus & Zoom', defaultBinding: { key: ']', modifiers: { shift: true } } },
  { id: 'zoomOut', label: 'Zoom Out', category: 'Focus & Zoom', defaultBinding: { key: '[', modifiers: { shift: true } } },
  { id: 'focusNear', label: 'Focus Near', category: 'Focus & Zoom', defaultBinding: { key: '[' } },
  { id: 'focusFar', label: 'Focus Far', category: 'Focus & Zoom', defaultBinding: { key: ']' } },

  // Gimbal Selection
  { id: 'nextGimbal', label: 'Next Gimbal', category: 'Gimbal Selection', defaultBinding: { key: 'Tab' } },
  { id: 'prevGimbal', label: 'Previous Gimbal', category: 'Gimbal Selection', defaultBinding: { key: 'Tab', modifiers: { shift: true } } },
];

// Get default bindings as a map
function getDefaultBindings(): Record<ActionId, ShortcutBinding> {
  const bindings: Record<string, ShortcutBinding> = {};
  for (const action of ACTION_DEFINITIONS) {
    bindings[action.id] = { ...action.defaultBinding };
  }
  return bindings as Record<ActionId, ShortcutBinding>;
}

interface ShortcutsState {
  bindings: Record<ActionId, ShortcutBinding>;
  setBinding: (actionId: ActionId, binding: ShortcutBinding) => void;
  resetBinding: (actionId: ActionId) => void;
  resetAllBindings: () => void;
  getBindingForAction: (actionId: ActionId) => ShortcutBinding;
  getActionForKey: (key: string, modifiers: { shift: boolean; ctrl: boolean; alt: boolean; meta: boolean }) => ActionId | null;
}

export const useShortcutsStore = create<ShortcutsState>()(
  persist(
    (set, get) => ({
      bindings: getDefaultBindings(),

      setBinding: (actionId, binding) => {
        set((state) => ({
          bindings: {
            ...state.bindings,
            [actionId]: binding,
          },
        }));
      },

      resetBinding: (actionId) => {
        const defaultBinding = ACTION_DEFINITIONS.find((a) => a.id === actionId)?.defaultBinding;
        if (defaultBinding) {
          set((state) => ({
            bindings: {
              ...state.bindings,
              [actionId]: { ...defaultBinding },
            },
          }));
        }
      },

      resetAllBindings: () => {
        set({ bindings: getDefaultBindings() });
      },

      getBindingForAction: (actionId) => {
        return get().bindings[actionId];
      },

      getActionForKey: (key, modifiers) => {
        const { bindings } = get();
        const normalizedKey = key.toLowerCase();

        for (const [actionId, binding] of Object.entries(bindings)) {
          const bindingKey = binding.key.toLowerCase();
          const bindingMods = binding.modifiers || {};

          // Check if key matches
          if (bindingKey !== normalizedKey) continue;

          // Check modifiers
          const shiftMatch = !!bindingMods.shift === modifiers.shift;
          const ctrlMatch = !!bindingMods.ctrl === modifiers.ctrl;
          const altMatch = !!bindingMods.alt === modifiers.alt;
          const metaMatch = !!bindingMods.meta === modifiers.meta;

          if (shiftMatch && ctrlMatch && altMatch && metaMatch) {
            return actionId as ActionId;
          }
        }

        return null;
      },
    }),
    {
      name: 'gimbal-shortcuts',
    }
  )
);

// Helper to format a binding for display
export function formatBinding(binding: ShortcutBinding): string {
  const parts: string[] = [];

  if (binding.modifiers?.ctrl) parts.push('Ctrl');
  if (binding.modifiers?.alt) parts.push('Alt');
  if (binding.modifiers?.shift) parts.push('Shift');
  if (binding.modifiers?.meta) parts.push('Cmd');

  // Format special keys
  let keyDisplay = binding.key;
  if (binding.key === ' ') keyDisplay = 'Space';
  else if (binding.key === 'Tab') keyDisplay = 'Tab';
  else if (binding.key.length === 1) keyDisplay = binding.key.toUpperCase();

  parts.push(keyDisplay);

  return parts.join(' + ');
}

// Get categories with their actions
export function getActionsByCategory(): Record<string, ActionDefinition[]> {
  const categories: Record<string, ActionDefinition[]> = {};

  for (const action of ACTION_DEFINITIONS) {
    if (!categories[action.category]) {
      categories[action.category] = [];
    }
    categories[action.category].push(action);
  }

  return categories;
}
