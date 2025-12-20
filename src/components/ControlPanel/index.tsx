import { useState, useCallback, useRef, useEffect } from 'react';
import { motion } from 'framer-motion';
import {
  Home,
  Target,
  Zap,
  StopCircle,
  ZoomIn,
  Focus,
  RotateCcw,
  Gauge,
  Radio,
  Monitor,
} from 'lucide-react';
import { useGimbalStore } from '../../store/gimbalStore';
import { gimbalSocket } from '../../services/websocket';

interface JoystickProps {
  label: string;
  onMove: (x: number, y: number) => void;
  onRelease: () => void;
  disabled?: boolean;
}

function Joystick({ label, onMove, onRelease, disabled }: JoystickProps) {
  const containerRef = useRef<HTMLDivElement>(null);
  const [position, setPosition] = useState({ x: 0, y: 0 });
  const [isDragging, setIsDragging] = useState(false);

  const handleMove = useCallback(
    (clientX: number, clientY: number) => {
      if (!containerRef.current || disabled) return;

      const rect = containerRef.current.getBoundingClientRect();
      const centerX = rect.left + rect.width / 2;
      const centerY = rect.top + rect.height / 2;
      const maxRadius = rect.width / 2 - 20;

      let dx = clientX - centerX;
      let dy = clientY - centerY;

      // Clamp to circle
      const distance = Math.sqrt(dx * dx + dy * dy);
      if (distance > maxRadius) {
        dx = (dx / distance) * maxRadius;
        dy = (dy / distance) * maxRadius;
      }

      setPosition({ x: dx, y: dy });
      onMove(dx / maxRadius, dy / maxRadius);
    },
    [onMove, disabled]
  );

  const handleStart = useCallback(
    (e: React.MouseEvent | React.TouchEvent) => {
      if (disabled) return;
      setIsDragging(true);
      const point = 'touches' in e ? e.touches[0] : e;
      handleMove(point.clientX, point.clientY);
    },
    [handleMove, disabled]
  );

  const handleEnd = useCallback(() => {
    setIsDragging(false);
    setPosition({ x: 0, y: 0 });
    onRelease();
  }, [onRelease]);

  useEffect(() => {
    if (!isDragging) return;

    const handleMouseMove = (e: MouseEvent) => handleMove(e.clientX, e.clientY);
    const handleTouchMove = (e: TouchEvent) => {
      e.preventDefault();
      handleMove(e.touches[0].clientX, e.touches[0].clientY);
    };

    window.addEventListener('mousemove', handleMouseMove);
    window.addEventListener('mouseup', handleEnd);
    window.addEventListener('touchmove', handleTouchMove, { passive: false });
    window.addEventListener('touchend', handleEnd);

    return () => {
      window.removeEventListener('mousemove', handleMouseMove);
      window.removeEventListener('mouseup', handleEnd);
      window.removeEventListener('touchmove', handleTouchMove);
      window.removeEventListener('touchend', handleEnd);
    };
  }, [isDragging, handleMove, handleEnd]);

  return (
    <div className="flex flex-col items-center gap-2">
      <span className="text-xs font-medium text-gimbal-text-dim uppercase tracking-wider">
        {label}
      </span>
      <div
        ref={containerRef}
        className={`relative w-32 h-32 rounded-full bg-gimbal-bg border-2 border-gimbal-border ${
          disabled ? 'opacity-50 cursor-not-allowed' : 'cursor-pointer'
        }`}
        onMouseDown={handleStart}
        onTouchStart={handleStart}
      >
        {/* Grid lines */}
        <div className="absolute inset-0 flex items-center justify-center">
          <div className="w-full h-px bg-gimbal-border" />
        </div>
        <div className="absolute inset-0 flex items-center justify-center">
          <div className="h-full w-px bg-gimbal-border" />
        </div>

        {/* Joystick knob */}
        <motion.div
          className={`absolute w-10 h-10 rounded-full bg-gimbal-accent shadow-lg ${
            isDragging ? 'scale-110' : ''
          }`}
          style={{
            left: '50%',
            top: '50%',
            x: position.x - 20,
            y: position.y - 20,
          }}
          animate={{ scale: isDragging ? 1.1 : 1 }}
          transition={{ type: 'spring', stiffness: 400, damping: 25 }}
        />
      </div>
      <div className="text-xs font-mono text-gimbal-text-dim">
        X: {(position.x / 44).toFixed(2)} Y: {(position.y / 44).toFixed(2)}
      </div>
    </div>
  );
}

