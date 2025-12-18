import { useEffect, useCallback, useRef } from 'react';
import { useGimbalStore } from '../store/gimbalStore';
import { useShortcutsStore, type ActionId } from '../store/shortcutsStore';
import { gimbalSocket } from '../services/websocket';

// Speed presets for quick access
const SPEED_PRESETS = [0.25, 0.5, 1.0, 2.0]; // Slow, Normal, Fast, Max

// Track which movement keys are currently pressed
interface ActiveKeys {
  pitch: number; // -1, 0, or 1
  yaw: number;   // -1, 0, or 1
  roll: number;  // -1, 0, or 1
}

// Movement actions that need key tracking
const MOVEMENT_ACTIONS: Record<string, { axis: 'pitch' | 'yaw' | 'roll'; direction: 1 | -1 }> = {
  pitchUp: { axis: 'pitch', direction: 1 },
  pitchDown: { axis: 'pitch', direction: -1 },
  yawLeft: { axis: 'yaw', direction: -1 },
  yawRight: { axis: 'yaw', direction: 1 },
  rollLeft: { axis: 'roll', direction: -1 },
  rollRight: { axis: 'roll', direction: 1 },
};

export function useKeyboardControls() {
  const { sensitivity, connected, setSensitivity, controlMapping } = useGimbalStore();
  const { getActionForKey } = useShortcutsStore();

  // Track active movement keys
  const activeKeys = useRef<ActiveKeys>({ pitch: 0, yaw: 0, roll: 0 });
  // Track which actions are currently active (for key up detection)
  const activeActions = useRef<Set<ActionId>>(new Set());

  // Apply current speed based on active keys
  const applyCurrentSpeed = useCallback((newSensitivity?: number) => {
    const speed = newSensitivity ?? sensitivity;
    const keys = activeKeys.current;

    // Apply invert settings
    const pitchMultiplier = controlMapping.invertPitch ? -1 : 1;
    const yawMultiplier = controlMapping.invertYaw ? -1 : 1;
    const rollMultiplier = controlMapping.invertRoll ? -1 : 1;

    // Only send if there's active movement
    if (keys.pitch !== 0 || keys.yaw !== 0 || keys.roll !== 0) {
      gimbalSocket.setSpeed({
        pitch: keys.pitch * speed * pitchMultiplier,
        yaw: keys.yaw * speed * yawMultiplier,
        roll: keys.roll * speed * rollMultiplier,
      });
    } else {
      gimbalSocket.setSpeed({ pitch: 0, yaw: 0, roll: 0 });
    }
  }, [sensitivity, controlMapping.invertPitch, controlMapping.invertYaw, controlMapping.invertRoll]);

  // Change speed and immediately apply to current movement
  const changeSpeed = useCallback((newSpeed: number) => {
    setSensitivity(newSpeed);
    gimbalSocket.setSpeedMultiplier(newSpeed);
    // Immediately apply new speed to current movement
    applyCurrentSpeed(newSpeed);
  }, [setSensitivity, applyCurrentSpeed]);

  // Execute an action
  const executeAction = useCallback((actionId: ActionId, isKeyDown: boolean) => {
    // Handle movement actions
    const movementAction = MOVEMENT_ACTIONS[actionId];
    if (movementAction) {
      if (isKeyDown) {
        if (activeKeys.current[movementAction.axis] !== movementAction.direction) {
          activeKeys.current[movementAction.axis] = movementAction.direction;
          activeActions.current.add(actionId);
          applyCurrentSpeed();
        }
      }
      return;
    }

    // Non-movement actions only trigger on key down
    if (!isKeyDown) return;

    switch (actionId) {
      case 'goHome':
        gimbalSocket.goHome();
        break;
      case 'setHome':
        gimbalSocket.setHome();
        break;
      case 'toggleTracking':
        const currentTracking = useGimbalStore.getState().tracking;
        gimbalSocket.toggleTracking(!currentTracking);
        break;
      case 'toggleSpeedBoost':
        const currentBoost = useGimbalStore.getState().speedBoost;
        gimbalSocket.toggleSpeedBoost(!currentBoost);
        break;
      case 'emergencyStop':
        activeKeys.current = { pitch: 0, yaw: 0, roll: 0 };
        activeActions.current.clear();
        gimbalSocket.stopSpeed();
        break;
      case 'autoFocus':
        gimbalSocket.calibrateFocus();
        break;
      case 'zoomIn':
        const currentZoom = useGimbalStore.getState().zoom;
        gimbalSocket.setZoom(Math.min(100, currentZoom + 5));
        break;
      case 'zoomOut':
        const currentZoom2 = useGimbalStore.getState().zoom;
        gimbalSocket.setZoom(Math.max(0, currentZoom2 - 5));
        break;
      case 'focusNear':
        const currentFocus = useGimbalStore.getState().focus;
        gimbalSocket.setFocus(Math.max(0, currentFocus - 5));
        break;
      case 'focusFar':
        const currentFocus2 = useGimbalStore.getState().focus;
        gimbalSocket.setFocus(Math.min(100, currentFocus2 + 5));
        break;
      case 'speedSlow':
        changeSpeed(SPEED_PRESETS[0]);
        break;
      case 'speedNormal':
        changeSpeed(SPEED_PRESETS[1]);
        break;
      case 'speedFast':
        changeSpeed(SPEED_PRESETS[2]);
        break;
      case 'speedMax':
        changeSpeed(SPEED_PRESETS[3]);
        break;
      case 'speedIncrease':
        changeSpeed(Math.min(2.0, sensitivity + 0.1));
        break;
      case 'speedDecrease':
        changeSpeed(Math.max(0.1, sensitivity - 0.1));
        break;
      case 'nextGimbal':
        // TODO: Implement gimbal switching
        break;
      case 'prevGimbal':
        // TODO: Implement gimbal switching
        break;
    }
  }, [applyCurrentSpeed, changeSpeed, sensitivity]);

  const handleKeyDown = useCallback(
    (event: KeyboardEvent) => {
      if (!connected) return;

      // Ignore if user is typing in an input
      if (
        event.target instanceof HTMLInputElement ||
        event.target instanceof HTMLTextAreaElement
      ) {
        return;
      }

      const modifiers = {
        shift: event.shiftKey,
        ctrl: event.ctrlKey,
        alt: event.altKey,
        meta: event.metaKey,
      };

      const actionId = getActionForKey(event.key, modifiers);
      if (actionId) {
        // Prevent default for space (scrolling) and tab (focus)
        if (event.key === ' ' || event.key === 'Tab') {
          event.preventDefault();
        }
        executeAction(actionId, true);
      }
    },
    [connected, getActionForKey, executeAction]
  );

  const handleKeyUp = useCallback(
    (event: KeyboardEvent) => {
      if (!connected) return;

      // Ignore if user is typing in an input
      if (
        event.target instanceof HTMLInputElement ||
        event.target instanceof HTMLTextAreaElement
      ) {
        return;
      }

      const modifiers = {
        shift: event.shiftKey,
        ctrl: event.ctrlKey,
        alt: event.altKey,
        meta: event.metaKey,
      };

      // For key up, we need to check all possible modifier combinations
      // because shift might have been released before the key
      const actionIdWithMods = getActionForKey(event.key, modifiers);
      const actionIdNoShift = getActionForKey(event.key, { ...modifiers, shift: false });
      const actionIdWithShift = getActionForKey(event.key, { ...modifiers, shift: true });

      // Check which action was active and release it
      for (const actionId of [actionIdWithMods, actionIdNoShift, actionIdWithShift]) {
        if (actionId && activeActions.current.has(actionId)) {
          const movementAction = MOVEMENT_ACTIONS[actionId];
          if (movementAction) {
            // Only reset if this action was the one that set the direction
            if (activeKeys.current[movementAction.axis] === movementAction.direction) {
              activeKeys.current[movementAction.axis] = 0;
            }
            activeActions.current.delete(actionId);
            applyCurrentSpeed();
          }
        }
      }
    },
    [connected, getActionForKey, applyCurrentSpeed]
  );

  useEffect(() => {
    window.addEventListener('keydown', handleKeyDown);
    window.addEventListener('keyup', handleKeyUp);

    return () => {
      window.removeEventListener('keydown', handleKeyDown);
      window.removeEventListener('keyup', handleKeyUp);
    };
  }, [handleKeyDown, handleKeyUp]);
}
