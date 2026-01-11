import { Settings, Sliders, Monitor, Gamepad2, RotateCcw } from 'lucide-react';
import { useGimbalStore } from '../../store/gimbalStore';
import { ShortcutEditor } from '../ShortcutEditor';
import { GamepadConfig } from '../GamepadConfig';
import { GimbalManager } from '../GimbalManager';
import { AtemSettings } from '../AtemSettings';

export function SettingsPage() {
  const { controlMapping, setControlMapping, sensitivity, setSensitivity, serverUrl, setServerUrl } = useGimbalStore();

  return (
    <div className="space-y-6 max-w-4xl">
      <div className="flex items-center gap-2">
        <Settings size={20} className="text-gimbal-accent" />
        <h2 className="text-lg font-semibold text-gimbal-text">Settings</h2>
      </div>

      {/* Gimbal Devices - Most important, at the top */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <GimbalManager />
      </div>

      {/* ATEM Switcher Configuration */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <AtemSettings />
      </div>

      {/* Connection Settings */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4 flex items-center gap-2">
          <Monitor size={16} />
          Server Connection
        </h3>

        <div>
          <label className="block text-xs text-gimbal-text-dim mb-2">
            WebSocket Server URL
          </label>
          <input
            type="text"
            value={serverUrl}
            onChange={(e) => setServerUrl(e.target.value)}
            className="w-full px-4 py-2 bg-gimbal-bg border border-gimbal-border rounded-lg text-gimbal-text text-sm focus:outline-none focus:border-gimbal-accent"
            placeholder="http://localhost:3001"
          />
          <p className="text-xs text-gimbal-text-dim mt-2">
            The Python bridge server that handles gimbal communication
          </p>
        </div>
      </div>

      {/* Gamepad Configuration - Primary input method */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4 flex items-center gap-2">
          <Gamepad2 size={16} />
          Xbox Gamepad
        </h3>
        <GamepadConfig />
      </div>

      {/* Keyboard Shortcuts */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <ShortcutEditor />
      </div>

      {/* Axis Inversion */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4 flex items-center gap-2">
          <RotateCcw size={16} />
          Axis Inversion
        </h3>
        <p className="text-xs text-gimbal-text-dim mb-4">
          Invert movement direction for on-screen joysticks and keyboard controls.
        </p>

        <div className="grid grid-cols-3 gap-4">
          <label className="flex items-center gap-2 cursor-pointer">
            <input
              type="checkbox"
              checked={controlMapping.invertPitch}
              onChange={(e) =>
                setControlMapping({ invertPitch: e.target.checked })
              }
              className="w-4 h-4 rounded border-gimbal-border bg-gimbal-bg text-gimbal-accent focus:ring-gimbal-accent"
            />
            <span className="text-sm text-gimbal-text">Invert Pitch</span>
          </label>
          <label className="flex items-center gap-2 cursor-pointer">
            <input
              type="checkbox"
              checked={controlMapping.invertYaw}
              onChange={(e) =>
                setControlMapping({ invertYaw: e.target.checked })
              }
              className="w-4 h-4 rounded border-gimbal-border bg-gimbal-bg text-gimbal-accent focus:ring-gimbal-accent"
            />
            <span className="text-sm text-gimbal-text">Invert Yaw</span>
          </label>
          <label className="flex items-center gap-2 cursor-pointer">
            <input
              type="checkbox"
              checked={controlMapping.invertRoll}
              onChange={(e) =>
                setControlMapping({ invertRoll: e.target.checked })
              }
              className="w-4 h-4 rounded border-gimbal-border bg-gimbal-bg text-gimbal-accent focus:ring-gimbal-accent"
            />
            <span className="text-sm text-gimbal-text">Invert Roll</span>
          </label>
        </div>
      </div>

      {/* Sensitivity Settings */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4 flex items-center gap-2">
          <Sliders size={16} />
          Default Speed
        </h3>
        <p className="text-xs text-gimbal-text-dim mb-4">
          Default movement speed for on-screen joysticks and keyboard. Can also be adjusted in the Control tab.
        </p>

        <div>
          <div className="flex justify-between mb-2">
            <label className="text-xs text-gimbal-text-dim">
              Speed Multiplier
            </label>
            <span className="text-sm font-mono text-gimbal-accent">
              {(sensitivity * 100).toFixed(0)}%
            </span>
          </div>
          <input
            type="range"
            min="0.1"
            max="2"
            step="0.1"
            value={sensitivity}
            onChange={(e) => setSensitivity(parseFloat(e.target.value))}
            className="w-full h-2 bg-gimbal-bg rounded-lg appearance-none cursor-pointer"
          />
          <div className="flex justify-between text-xs text-gimbal-text-dim mt-1">
            <span>10%</span>
            <span>100%</span>
            <span>200%</span>
          </div>
        </div>
      </div>

      {/* Auto-save notice */}
      <div className="text-center text-xs text-gimbal-text-dim py-2">
        All settings are saved automatically
      </div>
    </div>
  );
}
