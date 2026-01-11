import { useState } from 'react';
import { Camera, Wifi, WifiOff, Video, Settings2 } from 'lucide-react';
import { useAtemStore } from '../store/atemStore';
import { useGimbalStore } from '../store/gimbalStore';
import { gimbalSocket } from '../services/websocket';
import type { AtemCameraType } from '../types';
import { cameraTypeToString } from '../types';

export function AtemSettings() {
  const { config, mappings } = useAtemStore();
  const { availableGimbals } = useGimbalStore();
  const [ipInput, setIpInput] = useState(config.ip || '');

  const handleConnect = () => {
    if (ipInput.trim()) {
      gimbalSocket.connectAtem(ipInput.trim());
    }
  };

  const handleDisconnect = () => {
    gimbalSocket.disconnectAtem();
  };

  const handleMappingChange = (gimbalId: string, port: number, cameraType: AtemCameraType) => {
    gimbalSocket.setAtemGimbalMapping(gimbalId, port, cameraType);
  };

  // Filter out virtual gimbal for ATEM mapping
  const realGimbals = availableGimbals.filter(g => g.mode !== 'virtual');

  return (
    <div className="space-y-4">
      <h3 className="text-sm font-semibold text-gimbal-text flex items-center gap-2">
        <Video size={16} />
        ATEM Switcher
      </h3>
      <p className="text-xs text-gimbal-text-dim">
        Connect to a Blackmagic ATEM switcher to control camera parameters (focus, zoom, aperture, ISO) via SDI camera control.
      </p>

      {/* Connection Status */}
      <div className="flex items-center gap-2 p-3 bg-gimbal-bg rounded-lg border border-gimbal-border">
        {config.connected ? (
          <>
            <Wifi size={16} className="text-green-500" />
            <span className="text-sm text-green-400">Connected to {config.ip}</span>
          </>
        ) : config.connecting ? (
          <>
            <Wifi size={16} className="text-yellow-500 animate-pulse" />
            <span className="text-sm text-yellow-400">Connecting...</span>
          </>
        ) : (
          <>
            <WifiOff size={16} className="text-gimbal-text-dim" />
            <span className="text-sm text-gimbal-text-dim">Not connected</span>
          </>
        )}
      </div>

      {/* Connection Form */}
      <div className="space-y-3">
        <div>
          <label className="block text-xs text-gimbal-text-dim mb-2">
            ATEM Switcher IP Address
          </label>
          <div className="flex gap-2">
            <input
              type="text"
              value={ipInput}
              onChange={(e) => setIpInput(e.target.value)}
              placeholder="192.168.10.240"
              disabled={config.connecting}
              className="flex-1 px-4 py-2 bg-gimbal-bg border border-gimbal-border rounded-lg text-gimbal-text text-sm focus:outline-none focus:border-gimbal-accent disabled:opacity-50"
            />
            {config.connected ? (
              <button
                onClick={handleDisconnect}
                className="px-4 py-2 bg-red-500/20 text-red-400 border border-red-500/30 rounded-lg text-sm hover:bg-red-500/30 transition-colors"
              >
                Disconnect
              </button>
            ) : (
              <button
                onClick={handleConnect}
                disabled={!ipInput.trim() || config.connecting}
                className="px-4 py-2 bg-gimbal-accent text-white rounded-lg text-sm hover:bg-gimbal-accent/80 transition-colors disabled:opacity-50 disabled:cursor-not-allowed"
              >
                Connect
              </button>
            )}
          </div>
          <p className="text-xs text-gimbal-text-dim mt-2">
            The IP address of your Blackmagic ATEM switcher
          </p>
        </div>
      </div>

      {/* Gimbal to Camera Mapping */}
      {realGimbals.length > 0 && (
        <div className="space-y-3 pt-4 border-t border-gimbal-border">
          <h4 className="text-sm font-medium text-gimbal-text flex items-center gap-2">
            <Settings2 size={14} />
            Gimbal to Camera Mapping
          </h4>
          <p className="text-xs text-gimbal-text-dim">
            Associate each gimbal with an ATEM camera input. When controlling a gimbal, camera functions (focus, zoom, aperture, ISO) will be sent to the associated camera.
          </p>

          <div className="space-y-2">
            {realGimbals.map((gimbal) => {
              const mapping = mappings[gimbal.id];
              const currentPort = mapping?.port || 0;
              const currentCameraType: AtemCameraType = cameraTypeToString(mapping?.cameraType);

              return (
                <div
                  key={gimbal.id}
                  className="flex items-center gap-3 p-3 bg-gimbal-bg rounded-lg border border-gimbal-border"
                >
                  <Camera size={16} className="text-gimbal-text-dim" />
                  <span className="text-sm text-gimbal-text flex-1 truncate">
                    {gimbal.name}
                  </span>

                  {/* Camera Port Selector */}
                  <select
                    value={currentPort}
                    onChange={(e) => {
                      const port = parseInt(e.target.value, 10);
                      handleMappingChange(gimbal.id, port, currentCameraType);
                    }}
                    className="px-3 py-1.5 bg-gimbal-panel border border-gimbal-border rounded text-sm text-gimbal-text focus:outline-none focus:border-gimbal-accent"
                  >
                    <option value={0}>No Camera</option>
                    <option value={1}>Camera 1</option>
                    <option value={2}>Camera 2</option>
                    <option value={3}>Camera 3</option>
                    <option value={4}>Camera 4</option>
                    <option value={5}>Camera 5</option>
                    <option value={6}>Camera 6</option>
                    <option value={7}>Camera 7</option>
                    <option value={8}>Camera 8</option>
                  </select>

                  {/* Camera Type Selector (only if port is selected) */}
                  {currentPort > 0 && (
                    <select
                      value={currentCameraType}
                      onChange={(e) => {
                        handleMappingChange(gimbal.id, currentPort, e.target.value as AtemCameraType);
                      }}
                      className="px-3 py-1.5 bg-gimbal-panel border border-gimbal-border rounded text-sm text-gimbal-text focus:outline-none focus:border-gimbal-accent"
                    >
                      <option value="MFT">MFT Lens</option>
                      <option value="EF">EF Lens</option>
                    </select>
                  )}
                </div>
              );
            })}
          </div>

          <p className="text-xs text-gimbal-text-dim mt-2">
            <strong>MFT</strong>: Micro Four Thirds (absolute focus control)
            <br />
            <strong>EF</strong>: Canon EF mount (offset-based focus control)
          </p>
        </div>
      )}

      {realGimbals.length === 0 && (
        <div className="text-center py-4 text-gimbal-text-dim text-sm">
          Add gimbals in the Gimbal Devices section above to configure ATEM camera mappings.
        </div>
      )}
    </div>
  );
}
