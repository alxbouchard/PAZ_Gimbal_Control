import { useState, useEffect, useCallback } from 'react';
import {
  Gamepad2,
  RotateCcw,
  X,
  ChevronDown,
  Zap,
  AlertCircle,
} from 'lucide-react';
import {
  useGamepadStore,
  BUTTON_NAMES,
  AXIS_NAMES,
  ACTION_LABELS,
  AXIS_ACTION_LABELS,
  type GamepadAction,
  type AxisAction,
} from '../../store/gamepadStore';

// Visual gamepad button component
function GamepadButton({
  index,
  isActive,
  isBinding,
  onClick,
}: {
  index: number;
  isActive: boolean;
  isBinding: boolean;
  onClick: () => void;
}) {
  const { mapping } = useGamepadStore();
  const action = mapping.buttons[index] || 'none';
  const label = BUTTON_NAMES[index] || `B${index}`;

  return (
    <button
      onClick={onClick}
      className={`
        relative p-3 rounded-lg border-2 transition-all
        ${isBinding ? 'border-gimbal-warning bg-gimbal-warning/20 animate-pulse' : ''}
        ${isActive && !isBinding ? 'border-gimbal-success bg-gimbal-success/20' : ''}
        ${!isActive && !isBinding ? 'border-gimbal-border bg-gimbal-panel hover:border-gimbal-accent' : ''}
      `}
    >
      <div className="text-xs font-bold text-gimbal-text mb-1">{label}</div>
      <div className="text-[10px] text-gimbal-text-dim truncate max-w-[80px]">
        {ACTION_LABELS[action]}
      </div>
      {isBinding && (
        <div className="absolute -top-2 -right-2 w-4 h-4 bg-gimbal-warning rounded-full flex items-center justify-center">
          <Zap size={10} className="text-black" />
        </div>
      )}
    </button>
  );
}

// Axis configuration row
function AxisConfigRow({
  axisIndex,
  currentValue,
}: {
  axisIndex: number;
  currentValue: number;
}) {
  const { mapping, setAxisMapping } = useGamepadStore();
  const config = mapping.axes[axisIndex];
  const [isOpen, setIsOpen] = useState(false);

  if (!config) return null;

  return (
    <div className="flex items-center gap-4 p-3 bg-gimbal-bg rounded-lg">
      <div className="w-28">
        <div className="text-sm font-medium text-gimbal-text">
          {AXIS_NAMES[axisIndex]}
        </div>
        <div className="h-2 bg-gimbal-border rounded-full mt-1 overflow-hidden">
          <div
            className="h-full bg-gimbal-accent transition-all"
            style={{
              width: `${Math.abs(currentValue) * 50}%`,
              marginLeft: currentValue < 0 ? `${50 - Math.abs(currentValue) * 50}%` : '50%',
            }}
          />
        </div>
      </div>

      <div className="relative flex-1">
        <button
          onClick={() => setIsOpen(!isOpen)}
          className="w-full flex items-center justify-between px-3 py-2 bg-gimbal-panel border border-gimbal-border rounded-lg text-sm text-gimbal-text"
        >
          <span>{AXIS_ACTION_LABELS[config.action]}</span>
          <ChevronDown size={14} className={`transition-transform ${isOpen ? 'rotate-180' : ''}`} />
        </button>
        {isOpen && (
          <div className="absolute top-full left-0 right-0 mt-1 bg-gimbal-panel border border-gimbal-border rounded-lg shadow-lg z-10 overflow-hidden">
            {(Object.keys(AXIS_ACTION_LABELS) as AxisAction[]).map((action) => (
              <button
                key={action}
                onClick={() => {
                  setAxisMapping(axisIndex, { action });
                  setIsOpen(false);
                }}
                className={`w-full px-3 py-2 text-left text-sm transition-colors ${
                  config.action === action
                    ? 'bg-gimbal-accent/20 text-gimbal-accent'
                    : 'text-gimbal-text hover:bg-gimbal-border/50'
                }`}
              >
                {AXIS_ACTION_LABELS[action]}
              </button>
            ))}
          </div>
        )}
      </div>

      <label className="flex items-center gap-2 cursor-pointer">
        <input
          type="checkbox"
          checked={config.inverted}
          onChange={(e) => setAxisMapping(axisIndex, { inverted: e.target.checked })}
          className="w-4 h-4 rounded border-gimbal-border bg-gimbal-bg text-gimbal-accent focus:ring-gimbal-accent"
        />
        <span className="text-xs text-gimbal-text-dim">Invert</span>
      </label>

      <div className="flex items-center gap-2">
        <span className="text-xs text-gimbal-text-dim">Sens:</span>
        <input
          type="range"
          min="0.1"
          max="2"
          step="0.1"
          value={config.sensitivity}
          onChange={(e) => setAxisMapping(axisIndex, { sensitivity: parseFloat(e.target.value) })}
          className="w-20 h-2 bg-gimbal-border rounded-lg appearance-none cursor-pointer"
        />
        <span className="text-xs text-gimbal-text w-8">{config.sensitivity.toFixed(1)}x</span>
      </div>
    </div>
  );
}

