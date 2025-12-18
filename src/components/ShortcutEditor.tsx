import { useState, useEffect, useCallback } from 'react';
import { motion } from 'framer-motion';
import { Keyboard, RotateCcw, X } from 'lucide-react';
import {
  useShortcutsStore,
  ACTION_DEFINITIONS,
  getActionsByCategory,
  formatBinding,
  type ActionId,
  type ShortcutBinding,
} from '../store/shortcutsStore';

interface KeyCaptureModalProps {
  actionLabel: string;
  onCapture: (binding: ShortcutBinding) => void;
  onCancel: () => void;
}

function KeyCaptureModal({ actionLabel, onCapture, onCancel }: KeyCaptureModalProps) {
  const [captured, setCaptured] = useState<ShortcutBinding | null>(null);

  useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      e.preventDefault();
      e.stopPropagation();

      // Ignore modifier-only keys
      if (['Shift', 'Control', 'Alt', 'Meta'].includes(e.key)) {
        return;
      }

      // Escape to cancel
      if (e.key === 'Escape') {
        onCancel();
        return;
      }

      const binding: ShortcutBinding = {
        key: e.key,
        modifiers: {
          shift: e.shiftKey,
          ctrl: e.ctrlKey,
          alt: e.altKey,
          meta: e.metaKey,
        },
      };

      // Clean up modifiers (remove false values)
      if (!binding.modifiers?.shift && !binding.modifiers?.ctrl &&
          !binding.modifiers?.alt && !binding.modifiers?.meta) {
        delete binding.modifiers;
      }

      setCaptured(binding);
    };

    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [onCancel]);

  return (
    <motion.div
      initial={{ opacity: 0 }}
      animate={{ opacity: 1 }}
      exit={{ opacity: 0 }}
      className="fixed inset-0 bg-black/50 flex items-center justify-center z-50"
      onClick={onCancel}
    >
      <motion.div
        initial={{ scale: 0.9, opacity: 0 }}
        animate={{ scale: 1, opacity: 1 }}
        exit={{ scale: 0.9, opacity: 0 }}
        className="bg-gimbal-panel border border-gimbal-border rounded-xl p-6 max-w-md w-full mx-4"
        onClick={(e) => e.stopPropagation()}
      >
        <div className="flex items-center justify-between mb-4">
          <h3 className="text-lg font-semibold text-gimbal-text">
            Bind Key for "{actionLabel}"
          </h3>
          <button
            onClick={onCancel}
            className="p-1 text-gimbal-text-dim hover:text-gimbal-text"
          >
            <X size={20} />
          </button>
        </div>

        <div className="text-center py-8">
          {captured ? (
            <div className="space-y-4">
              <div className="text-2xl font-mono text-gimbal-accent">
                {formatBinding(captured)}
              </div>
              <div className="flex justify-center gap-3">
                <button
                  onClick={() => onCapture(captured)}
                  className="px-4 py-2 bg-gimbal-accent text-white rounded-lg hover:bg-gimbal-accent/80"
                >
                  Confirm
                </button>
                <button
                  onClick={() => setCaptured(null)}
                  className="px-4 py-2 bg-gimbal-bg border border-gimbal-border text-gimbal-text rounded-lg hover:bg-gimbal-border/50"
                >
                  Try Again
                </button>
              </div>
            </div>
          ) : (
            <div className="space-y-2">
              <Keyboard size={48} className="mx-auto text-gimbal-accent animate-pulse" />
              <p className="text-gimbal-text">Press any key combination...</p>
              <p className="text-sm text-gimbal-text-dim">Press Escape to cancel</p>
            </div>
          )}
        </div>
      </motion.div>
    </motion.div>
  );
}

interface ShortcutRowProps {
  actionId: ActionId;
  label: string;
  binding: ShortcutBinding;
  defaultBinding: ShortcutBinding;
  onEdit: () => void;
  onReset: () => void;
  isModified: boolean;
}

