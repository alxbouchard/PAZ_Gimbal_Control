import { useState } from 'react';
import {
  Radio,
  Monitor,
  Plus,
  Trash2,
  RefreshCw,
  Edit2,
  Check,
  X,
  Wifi,
  WifiOff,
  AlertCircle,
  Loader2,
} from 'lucide-react';
import { useGimbalStore } from '../../store/gimbalStore';
import { gimbalSocket } from '../../services/websocket';

interface AddGimbalFormData {
  name: string;
  ip: string;
}

export function GimbalManager() {
  const { availableGimbals, activeGimbalId } = useGimbalStore();
  const [isAdding, setIsAdding] = useState(false);
  const [editingId, setEditingId] = useState<string | null>(null);
  const [formData, setFormData] = useState<AddGimbalFormData>({ name: '', ip: '' });
  const [editData, setEditData] = useState<AddGimbalFormData>({ name: '', ip: '' });

  const handleAddGimbal = () => {
    if (!formData.ip.trim()) return;

    gimbalSocket.addGimbal({
      name: formData.name.trim() || `Gimbal ${formData.ip}`,
      ip: formData.ip.trim(),
    });

    setFormData({ name: '', ip: '' });
    setIsAdding(false);
  };

  const handleRemoveGimbal = (gimbalId: string) => {
    if (confirm('Are you sure you want to remove this gimbal?')) {
      gimbalSocket.removeGimbal(gimbalId);
    }
  };

  const handleEditGimbal = (gimbalId: string) => {
    const gimbal = availableGimbals.find(g => g.id === gimbalId);
    if (gimbal) {
      setEditData({ name: gimbal.name, ip: gimbal.ip || '' });
      setEditingId(gimbalId);
    }
  };

  const handleSaveEdit = () => {
    if (!editingId || !editData.ip.trim()) return;

    gimbalSocket.updateGimbal({
      id: editingId,
      name: editData.name.trim(),
      ip: editData.ip.trim(),
    });

    setEditingId(null);
  };

  const handleConnectGimbal = (gimbalId: string) => {
    gimbalSocket.connectGimbal(gimbalId);
  };

  const realGimbals = availableGimbals.filter(g => g.mode === 'real');
  const virtualGimbal = availableGimbals.find(g => g.mode === 'virtual');

  return (
    <div className="space-y-6">
      {/* Header */}
      <div className="flex items-center justify-between">
        <div>
          <h3 className="text-sm font-semibold text-gimbal-text">Gimbal Devices</h3>
          <p className="text-xs text-gimbal-text-dim mt-1">
            Add and manage your DJI gimbals via EthCAN
          </p>
        </div>
        <button
          onClick={() => setIsAdding(true)}
          className="flex items-center gap-2 px-3 py-2 bg-gimbal-accent text-white rounded-lg text-sm font-medium hover:bg-gimbal-accent-hover transition-colors"
        >
          <Plus size={16} />
          Add Gimbal
        </button>
      </div>

      {/* Add Gimbal Form */}
      {isAdding && (
        <div className="p-4 bg-gimbal-bg rounded-lg border-2 border-gimbal-accent/50">
          <h4 className="text-sm font-medium text-gimbal-text mb-4">Add New Gimbal</h4>
          <div className="grid grid-cols-2 gap-4">
            <div>
              <label className="block text-xs text-gimbal-text-dim mb-2">Name</label>
              <input
                type="text"
                value={formData.name}
                onChange={(e) => setFormData({ ...formData, name: e.target.value })}
                placeholder="e.g., Main Stage"
                className="w-full px-3 py-2 bg-gimbal-panel border border-gimbal-border rounded-lg text-sm text-gimbal-text focus:outline-none focus:border-gimbal-accent"
              />
            </div>
            <div>
              <label className="block text-xs text-gimbal-text-dim mb-2">
                IP Address <span className="text-gimbal-error">*</span>
              </label>
              <input
                type="text"
                value={formData.ip}
                onChange={(e) => setFormData({ ...formData, ip: e.target.value })}
                placeholder="192.168.0.100"
                className="w-full px-3 py-2 bg-gimbal-panel border border-gimbal-border rounded-lg text-sm text-gimbal-text focus:outline-none focus:border-gimbal-accent"
              />
            </div>
          </div>
          <div className="flex justify-end gap-2 mt-4">
            <button
              onClick={() => setIsAdding(false)}
              className="px-4 py-2 text-sm text-gimbal-text-dim hover:text-gimbal-text transition-colors"
            >
              Cancel
            </button>
            <button
              onClick={handleAddGimbal}
              disabled={!formData.ip.trim()}
              className="px-4 py-2 bg-gimbal-success text-white rounded-lg text-sm font-medium hover:bg-gimbal-success/80 transition-colors disabled:opacity-50 disabled:cursor-not-allowed"
            >
              Add Gimbal
            </button>
          </div>
        </div>
      )}

      {/* Virtual Gimbal */}
      {virtualGimbal && (
        <div className="p-4 bg-blue-500/10 rounded-lg border border-blue-500/30">
          <div className="flex items-center gap-3">
            <div className="p-2 bg-blue-500/20 rounded-lg">
              <Monitor size={20} className="text-blue-400" />
            </div>
            <div className="flex-1">
              <div className="flex items-center gap-2">
                <span className="text-sm font-medium text-blue-400">{virtualGimbal.name}</span>
                <span className="px-2 py-0.5 bg-blue-500/20 rounded text-[10px] text-blue-400">VIRTUAL</span>
              </div>
              <p className="text-xs text-gimbal-text-dim mt-1">
                Always available for testing and visualization
              </p>
            </div>
            <div className="flex items-center gap-2">
              <Wifi size={16} className="text-gimbal-success" />
              <span className="text-xs text-gimbal-success">Active</span>
            </div>
          </div>
        </div>
      )}

      {/* Real Gimbals List */}
      {realGimbals.length === 0 ? (
        <div className="p-8 bg-gimbal-bg rounded-lg border border-dashed border-gimbal-border text-center">
          <Radio size={32} className="text-gimbal-text-dim mx-auto mb-3" />
          <p className="text-sm text-gimbal-text-dim">No real gimbals configured</p>
          <p className="text-xs text-gimbal-text-dim mt-1">
            Click "Add Gimbal" to connect a DJI gimbal via EthCAN
          </p>
        </div>
      ) : (
        <div className="space-y-3">
          {realGimbals.map((gimbal) => {
            const isEditing = editingId === gimbal.id;
            const isActive = activeGimbalId === gimbal.id;
            const isConnecting = (gimbal as any).connecting;

            return (
              <div
                key={gimbal.id}
                className={`p-4 rounded-lg border transition-colors ${
                  isActive
                    ? 'bg-purple-500/10 border-purple-500/50'
                    : 'bg-gimbal-bg border-gimbal-border hover:border-gimbal-accent/30'
                }`}
              >
                {isEditing ? (
                  // Edit mode
                  <div className="space-y-3">
                    <div className="grid grid-cols-2 gap-3">
                      <input
                        type="text"
                        value={editData.name}
                        onChange={(e) => setEditData({ ...editData, name: e.target.value })}
                        placeholder="Gimbal name"
                        className="px-3 py-2 bg-gimbal-panel border border-gimbal-border rounded-lg text-sm text-gimbal-text focus:outline-none focus:border-gimbal-accent"
                      />
                      <input
                        type="text"
                        value={editData.ip}
                        onChange={(e) => setEditData({ ...editData, ip: e.target.value })}
                        placeholder="IP address"
                        className="px-3 py-2 bg-gimbal-panel border border-gimbal-border rounded-lg text-sm text-gimbal-text focus:outline-none focus:border-gimbal-accent"
                      />
                    </div>
                    <div className="flex justify-end gap-2">
                      <button
                        onClick={() => setEditingId(null)}
                        className="p-2 text-gimbal-text-dim hover:text-gimbal-text transition-colors"
                      >
                        <X size={16} />
                      </button>
                      <button
                        onClick={handleSaveEdit}
                        className="p-2 text-gimbal-success hover:bg-gimbal-success/20 rounded transition-colors"
                      >
                        <Check size={16} />
                      </button>
                    </div>
                  </div>
                ) : (
                  // View mode
                  <div className="flex items-center gap-3">
                    <div className={`p-2 rounded-lg ${gimbal.connected ? 'bg-purple-500/20' : 'bg-gimbal-border'}`}>
                      <Radio size={20} className={gimbal.connected ? 'text-purple-400' : 'text-gimbal-text-dim'} />
                    </div>

                    <div className="flex-1 min-w-0">
                      <div className="flex items-center gap-2">
                        <span className="text-sm font-medium text-gimbal-text truncate">
                          {gimbal.name}
                        </span>
                        {isActive && (
                          <span className="px-2 py-0.5 bg-purple-500/20 rounded text-[10px] text-purple-400">
                            ACTIVE
                          </span>
                        )}
                      </div>
                      <div className="flex items-center gap-3 mt-1">
                        <span className="text-xs text-gimbal-text-dim">{gimbal.ip}</span>
                        <span className="text-xs text-gimbal-text-dim">•</span>
                        <span className="text-xs text-gimbal-text-dim">{gimbal.model}</span>
                      </div>
                    </div>

                    {/* Connection status */}
                    <div className="flex items-center gap-2">
                      {isConnecting ? (
                        <>
                          <Loader2 size={16} className="text-gimbal-warning animate-spin" />
                          <span className="text-xs text-gimbal-warning">Connecting...</span>
                        </>
                      ) : gimbal.connected ? (
                        <>
                          <Wifi size={16} className="text-gimbal-success" />
                          <span className="text-xs text-gimbal-success">Connected</span>
                        </>
                      ) : (
                        <>
                          <WifiOff size={16} className="text-gimbal-error" />
                          <span className="text-xs text-gimbal-error">Offline</span>
                        </>
                      )}
                    </div>

                    {/* Actions */}
                    <div className="flex items-center gap-1">
                      {!gimbal.connected && !isConnecting && (
                        <button
                          onClick={() => handleConnectGimbal(gimbal.id)}
                          className="p-2 text-gimbal-text-dim hover:text-gimbal-accent hover:bg-gimbal-accent/10 rounded-lg transition-colors"
                          title="Connect"
                        >
                          <RefreshCw size={16} />
                        </button>
                      )}
                      <button
                        onClick={() => handleEditGimbal(gimbal.id)}
                        className="p-2 text-gimbal-text-dim hover:text-gimbal-text hover:bg-gimbal-border/50 rounded-lg transition-colors"
                        title="Edit"
                      >
                        <Edit2 size={16} />
                      </button>
                      <button
                        onClick={() => handleRemoveGimbal(gimbal.id)}
                        className="p-2 text-gimbal-text-dim hover:text-gimbal-error hover:bg-gimbal-error/10 rounded-lg transition-colors"
                        title="Remove"
                      >
                        <Trash2 size={16} />
                      </button>
                    </div>
                  </div>
                )}
              </div>
            );
          })}
        </div>
      )}

      {/* Help text */}
      <div className="p-4 bg-gimbal-bg rounded-lg">
        <div className="flex items-start gap-3">
          <AlertCircle size={16} className="text-gimbal-text-dim mt-0.5" />
          <div className="text-xs text-gimbal-text-dim space-y-1">
            <p>
              <strong>How to connect:</strong> Each DJI gimbal requires an EthCAN adapter connected to your network.
            </p>
            <p>
              Enter the IP address of the EthCAN adapter (e.g., 192.168.0.100). The gimbal will be detected automatically.
            </p>
          </div>
        </div>
      </div>
    </div>
  );
}