// Button action selector
function ButtonActionSelector({
  buttonIndex,
  onClose,
}: {
  buttonIndex: number;
  onClose: () => void;
}) {
  const { mapping, setButtonMapping } = useGamepadStore();
  const currentAction = mapping.buttons[buttonIndex] || 'none';

  return (
    <div className="fixed inset-0 bg-black/50 flex items-center justify-center z-50">
      <div className="bg-gimbal-panel border border-gimbal-border rounded-xl p-6 max-w-md w-full mx-4">
        <div className="flex items-center justify-between mb-4">
          <h3 className="text-lg font-semibold text-gimbal-text">
            Configure {BUTTON_NAMES[buttonIndex] || `Button ${buttonIndex}`}
          </h3>
          <button onClick={onClose} className="p-1 hover:bg-gimbal-border rounded">
            <X size={20} className="text-gimbal-text-dim" />
          </button>
        </div>

        <div className="grid grid-cols-2 gap-2">
          {(Object.keys(ACTION_LABELS) as GamepadAction[]).map((action) => (
            <button
              key={action}
              onClick={() => {
                setButtonMapping(buttonIndex, action);
                onClose();
              }}
              className={`p-3 rounded-lg border-2 text-left transition-all ${
                currentAction === action
                  ? 'border-gimbal-accent bg-gimbal-accent/20'
                  : 'border-gimbal-border hover:border-gimbal-accent/50'
              }`}
            >
              <div className="text-sm text-gimbal-text">{ACTION_LABELS[action]}</div>
            </button>
          ))}
        </div>

        <div className="mt-4 p-3 bg-gimbal-bg rounded-lg">
          <div className="flex items-center gap-2 text-xs text-gimbal-text-dim">
            <AlertCircle size={14} />
            <span>Or press another button on your gamepad to swap actions</span>
          </div>
        </div>
      </div>
    </div>
  );
}

