import { useEffect, useRef, useCallback } from 'react';
import { useGimbalStore } from '../store/gimbalStore';
import { useAtemStore } from '../store/atemStore';
import { useGamepadStore, type GamepadAction, type AxisAction } from '../store/gamepadStore';
import { gimbalSocket } from '../services/websocket';
import { cameraTypeToString } from '../types';

interface GamepadState {
  leftStick: { x: number; y: number };
  rightStick: { x: number; y: number };
  buttons: boolean[];
  triggers: { left: number; right: number };
  axes: number[];
}

export function useGamepad() {
  const { sensitivity, connected } = useGimbalStore();
  const { mapping, setConnectedGamepad } = useGamepadStore();
  const animationFrameRef = useRef<number>();
  const previousButtonsRef = useRef<boolean[]>([]);

  const applyDeadzone = useCallback((value: number): number => {
    const deadzone = mapping.deadzone;
    if (Math.abs(value) < deadzone) return 0;
    const sign = value > 0 ? 1 : -1;
    return sign * ((Math.abs(value) - deadzone) / (1 - deadzone));
  }, [mapping.deadzone]);

  const getGamepadState = useCallback((gamepad: Gamepad): GamepadState => {
    return {
      leftStick: {
        x: applyDeadzone(gamepad.axes[0] || 0),
        y: applyDeadzone(gamepad.axes[1] || 0),
      },
      rightStick: {
        x: applyDeadzone(gamepad.axes[2] || 0),
        y: applyDeadzone(gamepad.axes[3] || 0),
      },
      buttons: gamepad.buttons.map((b) => b.pressed),
      triggers: {
        left: gamepad.buttons[6]?.value || 0,
        right: gamepad.buttons[7]?.value || 0,
      },
      axes: gamepad.axes.map((a) => applyDeadzone(a)),
    };
  }, [applyDeadzone]);

  // Execute a button action
  const executeButtonAction = useCallback((action: GamepadAction) => {
    const store = useGimbalStore.getState();

    switch (action) {
      case 'toggleTracking':
        gimbalSocket.toggleTracking(!store.tracking);
        break;
      case 'goHome':
        gimbalSocket.goHome();
        break;
      case 'setHome':
        gimbalSocket.setHome();
        break;
      case 'toggleSpeedBoost':
        gimbalSocket.toggleSpeedBoost(!store.speedBoost);
        break;
      case 'emergencyStop':
        gimbalSocket.stopSpeed();
        break;
      case 'prevGimbal': {
        const gimbals = store.availableGimbals;
        const currentId = store.activeGimbalId;
        const currentIndex = gimbals.findIndex((g) => g.id === currentId);
        const prevIndex = (currentIndex - 1 + gimbals.length) % gimbals.length;
        if (gimbals[prevIndex]) {
          gimbalSocket.selectGimbal(gimbals[prevIndex].id);
        }
        break;
      }
      case 'nextGimbal': {
        const gimbals = store.availableGimbals;
        const currentId = store.activeGimbalId;
        const currentIndex = gimbals.findIndex((g) => g.id === currentId);
        const nextIndex = (currentIndex + 1) % gimbals.length;
        if (gimbals[nextIndex]) {
          gimbalSocket.selectGimbal(gimbals[nextIndex].id);
        }
        break;
      }
      case 'zoomIn':
        gimbalSocket.setZoom(Math.min(100, store.zoom + 10));
        break;
      case 'zoomOut':
        gimbalSocket.setZoom(Math.max(0, store.zoom - 10));
        break;
      case 'focusNear':
        gimbalSocket.setFocus(Math.min(100, store.focus + 5));
        break;
      case 'focusFar':
        gimbalSocket.setFocus(Math.max(0, store.focus - 5));
        break;
      // ATEM Camera controls
      case 'atemAutoFocus': {
        const atemStore = useAtemStore.getState();
        const mapping = store.activeGimbalId ? atemStore.mappings[store.activeGimbalId] : null;
        if (mapping && atemStore.config.connected) {
          gimbalSocket.triggerAtemAutoFocus(mapping.port);
        }
        break;
      }
      case 'atemAutoAperture': {
        const atemStore = useAtemStore.getState();
        const mapping = store.activeGimbalId ? atemStore.mappings[store.activeGimbalId] : null;
        if (mapping && atemStore.config.connected) {
          gimbalSocket.triggerAtemAutoAperture(mapping.port);
        }
        break;
      }
      case 'none':
      default:
        break;
    }
  }, []);

  // Apply axis action
  const applyAxisAction = useCallback((action: AxisAction, value: number, axisSensitivity: number, inverted: boolean) => {
    const store = useGimbalStore.getState();
    const adjustedValue = (inverted ? -value : value) * axisSensitivity * sensitivity;

    switch (action) {
      case 'pitch':
        gimbalSocket.setSpeed({ pitch: adjustedValue });
        break;
      case 'yaw':
        gimbalSocket.setSpeed({ yaw: adjustedValue });
        break;
      case 'roll':
        gimbalSocket.setSpeed({ roll: adjustedValue });
        break;
      case 'focus':
        if (Math.abs(value) > 0.1) {
          gimbalSocket.setFocus(Math.max(0, Math.min(100, store.focus - adjustedValue * 2)));
        }
        break;
      case 'zoom':
        if (Math.abs(value) > 0.1) {
          gimbalSocket.setZoom(Math.max(0, Math.min(100, store.zoom - adjustedValue * 2)));
        }
        break;
      // ATEM Camera axes
      case 'atemFocus': {
        if (Math.abs(value) > 0.1) {
          const atemStore = useAtemStore.getState();
          const mapping = store.activeGimbalId ? atemStore.mappings[store.activeGimbalId] : null;
          if (mapping && atemStore.config.connected) {
            const newFocus = Math.max(0, Math.min(100, atemStore.cameraControls.focus - adjustedValue * 2));
            gimbalSocket.setAtemFocus(mapping.port, newFocus, cameraTypeToString(mapping.cameraType));
          }
        }
        break;
      }
      case 'atemZoom': {
        if (Math.abs(value) > 0.05) {
          const atemStore = useAtemStore.getState();
          const mapping = store.activeGimbalId ? atemStore.mappings[store.activeGimbalId] : null;
          if (mapping && atemStore.config.connected) {
            // Continuous zoom: 50 = stop, <50 = zoom out, >50 = zoom in
            const zoomSpeed = 50 + value * 50 * axisSensitivity;
            gimbalSocket.setAtemZoom(mapping.port, zoomSpeed);
          }
        }
        break;
      }
      case 'none':
      default:
        break;
    }
  }, [sensitivity]);

  const processGamepad = useCallback(() => {
    if (!connected) {
      animationFrameRef.current = requestAnimationFrame(processGamepad);
      return;
    }

    const gamepads = navigator.getGamepads();
    const gamepad = gamepads[0];

    if (gamepad) {
      const state = getGamepadState(gamepad);
      const prevButtons = previousButtonsRef.current;

      // Process button actions (on press, not hold)
      Object.entries(mapping.buttons).forEach(([indexStr, action]) => {
        const index = parseInt(indexStr);
        if (state.buttons[index] && !prevButtons[index]) {
          executeButtonAction(action);
        }
      });

      // Process axis actions
      Object.entries(mapping.axes).forEach(([indexStr, config]) => {
        const index = parseInt(indexStr);
        const value = state.axes[index] || 0;
        if (value !== 0) {
          applyAxisAction(config.action, value, config.sensitivity, config.inverted);
        }
      });

      // Process triggers for speed multiplier
      const { leftTrigger, rightTrigger } = mapping;
      if (leftTrigger.action === 'speedMultiplier' || rightTrigger.action === 'speedMultiplier') {
        let speedMultiplier = 1.0;

        if (rightTrigger.action === 'speedMultiplier') {
          const rtValue = rightTrigger.inverted ? -state.triggers.right : state.triggers.right;
          speedMultiplier += rtValue * rightTrigger.sensitivity;
        }
        if (leftTrigger.action === 'speedMultiplier') {
          const ltValue = leftTrigger.inverted ? state.triggers.left : -state.triggers.left;
          speedMultiplier += ltValue * leftTrigger.sensitivity;
        }

        if (state.triggers.left > 0.1 || state.triggers.right > 0.1) {
          gimbalSocket.setSpeedMultiplier(Math.max(0.25, Math.min(2.0, speedMultiplier)));
        }
      }

      previousButtonsRef.current = [...state.buttons];
    }

    animationFrameRef.current = requestAnimationFrame(processGamepad);
  }, [connected, mapping, getGamepadState, executeButtonAction, applyAxisAction]);

  useEffect(() => {
    // Start polling
    animationFrameRef.current = requestAnimationFrame(processGamepad);

    // Gamepad connection events
    const handleGamepadConnected = (e: GamepadEvent) => {
      console.log('Gamepad connected:', e.gamepad.id);
      setConnectedGamepad(e.gamepad.id);
    };

    const handleGamepadDisconnected = (e: GamepadEvent) => {
      console.log('Gamepad disconnected:', e.gamepad.id);
      setConnectedGamepad(null);
    };

    window.addEventListener('gamepadconnected', handleGamepadConnected);
    window.addEventListener('gamepaddisconnected', handleGamepadDisconnected);

    return () => {
      if (animationFrameRef.current) {
        cancelAnimationFrame(animationFrameRef.current);
      }
      window.removeEventListener('gamepadconnected', handleGamepadConnected);
      window.removeEventListener('gamepaddisconnected', handleGamepadDisconnected);
    };
  }, [processGamepad, setConnectedGamepad]);
}
