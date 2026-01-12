import { describe, it, expect, beforeEach } from 'vitest';
import {
  useShortcutsStore,
  ACTION_DEFINITIONS,
  formatBinding,
  getActionsByCategory,
} from '../shortcutsStore';

describe('shortcutsStore', () => {
  beforeEach(() => {
    // Reset store to defaults
    useShortcutsStore.getState().resetAllBindings();
  });

  describe('Initial state', () => {
    it('should have default bindings for all actions', () => {
      const bindings = useShortcutsStore.getState().bindings;

      // Check movement keys
      expect(bindings.pitchUp.key).toBe('w');
      expect(bindings.pitchDown.key).toBe('s');
      expect(bindings.yawLeft.key).toBe('a');
      expect(bindings.yawRight.key).toBe('d');
      expect(bindings.rollLeft.key).toBe('q');
      expect(bindings.rollRight.key).toBe('e');
    });

    it('should have default bindings for speed controls', () => {
      const bindings = useShortcutsStore.getState().bindings;
      expect(bindings.speedSlow.key).toBe('1');
      expect(bindings.speedNormal.key).toBe('2');
      expect(bindings.speedFast.key).toBe('3');
      expect(bindings.speedMax.key).toBe('4');
    });

    it('should have default bindings for quick actions', () => {
      const bindings = useShortcutsStore.getState().bindings;
      expect(bindings.goHome.key).toBe('h');
      expect(bindings.setHome.key).toBe('h');
      expect(bindings.setHome.modifiers?.shift).toBe(true);
      expect(bindings.emergencyStop.key).toBe(' ');
    });
  });

  describe('setBinding', () => {
    it('should update a binding', () => {
      useShortcutsStore.getState().setBinding('pitchUp', { key: 'ArrowUp' });
      expect(useShortcutsStore.getState().bindings.pitchUp.key).toBe('ArrowUp');
    });

    it('should update binding with modifiers', () => {
      useShortcutsStore.getState().setBinding('goHome', {
        key: 'g',
        modifiers: { ctrl: true },
      });
      const binding = useShortcutsStore.getState().bindings.goHome;
      expect(binding.key).toBe('g');
      expect(binding.modifiers?.ctrl).toBe(true);
    });

    it('should preserve other bindings when updating one', () => {
      useShortcutsStore.getState().setBinding('pitchUp', { key: 'ArrowUp' });
      expect(useShortcutsStore.getState().bindings.pitchDown.key).toBe('s');
    });
  });

  describe('resetBinding', () => {
    it('should reset a single binding to default', () => {
      useShortcutsStore.getState().setBinding('pitchUp', { key: 'ArrowUp' });
      useShortcutsStore.getState().resetBinding('pitchUp');
      expect(useShortcutsStore.getState().bindings.pitchUp.key).toBe('w');
    });

    it('should reset binding with modifiers to default', () => {
      useShortcutsStore.getState().setBinding('setHome', { key: 'x' });
      useShortcutsStore.getState().resetBinding('setHome');
      const binding = useShortcutsStore.getState().bindings.setHome;
      expect(binding.key).toBe('h');
      expect(binding.modifiers?.shift).toBe(true);
    });
  });

  describe('resetAllBindings', () => {
    it('should reset all bindings to defaults', () => {
      useShortcutsStore.getState().setBinding('pitchUp', { key: 'ArrowUp' });
      useShortcutsStore.getState().setBinding('pitchDown', { key: 'ArrowDown' });
      useShortcutsStore.getState().setBinding('yawLeft', { key: 'ArrowLeft' });

      useShortcutsStore.getState().resetAllBindings();

      const bindings = useShortcutsStore.getState().bindings;
      expect(bindings.pitchUp.key).toBe('w');
      expect(bindings.pitchDown.key).toBe('s');
      expect(bindings.yawLeft.key).toBe('a');
    });
  });

  describe('getBindingForAction', () => {
    it('should return binding for action', () => {
      const binding = useShortcutsStore.getState().getBindingForAction('pitchUp');
      expect(binding.key).toBe('w');
    });

    it('should return custom binding after update', () => {
      useShortcutsStore.getState().setBinding('pitchUp', { key: 'i' });
      const binding = useShortcutsStore.getState().getBindingForAction('pitchUp');
      expect(binding.key).toBe('i');
    });
  });

  describe('getActionForKey', () => {
    it('should find action for key without modifiers', () => {
      const action = useShortcutsStore.getState().getActionForKey('w', {
        shift: false,
        ctrl: false,
        alt: false,
        meta: false,
      });
      expect(action).toBe('pitchUp');
    });

    it('should find action for key with shift modifier', () => {
      const action = useShortcutsStore.getState().getActionForKey('h', {
        shift: true,
        ctrl: false,
        alt: false,
        meta: false,
      });
      expect(action).toBe('setHome');
    });

    it('should not find action when modifiers dont match', () => {
      const action = useShortcutsStore.getState().getActionForKey('h', {
        shift: true,
        ctrl: true, // Extra modifier
        alt: false,
        meta: false,
      });
      expect(action).toBeNull();
    });

    it('should be case insensitive', () => {
      const actionLower = useShortcutsStore.getState().getActionForKey('w', {
        shift: false,
        ctrl: false,
        alt: false,
        meta: false,
      });
      const actionUpper = useShortcutsStore.getState().getActionForKey('W', {
        shift: false,
        ctrl: false,
        alt: false,
        meta: false,
      });
      expect(actionLower).toBe(actionUpper);
    });

    it('should return null for unbound key', () => {
      const action = useShortcutsStore.getState().getActionForKey('x', {
        shift: false,
        ctrl: false,
        alt: false,
        meta: false,
      });
      expect(action).toBeNull();
    });

    it('should find custom bindings', () => {
      useShortcutsStore.getState().setBinding('pitchUp', {
        key: 'k',
        modifiers: { alt: true },
      });
      const action = useShortcutsStore.getState().getActionForKey('k', {
        shift: false,
        ctrl: false,
        alt: true,
        meta: false,
      });
      expect(action).toBe('pitchUp');
    });
  });

  describe('formatBinding helper', () => {
    it('should format simple key', () => {
      expect(formatBinding({ key: 'w' })).toBe('W');
    });

    it('should format space key', () => {
      expect(formatBinding({ key: ' ' })).toBe('Space');
    });

    it('should format Tab key', () => {
      expect(formatBinding({ key: 'Tab' })).toBe('Tab');
    });

    it('should format key with shift modifier', () => {
      expect(formatBinding({ key: 'h', modifiers: { shift: true } })).toBe('Shift + H');
    });

    it('should format key with ctrl modifier', () => {
      expect(formatBinding({ key: 'a', modifiers: { ctrl: true } })).toBe('Ctrl + A');
    });

    it('should format key with multiple modifiers', () => {
      expect(
        formatBinding({
          key: 's',
          modifiers: { ctrl: true, shift: true },
        })
      ).toBe('Ctrl + Shift + S');
    });

    it('should format key with all modifiers', () => {
      expect(
        formatBinding({
          key: 'x',
          modifiers: { ctrl: true, alt: true, shift: true, meta: true },
        })
      ).toBe('Ctrl + Alt + Shift + Cmd + X');
    });
  });

  describe('getActionsByCategory helper', () => {
    it('should group actions by category', () => {
      const categories = getActionsByCategory();
      expect(Object.keys(categories)).toContain('Movement');
      expect(Object.keys(categories)).toContain('Speed Control');
      expect(Object.keys(categories)).toContain('Quick Actions');
      expect(Object.keys(categories)).toContain('Focus & Zoom');
    });

    it('should have movement actions in Movement category', () => {
      const categories = getActionsByCategory();
      const movementIds = categories['Movement'].map((a) => a.id);
      expect(movementIds).toContain('pitchUp');
      expect(movementIds).toContain('pitchDown');
      expect(movementIds).toContain('yawLeft');
      expect(movementIds).toContain('yawRight');
    });

    it('should have all defined actions', () => {
      const categories = getActionsByCategory();
      const allActions = Object.values(categories).flat();
      expect(allActions.length).toBe(ACTION_DEFINITIONS.length);
    });
  });

  describe('ACTION_DEFINITIONS', () => {
    it('should have all required properties for each action', () => {
      for (const action of ACTION_DEFINITIONS) {
        expect(action).toHaveProperty('id');
        expect(action).toHaveProperty('label');
        expect(action).toHaveProperty('category');
        expect(action).toHaveProperty('defaultBinding');
        expect(action.defaultBinding).toHaveProperty('key');
      }
    });

    it('should have unique action IDs', () => {
      const ids = ACTION_DEFINITIONS.map((a) => a.id);
      const uniqueIds = [...new Set(ids)];
      expect(ids.length).toBe(uniqueIds.length);
    });

    it('should have non-empty labels', () => {
      for (const action of ACTION_DEFINITIONS) {
        expect(action.label.length).toBeGreaterThan(0);
      }
    });
  });
});