function ShortcutRow({ label, binding, onEdit, onReset, isModified }: ShortcutRowProps) {
  return (
    <div className="flex items-center justify-between py-2 px-3 rounded-lg hover:bg-gimbal-bg/50">
      <span className="text-sm text-gimbal-text">{label}</span>
      <div className="flex items-center gap-2">
        <button
          onClick={onEdit}
          className={`px-3 py-1.5 rounded-md border font-mono text-sm transition-colors ${
            isModified
              ? 'bg-gimbal-accent/20 border-gimbal-accent text-gimbal-accent'
              : 'bg-gimbal-bg border-gimbal-border text-gimbal-text-dim hover:border-gimbal-accent'
          }`}
        >
          {formatBinding(binding)}
        </button>
        {isModified && (
          <button
            onClick={onReset}
            className="p-1.5 text-gimbal-text-dim hover:text-gimbal-warning"
            title="Reset to default"
          >
            <RotateCcw size={14} />
          </button>
        )}
      </div>
    </div>
  );
}

export function ShortcutEditor() {
  const { bindings, setBinding, resetBinding, resetAllBindings } = useShortcutsStore();
  const [editingAction, setEditingAction] = useState<ActionId | null>(null);

  const categories = getActionsByCategory();

  const handleCapture = useCallback(
    (binding: ShortcutBinding) => {
      if (editingAction) {
        setBinding(editingAction, binding);
        setEditingAction(null);
      }
    },
    [editingAction, setBinding]
  );

  const isBindingModified = (actionId: ActionId): boolean => {
    const current = bindings[actionId];
    const def = ACTION_DEFINITIONS.find((a) => a.id === actionId)?.defaultBinding;
    if (!def) return false;

    if (current.key.toLowerCase() !== def.key.toLowerCase()) return true;

    const curMods = current.modifiers || {};
    const defMods = def.modifiers || {};

    return (
      !!curMods.shift !== !!defMods.shift ||
      !!curMods.ctrl !== !!defMods.ctrl ||
      !!curMods.alt !== !!defMods.alt ||
      !!curMods.meta !== !!defMods.meta
    );
  };

  const hasAnyModified = Object.keys(bindings).some((id) =>
    isBindingModified(id as ActionId)
  );

  const editingActionDef = editingAction
    ? ACTION_DEFINITIONS.find((a) => a.id === editingAction)
    : null;

  return (
    <div className="space-y-6">
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-2">
          <Keyboard size={20} className="text-gimbal-accent" />
          <h3 className="text-lg font-semibold text-gimbal-text">
            Keyboard Shortcuts
          </h3>
        </div>
        {hasAnyModified && (
          <button
            onClick={resetAllBindings}
            className="flex items-center gap-1.5 px-3 py-1.5 text-sm text-gimbal-warning hover:bg-gimbal-warning/10 rounded-lg"
          >
            <RotateCcw size={14} />
            Reset All
          </button>
        )}
      </div>

      <p className="text-sm text-gimbal-text-dim">
        Click on any shortcut to change it. Your custom bindings are saved automatically.
      </p>

      <div className="space-y-6">
        {Object.entries(categories).map(([category, actions]) => (
          <div key={category} className="space-y-2">
            <h4 className="text-xs font-semibold text-gimbal-text-dim uppercase tracking-wider">
              {category}
            </h4>
            <div className="bg-gimbal-panel rounded-lg border border-gimbal-border divide-y divide-gimbal-border">
              {actions.map((action) => (
                <ShortcutRow
                  key={action.id}
                  actionId={action.id}
                  label={action.label}
                  binding={bindings[action.id]}
                  defaultBinding={action.defaultBinding}
                  onEdit={() => setEditingAction(action.id)}
                  onReset={() => resetBinding(action.id)}
                  isModified={isBindingModified(action.id)}
                />
              ))}
            </div>
          </div>
        ))}
      </div>

      {editingAction && editingActionDef && (
        <KeyCaptureModal
          actionLabel={editingActionDef.label}
          onCapture={handleCapture}
          onCancel={() => setEditingAction(null)}
        />
      )}
    </div>
  );
}
