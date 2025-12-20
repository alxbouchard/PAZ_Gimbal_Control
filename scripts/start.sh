#!/bin/bash
# PAZ Gimbal Control - Start Script

cd "/Users/alexmacbookair/Documents/Code/PAZ_Gimbal_Control"

# Cleanup function
cleanup() {
    echo ''
    echo '🛑 Shutting down PAZ Gimbal Control...'
    pkill -f 'zt_bridge.py' 2>/dev/null
    pkill -f 'vite preview' 2>/dev/null
    echo '✅ Servers stopped.'
    exit 0
}

# Trap signals
trap cleanup INT TERM

# Kill existing processes
pkill -f 'zt_bridge.py' 2>/dev/null
pkill -f 'vite preview' 2>/dev/null
sleep 1

echo ''
echo '🚀 Starting PAZ Gimbal Control v1.2.0...'
echo ''

# Start Python server in background
echo '📡 Starting gimbal server...'
python3 server/zt_bridge.py --virtual &
sleep 2

# Start web server in background
echo '🌐 Starting web server...'
npm run preview > /dev/null 2>&1 &
sleep 3

# Open browser
echo '🔗 Opening browser...'
open http://localhost:4173

echo ''
echo '═══════════════════════════════════════════════════'
echo '  ✅ PAZ Gimbal Control is running!'
echo '  🌐 Web UI: http://localhost:4173'
echo '  📡 Server: http://localhost:3001'
echo ''
echo '  Press Ctrl+C to stop'
echo '═══════════════════════════════════════════════════'
echo ''

# Keep alive - wait for Ctrl+C
while true; do
    sleep 1
done
