import { useEffect, useRef, useCallback } from 'react';
import { useGimbalStore } from '../store/gimbalStore';
import { gimbalSocket } from '../services/websocket';

interface GamepadState {
  leftStick: { x: number; y: number };
  rightStick: { x: number; y: number };
  buttons: boolean[];
  triggers: { left: number; right: number };
}

export function useGamepad() {
  const { sensitivity, connected } = useGimbalStore();
  const animationFrameRef = useRef<number>();
  const previousButtonsRef = useRef<boolean[]>([]);
  const deadzone = 0.15;

  const applyDeadzone = (value: number): number => {
    if (Math.abs(value) < deadzone) return 0;
    // Scale the value to account for the deadzone
    const sign = value > 0 ? 1 : -1;
    return sign * ((Math.abs(value) - deadzone) / (1 - deadzone));
  };

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
    };
  }, []);

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

      // Left stick - Pitch/Yaw
      if (state.leftStick.x !== 0 || state.leftStick.y !== 0) {
        gimbalSocket.setSpeed({
          yaw: state.leftStick.x * sensitivity,
          pitch: -state.leftStick.y * sensitivity, // Invert Y
        });
      }

      // Right stick - Roll/Focus
      if (state.rightStick.x !== 0) {
        gimbalSocket.setSpeed({
          roll: state.rightStick.x * sensitivity,
        });
      }
      if (Math.abs(state.rightStick.y) > 0.1) {
        const currentFocus = useGimbalStore.getState().focus;
        gimbalSocket.setFocus(
          Math.max(0, Math.min(100, currentFocus - state.rightStick.y * 2))
        );
      }

      // Button A (index 0) - Toggle Tracking
      if (state.buttons[0] && !prevButtons[0]) {
        const currentTracking = useGimbalStore.getState().tracking;
        gimbalSocket.toggleTracking(!currentTracking);
      }

      // Button B (index 1) - Go Home
      if (state.buttons[1] && !prevButtons[1]) {
        gimbalSocket.goHome();
      }

      // Button X (index 2) - Set Home
      if (state.buttons[2] && !prevButtons[2]) {
        gimbalSocket.setHome();
      }

      // Button Y (index 3) - Toggle Speed Boost
      if (state.buttons[3] && !prevButtons[3]) {
        const currentBoost = useGimbalStore.getState().speedBoost;
        gimbalSocket.toggleSpeedBoost(!currentBoost);
      }

      // Start button (index 9) - Emergency Stop
      if (state.buttons[9] && !prevButtons[9]) {
        gimbalSocket.stopSpeed();
      }

      // Triggers - Speed modifier
      const triggerBoost = state.triggers.right - state.triggers.left;
      if (Math.abs(triggerBoost) > 0.1) {
        // Could be used for speed adjustment
      }

      // LB/RB (4/5) - Gimbal selection
      if (state.buttons[4] && !prevButtons[4]) {
        // Previous gimbal
        const gimbals = useGimbalStore.getState().availableGimbals;
        const currentId = useGimbalStore.getState().activeGimbalId;
        const currentIndex = gimbals.findIndex((g) => g.id === currentId);
        const prevIndex = (currentIndex - 1 + gimbals.length) % gimbals.length;
        gimbalSocket.selectGimbal(gimbals[prevIndex].id);
      }
      if (state.buttons[5] && !prevButtons[5]) {
        // Next gimbal
        const gimbals = useGimbalStore.getState().availableGimbals;
        const currentId = useGimbalStore.getState().activeGimbalId;
        const currentIndex = gimbals.findIndex((g) => g.id === currentId);
        const nextIndex = (currentIndex + 1) % gimbals.length;
        gimbalSocket.selectGimbal(gimbals[nextIndex].id);
      }

      previousButtonsRef.current = [...state.buttons];
    }

    animationFrameRef.current = requestAnimationFrame(processGamepad);
  }, [connected, sensitivity, getGamepadState]);

  useEffect(() => {
    // Start polling
    animationFrameRef.current = requestAnimationFrame(processGamepad);

    // Gamepad connection events
    const handleGamepadConnected = (e: GamepadEvent) => {
      console.log('Gamepad connected:', e.gamepad.id);
    };

    const handleGamepadDisconnected = (e: GamepadEvent) => {
      console.log('Gamepad disconnected:', e.gamepad.id);
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
  }, [processGamepad]);
}
