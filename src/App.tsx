import { useEffect } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import { MainLayout } from './components/Layout';
import { ControlPanel } from './components/ControlPanel';
import { CameraControls } from './components/CameraControls';
import { VirtualGimbal } from './components/VirtualGimbal';
import { Dashboard } from './components/Dashboard';
import { SettingsPage } from './components/pages/SettingsPage';
import { ShortcutsPage } from './components/pages/ShortcutsPage';
import { AboutPage } from './components/pages/AboutPage';
import { ConnectionModeSelector } from './components/ConnectionModeSelector';
import { useKeyboardControls } from './hooks/useKeyboardControls';
import { useGamepad } from './hooks/useGamepad';
import { gimbalSocket } from './services/websocket';
import { useGimbalStore } from './store/gimbalStore';
import type { TabId } from './types';

function AppContent({ activeTab }: { activeTab: TabId }) {
  const pageVariants = {
    initial: { opacity: 0, y: 20 },
    animate: { opacity: 1, y: 0 },
    exit: { opacity: 0, y: -20 },
  };

  return (
    <AnimatePresence mode="wait">
      <motion.div
        key={activeTab}
        variants={pageVariants}
        initial="initial"
        animate="animate"
        exit="exit"
        transition={{ duration: 0.2 }}
        className="h-full"
      >
        {activeTab === 'control' && (
          <div className="grid grid-cols-1 xl:grid-cols-2 gap-6 h-full">
            <div className="space-y-6">
              <ControlPanel />
              <CameraControls />
            </div>
            <VirtualGimbal />
          </div>
        )}

        {activeTab === 'visualizer' && (
          <div className="h-full">
            <VirtualGimbal />
          </div>
        )}

        {activeTab === 'dashboard' && <Dashboard />}

        {activeTab === 'settings' && <SettingsPage />}

        {activeTab === 'shortcuts' && <ShortcutsPage />}

        {activeTab === 'about' && <AboutPage />}
      </motion.div>
    </AnimatePresence>
  );
}

function MainApp() {
  const { setConnecting } = useGimbalStore();

  // Initialize keyboard controls
  useKeyboardControls();

  // Initialize gamepad support
  useGamepad();

  // Connect to WebSocket server on mount
  useEffect(() => {
    const connect = async () => {
      try {
        setConnecting(true);
        await gimbalSocket.connect();
      } catch (error) {
        console.error('Failed to connect to gimbal server:', error);
      }
    };

    connect();

    // Cleanup on unmount
    return () => {
      gimbalSocket.disconnect();
    };
  }, [setConnecting]);

  return (
    <MainLayout>
      {(activeTab) => <AppContent activeTab={activeTab} />}
    </MainLayout>
  );
}

function App() {
  const { connectionMode } = useGimbalStore();

  // Show mode selector if no mode selected
  if (!connectionMode) {
    return <ConnectionModeSelector />;
  }

  return <MainApp />;
}

export default App;
