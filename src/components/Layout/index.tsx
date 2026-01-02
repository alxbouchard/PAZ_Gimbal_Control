import { useState, useRef, useEffect } from 'react';
import { motion } from 'framer-motion';
import { version } from '../../../package.json';
import {
  Gamepad2,
  Eye,
  BarChart3,
  Settings,
  Keyboard,
  Info,
  Wifi,
  WifiOff,
  Target,
  Zap,
  Monitor,
  Radio,
  ChevronDown,
  Check,
} from 'lucide-react';
import { useGimbalStore } from '../../store/gimbalStore';
import { gimbalSocket } from '../../services/websocket';
import type { TabId } from '../../types';

interface Tab {
  id: TabId;
  label: string;
  icon: React.ReactNode;
}

const tabs: Tab[] = [
  { id: 'control', label: 'Control', icon: <Gamepad2 size={18} /> },
  { id: 'visualizer', label: 'Visualizer', icon: <Eye size={18} /> },
  { id: 'dashboard', label: 'Dashboard', icon: <BarChart3 size={18} /> },
  { id: 'settings', label: 'Settings', icon: <Settings size={18} /> },
  { id: 'shortcuts', label: 'Shortcuts', icon: <Keyboard size={18} /> },
  { id: 'about', label: 'About', icon: <Info size={18} /> },
];

interface MainLayoutProps {
  children: (activeTab: TabId) => React.ReactNode;
}