interface SliderProps {
  label: string;
  value: number;
  onChange: (value: number) => void;
  icon: React.ReactNode;
  disabled?: boolean;
}

function VerticalSlider({ label, value, onChange, icon, disabled }: SliderProps) {
  return (
    <div className="flex flex-col items-center gap-2">
      <span className="text-xs font-medium text-gimbal-text-dim uppercase tracking-wider">
        {label}
      </span>
      <div className="flex flex-col items-center gap-2 h-32">
        <div className="text-gimbal-accent">{icon}</div>
        <input
          type="range"
          min="0"
          max="100"
          value={value}
          onChange={(e) => onChange(Number(e.target.value))}
          disabled={disabled}
          className="h-24 w-2 appearance-none bg-gimbal-border rounded-full cursor-pointer disabled:opacity-50 disabled:cursor-not-allowed"
          style={{
            writingMode: 'vertical-lr',
            direction: 'rtl',
          }}
        />
      </div>
      <span className="text-xs font-mono text-gimbal-text-dim">{value}%</span>
    </div>
  );
}

function GimbalSwitcher() {
  const { availableGimbals, activeGimbalId } = useGimbalStore();

  const handleSelectGimbal = (gimbalId: string) => {
    gimbalSocket.selectGimbal(gimbalId);
  };

  if (availableGimbals.length === 0) {
    return null;
  }

  return (
    <div className="space-y-2">
      <div className="flex items-center gap-2">
        <Radio size={14} className="text-gimbal-text-dim" />
        <span className="text-xs font-medium text-gimbal-text-dim uppercase tracking-wider">
          Gimbal Selection
        </span>
      </div>
      <div className="flex flex-wrap gap-2">
        {availableGimbals.map((gimbal, index) => {
          const isActive = gimbal.id === activeGimbalId;
          const isVirtual = gimbal.mode === 'virtual';
          const isConnected = gimbal.connected;
          const controlledBy = gimbal.controlledBy;
          const hasController = controlledBy && !isVirtual;

          return (
            <motion.button
              key={gimbal.id}
              onClick={() => handleSelectGimbal(gimbal.id)}
              className={`relative flex flex-col items-start gap-0.5 px-3 py-2 rounded-lg border-2 transition-all ${
                isActive
                  ? isVirtual
                    ? 'bg-blue-500/20 border-blue-500 text-blue-400'
                    : 'bg-purple-500/20 border-purple-500 text-purple-400'
                  : isConnected || isVirtual
                    ? 'bg-gimbal-bg border-gimbal-border text-gimbal-text hover:border-gimbal-accent'
                    : 'bg-gimbal-bg border-gimbal-border/50 text-gimbal-text-dim opacity-60'
              }`}
              whileHover={{ scale: isConnected || isVirtual ? 1.02 : 1 }}
              whileTap={{ scale: isConnected || isVirtual ? 0.98 : 1 }}
              disabled={!isConnected && !isVirtual}
            >
              <div className="flex items-center gap-2">
                {isVirtual ? (
                  <Monitor size={14} />
                ) : (
                  <Radio size={14} />
                )}
                <span className="text-sm font-medium">
                  {index + 1}
                </span>
                <span className="text-xs truncate max-w-[80px]">
                  {gimbal.name}
                </span>
              </div>
              {hasController && (
                <span className={`text-[10px] ${isActive ? 'text-purple-300' : 'text-green-400'} font-medium`}>
                  {controlledBy}
                </span>
              )}
              {isActive && (
                <motion.div
                  className={`absolute -top-1 -right-1 w-3 h-3 rounded-full ${
                    isVirtual ? 'bg-blue-500' : 'bg-purple-500'
                  }`}
                  layoutId="activeGimbalIndicator"
                  initial={false}
                  transition={{ type: 'spring', stiffness: 500, damping: 30 }}
                />
              )}
              {!isVirtual && !isConnected && (
                <span className="absolute -bottom-1 left-1/2 -translate-x-1/2 text-[8px] text-gimbal-error bg-gimbal-panel px-1 rounded">
                  offline
                </span>
              )}
            </motion.button>
          );
        })}
      </div>
    </div>
  );
}

