import { useState } from 'react';
import { motion } from 'framer-motion';
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
} from 'lucide-react';
import { useGimbalStore } from '../../store/gimbalStore';
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
  const { connected, tracking, speedBoost, position } = useGimbalStore();

  return (
    <div className="h-full flex flex-col bg-gimbal-bg">
      {/* Header */}
      <header className="flex-shrink-0 border-b border-gimbal-border bg-gimbal-panel">
        <div className="flex items-center justify-between px-4 py-3">
          {/* Logo & Title */}
          <div className="flex items-center gap-3">
            <div className="w-8 h-8 rounded-lg bg-gradient-to-br from-gimbal-accent to-blue-600 flex items-center justify-center">
              <Target size={18} className="text-white" />
            </div>
            <div>
              <h1 className="text-sm font-semibold text-gimbal-text">
                PAZ Gimbal Control
              </h1>
              <p className="text-xs text-gimbal-text-dim">v1.0.0</p>
            </div>
          </div>

          {/* Status indicators */}
          <div className="flex items-center gap-4">
            {/* Position display */}
            <div className="hidden md:flex items-center gap-4 text-xs font-mono text-gimbal-text-dim">
              <span>P: {position.pitch.toFixed(1)}°</span>
              <span>Y: {position.yaw.toFixed(1)}°</span>
              <span>R: {position.roll.toFixed(1)}°</span>
            </div>

            {/* Status badges */}
            <div className="flex items-center gap-2">
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
                className={`flex items-center gap-1 px-2 py-1 rounded-full text-xs ${
                  connected
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
        <nav className="flex gap-1 px-4 pb-2">
          {tabs.map((tab) => (
            <button
              key={tab.id}
              onClick={() => setActiveTab(tab.id)}
              className={`relative flex items-center gap-2 px-4 py-2 rounded-lg text-sm font-medium transition-colors ${
                activeTab === tab.id
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