export function MainLayout({ children }: MainLayoutProps) {
  const [activeTab, setActiveTab] = useState<TabId>('control');
  const [gimbalDropdownOpen, setGimbalDropdownOpen] = useState(false);
  const dropdownRef = useRef<HTMLDivElement>(null);
  const { connected, tracking, speedBoost, position, gimbalMode, availableGimbals, activeGimbalId } = useGimbalStore();

  // Get active gimbal info
  const activeGimbal = availableGimbals.find(g => g.id === activeGimbalId);

  // Close dropdown when clicking outside
  useEffect(() => {
    function handleClickOutside(event: MouseEvent) {
      if (dropdownRef.current && !dropdownRef.current.contains(event.target as Node)) {
        setGimbalDropdownOpen(false);
      }
    }
    document.addEventListener('mousedown', handleClickOutside);
    return () => document.removeEventListener('mousedown', handleClickOutside);
  }, []);

  const handleGimbalSelect = (gimbalId: string) => {
    gimbalSocket.selectGimbal(gimbalId);
    setGimbalDropdownOpen(false);
  };

  return (
    <div className="h-full flex flex-col bg-gimbal-bg">
      {/* Header */}
      <header
        className="flex-shrink-0 border-b border-gimbal-border bg-gimbal-panel"
        style={{ WebkitAppRegion: 'drag' } as any}
      >
        <div className="flex items-center justify-between px-4 py-3 pl-24">
          {/* Logo & Title */}
          <div className="flex items-center gap-3">
            <div className="w-8 h-8 rounded-lg bg-gradient-to-br from-gimbal-accent to-blue-600 flex items-center justify-center">
              <Target size={18} className="text-white" />
            </div>
            <div>
              <h1 className="text-sm font-semibold text-gimbal-text">
                PAZ Gimbal Control
              </h1>
              <p className="text-xs text-gimbal-text-dim">v{version}</p>
            </div>
          </div>

          {/* Status indicators */}
          <div
            className="flex items-center gap-4"
            style={{ WebkitAppRegion: 'no-drag' } as any}
          >
            {/* Position display */}
            <div className="hidden md:flex items-center gap-4 text-xs font-mono text-gimbal-text-dim">
              <span>P: {position.pitch.toFixed(1)}°</span>
              <span>Y: {position.yaw.toFixed(1)}°</span>
              <span>R: {position.roll.toFixed(1)}°</span>
            </div>

            {/* Status badges */}
            <div className="flex items-center gap-2">
              {/* Gimbal selector dropdown */}
              {connected && (
                <div className="relative" ref={dropdownRef}>
                  <button
                    onClick={() => setGimbalDropdownOpen(!gimbalDropdownOpen)}
                    className={`flex items-center gap-1 px-2 py-1 rounded-full text-xs cursor-pointer transition-colors ${gimbalMode === 'virtual'
                        ? 'bg-blue-500/20 text-blue-400 hover:bg-blue-500/30'
                        : 'bg-purple-500/20 text-purple-400 hover:bg-purple-500/30'
                      }`}
                  >
                    {gimbalMode === 'virtual' ? <Monitor size={12} /> : <Radio size={12} />}
                    <span className="max-w-[100px] truncate">
                      {activeGimbal?.name || (gimbalMode === 'virtual' ? 'Virtual' : 'Real Gimbal')}
                    </span>
                    <ChevronDown size={12} className={`transition-transform ${gimbalDropdownOpen ? 'rotate-180' : ''}`} />
                  </button>

                  {/* Dropdown menu */}
                  {gimbalDropdownOpen && (
                    <div className="absolute right-0 top-full mt-1 w-56 bg-gimbal-panel border border-gimbal-border rounded-lg shadow-lg z-50 overflow-hidden">
                      <div className="py-1">
                        <div className="px-3 py-2 text-xs text-gimbal-text-dim border-b border-gimbal-border">
                          Select Gimbal ({availableGimbals.length} available)
                        </div>
                        {availableGimbals.map((gimbal) => (
                          <button
                            key={gimbal.id}
                            onClick={() => handleGimbalSelect(gimbal.id)}
                            className={`w-full flex items-center gap-2 px-3 py-2 text-left text-sm transition-colors ${gimbal.id === activeGimbalId
                                ? 'bg-gimbal-accent/20 text-gimbal-accent'
                                : 'text-gimbal-text hover:bg-gimbal-border/50'
                              }`}
                          >
                            {gimbal.mode === 'virtual' ? (
                              <Monitor size={14} className="text-blue-400" />
                            ) : (
                              <Radio size={14} className="text-purple-400" />
                            )}
                            <div className="flex-1 min-w-0">
                              <div className="font-medium truncate">{gimbal.name}</div>
                              <div className="text-xs text-gimbal-text-dim truncate">
                                {gimbal.model} • {gimbal.ip}
                              </div>
                            </div>
                            {gimbal.id === activeGimbalId && (
                              <Check size={14} className="text-gimbal-accent" />
                            )}
                          </button>
                        ))}
                      </div>
                    </div>
                  )}
                </div>
              )}
              {speedBoost && (
                <div className="flex items-center gap-1 px-2 py-1 rounded-full bg-gimbal-warning/20 text-gimbal-warning text-xs">
                  <Zap size={12} />
                  <span>Boost</span>
                </div>
              )}
              {tracking && (
                <div className="flex items-center gap-1 px-2 py-1 rounded-full bg-gimbal-success/20 text-gimbal-success text-xs">
                  <Target size={12} />
                  <span>Tracking</span>
                </div>
              )}
              <div
                className={`flex items-center gap-1 px-2 py-1 rounded-full text-xs ${connected
                    ? 'bg-gimbal-success/20 text-gimbal-success'
                    : 'bg-gimbal-error/20 text-gimbal-error'
                  }`}
              >
                {connected ? <Wifi size={12} /> : <WifiOff size={12} />}
                <span>{connected ? 'Connected' : 'Disconnected'}</span>
              </div>
            </div>
          </div>
        </div>

        {/* Tabs */}
        <nav
          className="flex gap-1 px-4 pb-2"
          style={{ WebkitAppRegion: 'no-drag' } as any}
        >
          {tabs.map((tab) => (
            <button
              key={tab.id}
              onClick={() => setActiveTab(tab.id)}
              className={`relative flex items-center gap-2 px-4 py-2 rounded-lg text-sm font-medium transition-colors ${activeTab === tab.id
                  ? 'text-gimbal-accent bg-gimbal-accent/10'
                  : 'text-gimbal-text-dim hover:text-gimbal-text hover:bg-gimbal-border/50'
                }`}
            >
              {tab.icon}
              <span className="hidden sm:inline">{tab.label}</span>
              {activeTab === tab.id && (
                <motion.div
                  layoutId="activeTab"
                  className="absolute inset-0 rounded-lg border border-gimbal-accent/30"
                  transition={{ type: 'spring', bounce: 0.2, duration: 0.4 }}
                />
              )}
            </button>
          ))}
        </nav>
      </header>

      {/* Main content */}
      <main className="flex-1 overflow-auto p-4 md:p-6">{children(activeTab)}</main>

      {/* Footer status bar */}
      <footer className="flex-shrink-0 border-t border-gimbal-border bg-gimbal-panel px-4 py-2">
        <div className="flex items-center justify-between text-xs text-gimbal-text-dim">
          <div className="flex items-center gap-4">
            <span>
              Press <kbd className="px-1 py-0.5 bg-gimbal-border rounded text-gimbal-text">H</kbd> for Home
            </span>
            <span>
              Press <kbd className="px-1 py-0.5 bg-gimbal-border rounded text-gimbal-text">Space</kbd> to Stop
            </span>
          </div>
          <div className="flex items-center gap-2">
            <span className="w-2 h-2 rounded-full bg-gimbal-success animate-pulse" />
            <span>System Ready</span>
          </div>
        </div>
      </footer>
    </div>
  );
}
