import { motion } from 'framer-motion';
import { Camera, Focus, Aperture, Sun, ZoomIn, CircleDot } from 'lucide-react';
import { useAtemStore } from '../store/atemStore';
import { useGimbalStore } from '../store/gimbalStore';
import { gimbalSocket } from '../services/websocket';
import type { AtemCameraType } from '../types';
import { cameraTypeToString } from '../types';

interface CameraSliderProps {
  label: string;
  value: number;
  onChange: (value: number) => void;
  onAuto?: () => void;
  icon: React.ReactNode;
  disabled?: boolean;
  showAutoButton?: boolean;
}

function CameraSlider({ label, value, onChange, onAuto, icon, disabled, showAutoButton }: CameraSliderProps) {
  return (
    <div className="flex flex-col gap-2">
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-2">
          <span className="text-gimbal-accent">{icon}</span>
          <span className="text-xs font-medium text-gimbal-text-dim uppercase tracking-wider">
            {label}
          </span>
        </div>
        <div className="flex items-center gap-2">
          <span className="text-xs font-mono text-gimbal-accent">{value.toFixed(0)}%</span>
          {showAutoButton && onAuto && (
            <motion.button
              onClick={onAuto}
              disabled={disabled}
              className="px-2 py-0.5 text-[10px] font-medium bg-gimbal-accent/20 text-gimbal-accent rounded hover:bg-gimbal-accent/30 disabled:opacity-50 disabled:cursor-not-allowed"
              whileHover={{ scale: disabled ? 1 : 1.05 }}
              whileTap={{ scale: disabled ? 1 : 0.95 }}
            >
              AUTO
            </motion.button>
          )}
        </div>
      </div>
      <input
        type="range"
        min="0"
        max="100"
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
    </div>
  );
}

export function CameraControls() {
  const { config, mappings, cameraControls } = useAtemStore();
  const { activeGimbalId, connected } = useGimbalStore();

  // Get the ATEM port for the active gimbal
  const activeMapping = activeGimbalId ? mappings[activeGimbalId] : null;
  const activePort = activeMapping?.port || 0;
  const cameraType: AtemCameraType = cameraTypeToString(activeMapping?.cameraType);

  // Check if ATEM is connected and gimbal has a camera mapping
  const isAtemActive = config.connected && activePort > 0;

  if (!isAtemActive) {
    return null;
  }

  const handleFocusChange = (value: number) => {
    gimbalSocket.setAtemFocus(activePort, value, cameraType);
  };

  const handleApertureChange = (value: number) => {
    gimbalSocket.setAtemAperture(activePort, value);
  };

  const handleGainChange = (value: number) => {
    gimbalSocket.setAtemGain(activePort, value);
  };

  const handleZoomChange = (value: number) => {
    // For continuous zoom: <50 = zoom out, 50 = stop, >50 = zoom in
    gimbalSocket.setAtemZoom(activePort, value);
  };

  const handleAutoFocus = () => {
    gimbalSocket.triggerAtemAutoFocus(activePort);
  };

  const handleAutoAperture = () => {
    gimbalSocket.triggerAtemAutoAperture(activePort);
  };

  return (
    <div className="bg-gimbal-panel rounded-xl border border-gimbal-border p-4 space-y-4">
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-2">
          <Camera size={16} className="text-gimbal-accent" />
          <h3 className="text-sm font-semibold text-gimbal-text">
            Camera Control
          </h3>
        </div>
        <div className="flex items-center gap-2">
          <CircleDot size={10} className="text-green-500" />
          <span className="text-xs text-gimbal-text-dim">
            CAM {activePort}
          </span>
          <span className="text-[10px] px-1.5 py-0.5 bg-gimbal-bg rounded text-gimbal-text-dim">
            {cameraType}
          </span>
        </div>
      </div>

      <div className="grid grid-cols-1 sm:grid-cols-2 gap-4">
        <CameraSlider
          label="Focus"
          value={cameraControls.focus}
          onChange={handleFocusChange}
          onAuto={handleAutoFocus}
          icon={<Focus size={14} />}
          disabled={!connected}
          showAutoButton
        />

        <CameraSlider
          label="Aperture"
          value={cameraControls.aperture}
          onChange={handleApertureChange}
          onAuto={handleAutoAperture}
          icon={<Aperture size={14} />}
          disabled={!connected}
          showAutoButton
        />

        <CameraSlider
          label="ISO/Gain"
          value={cameraControls.gain}
          onChange={handleGainChange}
          icon={<Sun size={14} />}
          disabled={!connected}
        />

        <CameraSlider
          label="Zoom"
          value={cameraControls.zoom}
          onChange={handleZoomChange}
          icon={<ZoomIn size={14} />}
          disabled={!connected}
        />
      </div>

      <p className="text-[10px] text-gimbal-text-dim text-center">
        Camera parameters are sent via SDI to the ATEM switcher
      </p>
    </div>
  );
}
