import { Keyboard } from 'lucide-react';
import {
  useShortcutsStore,
  getActionsByCategory,
  formatBinding,
} from '../../store/shortcutsStore';

function KeyCap({ children }: { children: React.ReactNode }) {
  return (
    <span className="inline-flex items-center justify-center min-w-[28px] h-7 px-2 bg-gimbal-bg border border-gimbal-border rounded-md text-xs font-mono text-gimbal-text shadow-sm">
      {children}
    </span>
  );
}

export function ShortcutsPage() {
  const { bindings } = useShortcutsStore();
  const categories = getActionsByCategory();

  return (
    <div className="space-y-6 max-w-4xl">
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-2">
          <Keyboard size={20} className="text-gimbal-accent" />
          <h2 className="text-lg font-semibold text-gimbal-text">
            Keyboard Shortcuts
          </h2>
        </div>
      </div>

      <p className="text-sm text-gimbal-text-dim">
        Use these keyboard shortcuts for quick access to gimbal controls.
        You can customize these bindings in{' '}
        <span className="text-gimbal-accent">Settings</span>.
      </p>

      <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
        {Object.entries(categories).map(([category, actions]) => (
          <div
            key={category}
            className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6"
          >
            <h3 className="text-sm font-semibold text-gimbal-text mb-4">
              {category}
            </h3>

            <div className="space-y-3">
              {actions.map((action) => {
                const binding = bindings[action.id];
                return (
                  <div
                    key={action.id}
                    className="flex items-center justify-between gap-4"
                  >
                    <span className="text-sm text-gimbal-text-dim">
                      {action.label}
                    </span>
                    <KeyCap>{formatBinding(binding)}</KeyCap>
                  </div>
                );
              })}
            </div>
          </div>
        ))}
      </div>

      {/* Gamepad section */}
      <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-6">
        <h3 className="text-sm font-semibold text-gimbal-text mb-4">
          Xbox Gamepad Controls
        </h3>

        <p className="text-sm text-gimbal-text-dim mb-4">
          Full Xbox-style gamepad support. Connect a gamepad and it will be automatically detected.
        </p>

        <div className="grid grid-cols-2 md:grid-cols-3 gap-4">
          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="text-xs text-gimbal-text-dim mb-2">Left Stick</div>
            <div className="text-sm text-gimbal-text">Pitch & Yaw</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="text-xs text-gimbal-text-dim mb-2">Right Stick X</div>
            <div className="text-sm text-gimbal-text">Roll control</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="text-xs text-gimbal-text-dim mb-2">Right Stick Y</div>
            <div className="text-sm text-gimbal-text">Focus adjust</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg border-l-2 border-green-500">
            <div className="text-xs text-gimbal-text-dim mb-2">A Button</div>
            <div className="text-sm text-gimbal-text">Toggle Tracking</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg border-l-2 border-red-500">
            <div className="text-xs text-gimbal-text-dim mb-2">B Button</div>
            <div className="text-sm text-gimbal-text">Go Home</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg border-l-2 border-blue-500">
            <div className="text-xs text-gimbal-text-dim mb-2">X Button</div>
            <div className="text-sm text-gimbal-text">Set Home</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg border-l-2 border-yellow-500">
            <div className="text-xs text-gimbal-text-dim mb-2">Y Button</div>
            <div className="text-sm text-gimbal-text">Speed Boost</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="text-xs text-gimbal-text-dim mb-2">Start</div>
            <div className="text-sm text-gimbal-text">Emergency Stop</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="text-xs text-gimbal-text-dim mb-2">LB / RB</div>
            <div className="text-sm text-gimbal-text">Switch Gimbal</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="text-xs text-gimbal-text-dim mb-2">LT (Hold)</div>
            <div className="text-sm text-gimbal-text">Slow motion</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="text-xs text-gimbal-text-dim mb-2">RT (Hold)</div>
            <div className="text-sm text-gimbal-text">Fast motion</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="text-xs text-gimbal-text-dim mb-2">D-pad Up/Down</div>
            <div className="text-sm text-gimbal-text">Zoom In/Out</div>
          </div>
          <div className="p-4 bg-gimbal-bg rounded-lg">
            <div className="text-xs text-gimbal-text-dim mb-2">D-pad Left/Right</div>
            <div className="text-sm text-gimbal-text">Fine Yaw</div>
          </div>
        </div>
      </div>
    </div>
  );
}
