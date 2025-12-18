import { motion } from 'framer-motion';
import { Settings, Sliders, Gamepad, Monitor, Save } from 'lucide-react';
import { useGimbalStore } from '../../store/gimbalStore';
import { useState } from 'react';
import { ShortcutEditor } from '../ShortcutEditor';

export function SettingsPage() {
  const { controlMapping, setControlMapping, sensitivity, setSensitivity } = useGimbalStore();
  const [serverUrl, setServerUrl] = useState('http://localhost:3001');

  return (
    <div className="space-y-6 max-w-4xl">
      <div className="flex items-center gap-2">
        <Settings size={20} className="text-gimbal-accent" />
        <h2 className="text-lg font-semibold text-gimbal-text">Settings</h2>
      </div>

      {/* Connection Settings */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4 flex items-center gap-2">
          <Monitor size={16} />
          Connection
        </h3>

        <div className="space-y-4">
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
          </div>

          <div>
            <label className="block text-xs text-gimbal-text-dim mb-2">
              Gimbal IP Address (for real gimbal)
            </label>
            <input
              type="text"
              className="w-full px-4 py-2 bg-gimbal-bg border border-gimbal-border rounded-lg text-gimbal-text text-sm focus:outline-none focus:border-gimbal-accent"
              placeholder="192.168.0.200"
            />
          </div>
        </div>
      </div>

      {/* Control Mapping */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4 flex items-center gap-2">
          <Gamepad size={16} />
          Control Mapping
        </h3>

        <div className="grid grid-cols-2 gap-6">
          {/* Left Joystick */}
          <div className="space-y-4">
            <h4 className="text-xs font-medium text-gimbal-text-dim">
              LEFT JOYSTICK
            </h4>

            <div>
              <label className="block text-xs text-gimbal-text-dim mb-2">
                X Axis (Horizontal)
              </label>
              <select
                value={controlMapping.joystickLeft.x}
                onChange={(e) =>
                  setControlMapping({
                    joystickLeft: {
                      ...controlMapping.joystickLeft,
                      x: e.target.value as 'yaw' | 'roll' | 'none',
                    },
                  })
                }
                className="w-full px-4 py-2 bg-gimbal-bg border border-gimbal-border rounded-lg text-gimbal-text text-sm focus:outline-none focus:border-gimbal-accent"
              >
                <option value="yaw">Yaw</option>
                <option value="roll">Roll</option>
                <option value="none">None</option>
              </select>
            </div>

            <div>
              <label className="block text-xs text-gimbal-text-dim mb-2">
                Y Axis (Vertical)
              </label>
              <select
                value={controlMapping.joystickLeft.y}
                onChange={(e) =>
                  setControlMapping({
                    joystickLeft: {
                      ...controlMapping.joystickLeft,
                      y: e.target.value as 'pitch' | 'none',
                    },
                  })
                }
                className="w-full px-4 py-2 bg-gimbal-bg border border-gimbal-border rounded-lg text-gimbal-text text-sm focus:outline-none focus:border-gimbal-accent"
              >
                <option value="pitch">Pitch</option>
                <option value="none">None</option>
              </select>
            </div>
          </div>

          {/* Right Joystick */}
          <div className="space-y-4">
            <h4 className="text-xs font-medium text-gimbal-text-dim">
              RIGHT JOYSTICK
            </h4>

            <div>
              <label className="block text-xs text-gimbal-text-dim mb-2">
                X Axis (Horizontal)
              </label>
              <select
                value={controlMapping.joystickRight.x}
                onChange={(e) =>
                  setControlMapping({
                    joystickRight: {
                      ...controlMapping.joystickRight,
                      x: e.target.value as 'yaw' | 'roll' | 'none',
                    },
                  })
                }
                className="w-full px-4 py-2 bg-gimbal-bg border border-gimbal-border rounded-lg text-gimbal-text text-sm focus:outline-none focus:border-gimbal-accent"
              >
                <option value="roll">Roll</option>
                <option value="yaw">Yaw</option>
                <option value="none">None</option>
              </select>
            </div>

            <div>
              <label className="block text-xs text-gimbal-text-dim mb-2">
                Y Axis (Vertical)
              </label>
              <select
                value={controlMapping.joystickRight.y}
                onChange={(e) =>
                  setControlMapping({
                    joystickRight: {
                      ...controlMapping.joystickRight,
                      y: e.target.value as 'focus' | 'zoom' | 'none',
                    },
                  })
                }
                className="w-full px-4 py-2 bg-gimbal-bg border border-gimbal-border rounded-lg text-gimbal-text text-sm focus:outline-none focus:border-gimbal-accent"
              >
                <option value="focus">Focus</option>
                <option value="zoom">Zoom</option>
                <option value="none">None</option>
              </select>
            </div>
          </div>
        </div>

        {/* Invert options */}
        <div className="mt-6 grid grid-cols-3 gap-4">
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
          Sensitivity
        </h3>

        <div className="space-y-4">
          <div>
            <div className="flex justify-between mb-2">
              <label className="text-xs text-gimbal-text-dim">
                Default Sensitivity
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
      </div>

      {/* Keyboard Shortcuts */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <ShortcutEditor />
      </div>

      {/* Save Button */}
      <motion.button
        className="w-full py-3 bg-gimbal-accent text-white rounded-xl font-medium flex items-center justify-center gap-2 hover:bg-gimbal-accent-hover transition-colors"
        whileHover={{ scale: 1.01 }}
        whileTap={{ scale: 0.99 }}
      >
        <Save size={18} />
        Save Settings
      </motion.button>
    </div>
  );
}