function SpeedSlider({ value, onChange, disabled }: { value: number; onChange: (v: number) => void; disabled?: boolean }) {
  const speedPresets = [
    { label: 'Slow', value: 0.25, color: 'bg-blue-500' },
    { label: 'Normal', value: 0.5, color: 'bg-green-500' },
    { label: 'Fast', value: 1.0, color: 'bg-yellow-500' },
    { label: 'Max', value: 2.0, color: 'bg-red-500' },
  ];

  return (
    <div className="flex flex-col gap-3">
      <div className="flex items-center gap-2">
        <Gauge size={16} className="text-gimbal-accent" />
        <span className="text-xs font-medium text-gimbal-text-dim uppercase tracking-wider">
          Speed
        </span>
        <span className="ml-auto text-sm font-mono text-gimbal-accent">
          {(value * 100).toFixed(0)}%
        </span>
      </div>

      {/* Slider */}
      <div className="relative">
        <input
          type="range"
          min="0.1"
          max="2"
          step="0.05"
          value={value}
          onChange={(e) => onChange(Number(e.target.value))}
          disabled={disabled}
          className="w-full h-2 appearance-none bg-gimbal-border rounded-full cursor-pointer disabled:opacity-50 disabled:cursor-not-allowed
            [&::-webkit-slider-thumb]:appearance-none
            [&::-webkit-slider-thumb]:w-4
            [&::-webkit-slider-thumb]:h-4
            [&::-webkit-slider-thumb]:rounded-full
            [&::-webkit-slider-thumb]:bg-gimbal-accent
            [&::-webkit-slider-thumb]:cursor-pointer
            [&::-webkit-slider-thumb]:shadow-lg
            [&::-webkit-slider-thumb]:transition-transform
            [&::-webkit-slider-thumb]:hover:scale-110"
        />
        {/* Speed markers */}
        <div className="flex justify-between mt-1 px-1">
          <span className="text-[10px] text-gimbal-text-dim">10%</span>
          <span className="text-[10px] text-gimbal-text-dim">100%</span>
          <span className="text-[10px] text-gimbal-text-dim">200%</span>
        </div>
      </div>

      {/* Preset buttons */}
      <div className="flex gap-2">
        {speedPresets.map((preset) => (
          <button
            key={preset.label}
            onClick={() => onChange(preset.value)}
            disabled={disabled}
            className={`flex-1 px-2 py-1.5 text-xs font-medium rounded-md border transition-all disabled:opacity-50 disabled:cursor-not-allowed ${
              Math.abs(value - preset.value) < 0.05
                ? `${preset.color} border-transparent text-white`
                : 'bg-gimbal-bg border-gimbal-border text-gimbal-text-dim hover:border-gimbal-accent'
            }`}
          >
            {preset.label}
          </button>
        ))}
      </div>
    </div>
  );
}

