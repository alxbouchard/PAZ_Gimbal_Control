import { useState } from 'react';
import { motion } from 'framer-motion';
import { Save, Play, Trash2, MapPin } from 'lucide-react';
import { usePresetsStore } from '../store/presetsStore';
import { useGimbalStore } from '../store/gimbalStore';
import { gimbalSocket } from '../services/websocket';

const PRESET_COUNT = 9; // Presets 1-9

export function PresetControls() {
  const { activeGimbalId, connected } = useGimbalStore();
  const { getPresetsForGimbal } = usePresetsStore();
  const [editMode, setEditMode] = useState(false);
  const [longPressTimer, setLongPressTimer] = useState<NodeJS.Timeout | null>(null);

  const presets = activeGimbalId ? getPresetsForGimbal(activeGimbalId) : {};

  const handlePresetClick = (presetNum: number) => {
    if (!activeGimbalId || !connected) return;

    if (editMode) {
      // In edit mode, clicking saves the preset
      gimbalSocket.savePreset(activeGimbalId, presetNum);
    } else {
      // Normal mode, clicking recalls the preset (if it exists)
      if (presets[String(presetNum)]) {
        gimbalSocket.recallPreset(activeGimbalId, presetNum);
      }
    }
  };

  const handlePresetLongPressStart = (presetNum: number) => {
    // Long press to save preset (alternative to edit mode)
    const timer = setTimeout(() => {
      if (activeGimbalId && connected) {
        gimbalSocket.savePreset(activeGimbalId, presetNum);
      }
    }, 800);
    setLongPressTimer(timer);
  };

  const handlePresetLongPressEnd = () => {
    if (longPressTimer) {
      clearTimeout(longPressTimer);
      setLongPressTimer(null);
    }
  };

  const handleDeletePreset = (presetNum: number, e: React.MouseEvent) => {
    e.stopPropagation();
    if (activeGimbalId && connected) {
      gimbalSocket.deletePreset(activeGimbalId, presetNum);
    }
  };

  return (
    <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-4 space-y-3">
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-2">
          <MapPin size={16} className="text-gimbal-accent" />
          <h3 className="text-sm font-semibold text-gimbal-text">
            Position Presets
          </h3>
        </div>
        <motion.button
          onClick={() => setEditMode(!editMode)}
          className={`px-2 py-1 text-xs font-medium rounded transition-colors ${
            editMode
              ? 'bg-yellow-500/20 text-yellow-400 border border-yellow-500/30'
              : 'bg-gimbal-bg text-gimbal-text-dim hover:text-gimbal-text'
          }`}
          whileHover={{ scale: 1.02 }}
          whileTap={{ scale: 0.98 }}
        >
          {editMode ? 'SAVE MODE' : 'Edit'}
        </motion.button>
      </div>

      <div className="grid grid-cols-3 gap-2">
        {Array.from({ length: PRESET_COUNT }, (_, i) => i + 1).map((presetNum) => {
          const hasPreset = !!presets[String(presetNum)];
          const preset = presets[String(presetNum)];

          return (
            <motion.button
              key={presetNum}
              onClick={() => handlePresetClick(presetNum)}
              onMouseDown={() => !editMode && handlePresetLongPressStart(presetNum)}
              onMouseUp={handlePresetLongPressEnd}
              onMouseLeave={handlePresetLongPressEnd}
              onTouchStart={() => !editMode && handlePresetLongPressStart(presetNum)}
              onTouchEnd={handlePresetLongPressEnd}
              disabled={!connected || (!editMode && !hasPreset)}
              className={`relative p-3 rounded-lg border transition-all ${
                hasPreset
                  ? 'bg-gimbal-accent/10 border-gimbal-accent/30 hover:border-gimbal-accent'
                  : editMode
                  ? 'bg-yellow-500/5 border-yellow-500/20 hover:border-yellow-500/50'
                  : 'bg-gimbal-bg border-gimbal-border opacity-50'
              } ${!connected ? 'opacity-50 cursor-not-allowed' : ''}`}
              whileHover={connected ? { scale: 1.02 } : {}}
              whileTap={connected ? { scale: 0.98 } : {}}
            >
              <div className="flex flex-col items-center gap-1">
                <span className={`text-lg font-bold ${
                  hasPreset ? 'text-gimbal-accent' : editMode ? 'text-yellow-400' : 'text-gimbal-text-dim'
                }`}>
                  {presetNum}
                </span>
                {hasPreset ? (
                  <div className="flex items-center gap-1">
                    <Play size={10} className="text-gimbal-accent" />
                    <span className="text-[9px] text-gimbal-text-dim">
                      {preset?.pitch?.toFixed(0)}° / {preset?.yaw?.toFixed(0)}°
                    </span>
                  </div>
                ) : editMode ? (
                  <div className="flex items-center gap-1">
                    <Save size={10} className="text-yellow-400" />
                    <span className="text-[9px] text-yellow-400">Click to save</span>
                  </div>
                ) : (
                  <span className="text-[9px] text-gimbal-text-dim">Empty</span>
                )}
              </div>

              {/* Delete button (shown only in edit mode for existing presets) */}
              {editMode && hasPreset && (
                <motion.button
                  onClick={(e) => handleDeletePreset(presetNum, e)}
                  className="absolute -top-1 -right-1 w-5 h-5 bg-red-500/80 rounded-full flex items-center justify-center hover:bg-red-500"
                  whileHover={{ scale: 1.1 }}
                  whileTap={{ scale: 0.9 }}
                >
                  <Trash2 size={10} className="text-white" />
                </motion.button>
              )}
            </motion.button>
          );
        })}
      </div>

      <p className="text-[10px] text-gimbal-text-dim text-center">
        {editMode
          ? 'Click a slot to save current position'
          : 'Click to recall • Long press to save • Use Shift+1-9 as shortcuts'}
      </p>
    </div>
  );
}
