import { useState } from 'react';
import { motion } from 'framer-motion';
import { useGimbalStore } from '../store/gimbalStore';
import type { ConnectionMode } from '../types';

export function ConnectionModeSelector() {
  const { setConnectionMode, setServerUrl } = useGimbalStore();
  const [selectedMode, setSelectedMode] = useState<ConnectionMode>(null);
  const [serverIp, setServerIp] = useState('');
  const [error, setError] = useState('');
  const [isStartingServer, setIsStartingServer] = useState(false);

  const isElectron = !!window.electronAPI;

  const handleMasterMode = async () => {
    // If running in Electron, start the Python server
    if (window.electronAPI) {
      setIsStartingServer(true);
      setError('');
      try {
        const result = await window.electronAPI.startServer();
        if (!result.success) {
          setError('Failed to start server. Make sure Python is installed.');
          setIsStartingServer(false);
          return;
        }
      } catch (err) {
        setError('Failed to start server: ' + (err instanceof Error ? err.message : 'Unknown error'));
        setIsStartingServer(false);
        return;
      }
      setIsStartingServer(false);
    }

    setServerUrl('http://localhost:3001');
    setConnectionMode('master');
  };

  const handleClientMode = () => {
    setSelectedMode('client');
  };

  const handleConnectToServer = () => {
    const ip = serverIp.trim();
    if (!ip) {
      setError('Please enter the server IP address');
      return;
    }

    // Validate IP format (basic check)
    const ipRegex = /^(\d{1,3}\.){3}\d{1,3}$/;
    const hostnameRegex = /^[a-zA-Z0-9][a-zA-Z0-9.-]*[a-zA-Z0-9]$/;

    if (!ipRegex.test(ip) && !hostnameRegex.test(ip) && ip !== 'localhost') {
      setError('Please enter a valid IP address (e.g., 192.168.1.100)');
      return;
    }

    setServerUrl(`http://${ip}:3001`);
    setConnectionMode('client');
  };

  const handleBack = () => {
    setSelectedMode(null);
    setError('');
  };

  return (
    <div className="min-h-screen bg-gimbal-bg flex items-center justify-center p-8">
      <motion.div
        initial={{ opacity: 0, scale: 0.95 }}
        animate={{ opacity: 1, scale: 1 }}
        className="max-w-2xl w-full"
      >
        {/* Logo/Title */}
        <div className="text-center mb-12">
          <h1 className="text-4xl font-bold text-white mb-2">PAZ Gimbal Control</h1>
          <p className="text-gimbal-muted">Multi-User Gimbal Control System</p>
        </div>

        {/* Error message */}
        {error && selectedMode === null && (
          <div className="mb-6 p-4 bg-red-500/10 border border-red-500/30 rounded-lg">
            <p className="text-red-400 text-sm">{error}</p>
          </div>
        )}

        {selectedMode === null ? (
          /* Mode Selection */
          <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
            {/* Master Mode */}
            <motion.button
              whileHover={!isStartingServer ? { scale: 1.02 } : {}}
              whileTap={!isStartingServer ? { scale: 0.98 } : {}}
              onClick={handleMasterMode}
              disabled={isStartingServer}
              className={`bg-gimbal-card border border-gimbal-border rounded-2xl p-8 text-left transition-all group ${
                isStartingServer ? 'opacity-75 cursor-wait' : 'hover:border-gimbal-accent'
              } ${!isElectron ? 'opacity-50' : ''}`}
            >
              <div className="w-16 h-16 bg-gimbal-accent/20 rounded-xl flex items-center justify-center mb-6 group-hover:bg-gimbal-accent/30 transition-colors">
                {isStartingServer ? (
                  <svg className="w-8 h-8 text-gimbal-accent animate-spin" fill="none" viewBox="0 0 24 24">
                    <circle className="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" strokeWidth="4"></circle>
                    <path className="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
                  </svg>
                ) : (
                  <svg className="w-8 h-8 text-gimbal-accent" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M5 12h14M5 12a2 2 0 01-2-2V6a2 2 0 012-2h14a2 2 0 012 2v4a2 2 0 01-2 2M5 12a2 2 0 00-2 2v4a2 2 0 002 2h14a2 2 0 002-2v-4a2 2 0 00-2-2m-2-4h.01M17 16h.01" />
                  </svg>
                )}
              </div>
              <h2 className="text-xl font-semibold text-white mb-2">Master</h2>
              <p className="text-gimbal-muted text-sm leading-relaxed">
                {isElectron
                  ? 'Launch the server and control gimbals from this computer. Other users can connect to you.'
                  : 'Master mode requires the desktop app. Use the terminal to start the server.'}
              </p>
              <div className="mt-4 flex items-center text-gimbal-accent text-sm font-medium">
                <span>{isStartingServer ? 'Starting server...' : 'Start as host'}</span>
                {!isStartingServer && (
                  <svg className="w-4 h-4 ml-2" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M9 5l7 7-7 7" />
                  </svg>
                )}
              </div>
            </motion.button>

            {/* Client Mode */}
            <motion.button
              whileHover={{ scale: 1.02 }}
              whileTap={{ scale: 0.98 }}
              onClick={handleClientMode}
              disabled={isStartingServer}
              className="bg-gimbal-card border border-gimbal-border rounded-2xl p-8 text-left hover:border-green-500 transition-all group"
            >
              <div className="w-16 h-16 bg-green-500/20 rounded-xl flex items-center justify-center mb-6 group-hover:bg-green-500/30 transition-colors">
                <svg className="w-8 h-8 text-green-500" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                  <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M8.111 16.404a5.5 5.5 0 017.778 0M12 20h.01m-7.08-7.071c3.904-3.905 10.236-3.905 14.14 0M1.394 9.393c5.857-5.857 15.355-5.857 21.213 0" />
                </svg>
              </div>
              <h2 className="text-xl font-semibold text-white mb-2">Client</h2>
              <p className="text-gimbal-muted text-sm leading-relaxed">
                Connect to an existing server running on another computer on your network.
              </p>
              <div className="mt-4 flex items-center text-green-500 text-sm font-medium">
                <span>Connect to host</span>
                <svg className="w-4 h-4 ml-2" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                  <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M9 5l7 7-7 7" />
                </svg>
              </div>
            </motion.button>
          </div>
        ) : (
          /* Client IP Entry */
          <motion.div
            initial={{ opacity: 0, x: 20 }}
            animate={{ opacity: 1, x: 0 }}
            className="bg-gimbal-card border border-gimbal-border rounded-2xl p-8"
          >
            <button
              onClick={handleBack}
              className="flex items-center text-gimbal-muted hover:text-white mb-6 transition-colors"
            >
              <svg className="w-4 h-4 mr-2" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M15 19l-7-7 7-7" />
              </svg>
              Back
            </button>

            <div className="w-16 h-16 bg-green-500/20 rounded-xl flex items-center justify-center mb-6">
              <svg className="w-8 h-8 text-green-500" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M8.111 16.404a5.5 5.5 0 017.778 0M12 20h.01m-7.08-7.071c3.904-3.905 10.236-3.905 14.14 0M1.394 9.393c5.857-5.857 15.355-5.857 21.213 0" />
              </svg>
            </div>

            <h2 className="text-xl font-semibold text-white mb-2">Connect to Server</h2>
            <p className="text-gimbal-muted text-sm mb-6">
              Enter the IP address of the computer running PAZ Gimbal Control as Master.
            </p>

            <div className="space-y-4">
              <div>
                <label className="block text-sm font-medium text-gimbal-text mb-2">
                  Server IP Address
                </label>
                <input
                  type="text"
                  value={serverIp}
                  onChange={(e) => {
                    setServerIp(e.target.value);
                    setError('');
                  }}
                  onKeyDown={(e) => {
                    if (e.key === 'Enter') handleConnectToServer();
                  }}
                  placeholder="192.168.1.100"
                  className="w-full px-4 py-3 bg-gimbal-bg border border-gimbal-border rounded-lg text-white placeholder-gimbal-muted focus:outline-none focus:border-green-500 transition-colors"
                  autoFocus
                />
                {error && (
                  <p className="mt-2 text-red-400 text-sm">{error}</p>
                )}
              </div>

              <button
                onClick={handleConnectToServer}
                className="w-full py-3 bg-green-500 hover:bg-green-600 text-white font-medium rounded-lg transition-colors"
              >
                Connect
              </button>

              <p className="text-gimbal-muted text-xs text-center">
                The Master computer's IP can be found in its network settings or by running <code className="bg-gimbal-bg px-1 rounded">ipconfig</code> (Windows) or <code className="bg-gimbal-bg px-1 rounded">ifconfig</code> (Mac/Linux)
              </p>
            </div>
          </motion.div>
        )}

        {/* Version */}
        <p className="text-center text-gimbal-muted text-xs mt-8">
          v1.4.0
        </p>
      </motion.div>
    </div>
  );
}