export function ControlPanel() {
  const { connected, tracking, speedBoost, zoom, focus, sensitivity, setSensitivity, controlMapping } = useGimbalStore();

  // Track current joystick positions for dynamic speed updates
  const leftJoystickPos = useRef({ x: 0, y: 0 });
  const rightJoystickPos = useRef({ x: 0, y: 0 });

  // Apply current speed based on joystick positions
  const applyJoystickSpeed = useCallback((newSensitivity?: number) => {
    const speed = newSensitivity ?? sensitivity;
    const left = leftJoystickPos.current;
    const right = rightJoystickPos.current;

    // Apply invert settings
    const yawMultiplier = controlMapping.invertYaw ? -1 : 1;
    const pitchMultiplier = controlMapping.invertPitch ? -1 : 1;
    const rollMultiplier = controlMapping.invertRoll ? -1 : 1;

    // Only send if there's active joystick input
    if (left.x !== 0 || left.y !== 0 || right.x !== 0) {
      gimbalSocket.setSpeed({
        yaw: left.x * speed * yawMultiplier,
        pitch: -left.y * speed * pitchMultiplier,
        roll: right.x * speed * rollMultiplier,
      });
    }
  }, [sensitivity, controlMapping.invertYaw, controlMapping.invertPitch, controlMapping.invertRoll]);

  // Update server speed multiplier when sensitivity changes
  const handleSpeedChange = useCallback((value: number) => {
    setSensitivity(value);
    gimbalSocket.setSpeedMultiplier(value);
    // Immediately apply new speed to current joystick movement
    applyJoystickSpeed(value);
  }, [setSensitivity, applyJoystickSpeed]);

  const handleLeftJoystickMove = useCallback(
    (x: number, y: number) => {
      leftJoystickPos.current = { x, y };
      const yawMultiplier = controlMapping.invertYaw ? -1 : 1;
      const pitchMultiplier = controlMapping.invertPitch ? -1 : 1;
      gimbalSocket.setSpeed({
        yaw: x * sensitivity * yawMultiplier,
        pitch: -y * sensitivity * pitchMultiplier,
      });
    },
    [sensitivity, controlMapping.invertYaw, controlMapping.invertPitch]
  );

  const handleRightJoystickMove = useCallback(
    (x: number, y: number) => {
      rightJoystickPos.current = { x, y };
      const rollMultiplier = controlMapping.invertRoll ? -1 : 1;
      gimbalSocket.setSpeed({
        roll: x * sensitivity * rollMultiplier,
      });
    },
    [sensitivity, controlMapping.invertRoll]
  );

  const handleJoystickRelease = useCallback(() => {
    leftJoystickPos.current = { x: 0, y: 0 };
    rightJoystickPos.current = { x: 0, y: 0 };
    gimbalSocket.setSpeed({ pitch: 0, yaw: 0, roll: 0 });
  }, []);

  return (
    <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6 space-y-6">
      <div className="flex items-center justify-between">
        <h2 className="text-lg font-semibold text-gimbal-text">Control Panel</h2>
        <div className="flex items-center gap-2">
          {!connected && (
            <span className="text-xs text-gimbal-error">Not connected</span>
          )}
        </div>
      </div>

      {/* Gimbal Switcher */}
      <GimbalSwitcher />

      {/* Speed Control */}
      <SpeedSlider
        value={sensitivity}
        onChange={handleSpeedChange}
        disabled={!connected}
      />

      {/* Joysticks */}
      <div className="flex justify-center gap-8 flex-wrap">
        <Joystick
          label="Pitch / Yaw"
          onMove={handleLeftJoystickMove}
          onRelease={handleJoystickRelease}
          disabled={!connected}
        />
        <Joystick
          label="Roll"
          onMove={handleRightJoystickMove}
          onRelease={handleJoystickRelease}
          disabled={!connected}
        />
      </div>

      {/* Zoom & Focus sliders */}
      <div className="flex justify-center gap-8">
        <VerticalSlider
          label="Zoom"
          value={zoom}
          onChange={(v) => gimbalSocket.setZoom(v)}
          icon={<ZoomIn size={16} />}
          disabled={!connected}
        />
        <VerticalSlider
          label="Focus"
          value={focus}
          onChange={(v) => gimbalSocket.setFocus(v)}
          icon={<Focus size={16} />}
          disabled={!connected}
        />
      </div>

      {/* Quick Actions */}
      <div className="grid grid-cols-2 sm:grid-cols-4 gap-3">
        <motion.button
          onClick={() => gimbalSocket.goHome()}
          disabled={!connected}
          className="flex items-center justify-center gap-2 px-4 py-3 bg-gimbal-bg border border-gimbal-border rounded-lg text-gimbal-text hover:bg-gimbal-border/50 disabled:opacity-50 disabled:cursor-not-allowed transition-colors"
          whileHover={{ scale: connected ? 1.02 : 1 }}
          whileTap={{ scale: connected ? 0.98 : 1 }}
        >
          <Home size={18} />
          <span className="text-sm">Home</span>
        </motion.button>

        <motion.button
          onClick={() => gimbalSocket.toggleTracking(!tracking)}
          disabled={!connected}
          className={`flex items-center justify-center gap-2 px-4 py-3 border rounded-lg text-sm disabled:opacity-50 disabled:cursor-not-allowed transition-colors ${
            tracking
              ? 'bg-gimbal-success/20 border-gimbal-success text-gimbal-success'
              : 'bg-gimbal-bg border-gimbal-border text-gimbal-text hover:bg-gimbal-border/50'
          }`}
          whileHover={{ scale: connected ? 1.02 : 1 }}
          whileTap={{ scale: connected ? 0.98 : 1 }}
        >
          <Target size={18} />
          <span>Track</span>
        </motion.button>

        <motion.button
          onClick={() => gimbalSocket.toggleSpeedBoost(!speedBoost)}
          disabled={!connected}
          className={`flex items-center justify-center gap-2 px-4 py-3 border rounded-lg text-sm disabled:opacity-50 disabled:cursor-not-allowed transition-colors ${
            speedBoost
              ? 'bg-gimbal-warning/20 border-gimbal-warning text-gimbal-warning'
              : 'bg-gimbal-bg border-gimbal-border text-gimbal-text hover:bg-gimbal-border/50'
          }`}
          whileHover={{ scale: connected ? 1.02 : 1 }}
          whileTap={{ scale: connected ? 0.98 : 1 }}
        >
          <Zap size={18} />
          <span>Boost</span>
        </motion.button>

        <motion.button
          onClick={() => gimbalSocket.stopSpeed()}
          disabled={!connected}
          className="flex items-center justify-center gap-2 px-4 py-3 bg-gimbal-error/20 border border-gimbal-error rounded-lg text-gimbal-error disabled:opacity-50 disabled:cursor-not-allowed transition-colors"
          whileHover={{ scale: connected ? 1.02 : 1 }}
          whileTap={{ scale: connected ? 0.98 : 1 }}
        >
          <StopCircle size={18} />
          <span className="text-sm">Stop</span>
        </motion.button>
      </div>

      {/* Secondary Actions */}
      <div className="flex justify-center gap-3">
        <motion.button
          onClick={() => gimbalSocket.setHome()}
          disabled={!connected}
          className="flex items-center gap-2 px-3 py-2 text-xs text-gimbal-text-dim hover:text-gimbal-text disabled:opacity-50 disabled:cursor-not-allowed"
          whileHover={{ scale: connected ? 1.05 : 1 }}
        >
          <RotateCcw size={14} />
          Set as Home
        </motion.button>

        <motion.button
          onClick={() => gimbalSocket.calibrateFocus()}
          disabled={!connected}
          className="flex items-center gap-2 px-3 py-2 text-xs text-gimbal-text-dim hover:text-gimbal-text disabled:opacity-50 disabled:cursor-not-allowed"
          whileHover={{ scale: connected ? 1.05 : 1 }}
        >
          <Focus size={14} />
          Auto Focus
        </motion.button>
      </div>
    </div>
  );
}