export function GamepadConfig() {
  const {
    mapping,
    connectedGamepad,
    isBinding,
    setConnectedGamepad,
    cancelBinding,
    completeBinding,
    resetToDefaults,
    setDeadzone,
  } = useGamepadStore();

  const [gamepadState, setGamepadState] = useState<{
    buttons: boolean[];
    axes: number[];
    triggers: { left: number; right: number };
  }>({ buttons: [], axes: [], triggers: { left: 0, right: 0 } });
  const [selectedButton, setSelectedButton] = useState<number | null>(null);

  // Poll gamepad state
  const pollGamepad = useCallback(() => {
    const gamepads = navigator.getGamepads();
    const gamepad = gamepads[0];

    if (gamepad) {
      if (connectedGamepad !== gamepad.id) {
        setConnectedGamepad(gamepad.id);
      }

      const buttons = gamepad.buttons.map((b) => b.pressed);
      const axes = gamepad.axes.map((a) => a);
      const triggers = {
        left: gamepad.buttons[6]?.value || 0,
        right: gamepad.buttons[7]?.value || 0,
      };

      setGamepadState({ buttons, axes, triggers });

      // Handle binding mode
      if (isBinding && isBinding.type === 'button') {
        const pressedButton = buttons.findIndex((b, i) => b && !gamepadState.buttons[i]);
        if (pressedButton !== -1 && pressedButton !== isBinding.index) {
          completeBinding(pressedButton);
        }
      }
    } else if (connectedGamepad) {
      setConnectedGamepad(null);
    }
  }, [connectedGamepad, isBinding, gamepadState.buttons, setConnectedGamepad, completeBinding]);

  useEffect(() => {
    const interval = setInterval(pollGamepad, 50);
    return () => clearInterval(interval);
  }, [pollGamepad]);

  // Handle keyboard escape for binding
  useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      if (e.key === 'Escape') {
        if (isBinding) cancelBinding();
        if (selectedButton !== null) setSelectedButton(null);
      }
    };
    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [isBinding, selectedButton, cancelBinding]);

  const handleButtonClick = (index: number) => {
    setSelectedButton(index);
  };

  return (
    <div className="space-y-6">
      {/* Header */}
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-3">
          <div className={`p-2 rounded-lg ${connectedGamepad ? 'bg-gimbal-success/20' : 'bg-gimbal-border'}`}>
            <Gamepad2 size={20} className={connectedGamepad ? 'text-gimbal-success' : 'text-gimbal-text-dim'} />
          </div>
          <div>
            <h3 className="text-sm font-semibold text-gimbal-text">Gamepad Configuration</h3>
            <p className="text-xs text-gimbal-text-dim">
              {connectedGamepad ? connectedGamepad.substring(0, 40) + '...' : 'No gamepad connected'}
            </p>
          </div>
        </div>
        <button
          onClick={resetToDefaults}
          className="flex items-center gap-2 px-3 py-2 text-sm text-gimbal-text-dim hover:text-gimbal-text bg-gimbal-bg border border-gimbal-border rounded-lg transition-colors"
        >
          <RotateCcw size={14} />
          Reset Defaults
        </button>
      </div>

      {!connectedGamepad && (
        <div className="p-4 bg-gimbal-warning/10 border border-gimbal-warning/30 rounded-lg">
          <div className="flex items-center gap-2 text-gimbal-warning">
            <AlertCircle size={16} />
            <span className="text-sm">Connect a gamepad and press any button to detect it</span>
          </div>
        </div>
      )}

      {/* Deadzone setting */}
      <div className="p-4 bg-gimbal-bg rounded-lg">
        <div className="flex items-center justify-between">
          <div>
            <div className="text-sm font-medium text-gimbal-text">Deadzone</div>
            <div className="text-xs text-gimbal-text-dim">Ignore small stick movements</div>
          </div>
          <div className="flex items-center gap-3">
            <input
              type="range"
              min="0.05"
              max="0.4"
              step="0.05"
              value={mapping.deadzone}
              onChange={(e) => setDeadzone(parseFloat(e.target.value))}
              className="w-32 h-2 bg-gimbal-border rounded-lg appearance-none cursor-pointer"
            />
            <span className="text-sm text-gimbal-text w-12">{(mapping.deadzone * 100).toFixed(0)}%</span>
          </div>
        </div>
      </div>

      {/* Button mappings - Visual layout */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h4 className="text-sm font-semibold text-gimbal-text mb-4">Button Mappings</h4>
        <p className="text-xs text-gimbal-text-dim mb-4">Click a button to change its action</p>

        <div className="grid grid-cols-4 md:grid-cols-6 gap-3">
          {[0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15].map((index) => (
            <GamepadButton
              key={index}
              index={index}
              isActive={gamepadState.buttons[index] || false}
              isBinding={isBinding?.type === 'button' && isBinding.index === index}
              onClick={() => handleButtonClick(index)}
            />
          ))}
        </div>
      </div>

      {/* Axis mappings */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h4 className="text-sm font-semibold text-gimbal-text mb-4">Stick & Axis Mappings</h4>

        <div className="space-y-3">
          {[0, 1, 2, 3].map((index) => (
            <AxisConfigRow
              key={index}
              axisIndex={index}
              currentValue={gamepadState.axes[index] || 0}
            />
          ))}
        </div>
      </div>

      {/* Trigger mappings */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h4 className="text-sm font-semibold text-gimbal-text mb-4">Trigger Configuration</h4>

        <div className="grid grid-cols-2 gap-4">
          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="flex items-center justify-between mb-3">
              <span className="text-sm font-medium text-gimbal-text">Left Trigger (LT)</span>
              <div className="h-2 w-20 bg-gimbal-border rounded-full overflow-hidden">
                <div
                  className="h-full bg-gimbal-accent transition-all"
                  style={{ width: `${gamepadState.triggers.left * 100}%` }}
                />
              </div>
            </div>
            <div className="text-xs text-gimbal-text-dim">
              {AXIS_ACTION_LABELS[mapping.leftTrigger.action]}
              {mapping.leftTrigger.inverted ? ' (Inverted)' : ''}
            </div>
          </div>

          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="flex items-center justify-between mb-3">
              <span className="text-sm font-medium text-gimbal-text">Right Trigger (RT)</span>
              <div className="h-2 w-20 bg-gimbal-border rounded-full overflow-hidden">
                <div
                  className="h-full bg-gimbal-accent transition-all"
                  style={{ width: `${gamepadState.triggers.right * 100}%` }}
                />
              </div>
            </div>
            <div className="text-xs text-gimbal-text-dim">
              {AXIS_ACTION_LABELS[mapping.rightTrigger.action]}
              {mapping.rightTrigger.inverted ? ' (Inverted)' : ''}
            </div>
          </div>
        </div>
      </div>

      {/* Button selector modal */}
      {selectedButton !== null && (
        <ButtonActionSelector
          buttonIndex={selectedButton}
          onClose={() => setSelectedButton(null)}
        />
      )}
    </div>
  );
}
