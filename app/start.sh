#!/bin/bash
# PAZ Gimbal Control - Startup Script

cd "$(dirname "$0")/.."

# Kill any existing instances
pkill -f "zt_bridge.py" 2>/dev/null
pkill -f "vite preview" 2>/dev/null

# Build if dist doesn't exist
if [ ! -d "dist" ]; then
    echo "Building application..."
    npm run build
fi

# Start the server
python3 server/zt_bridge.py --virtual &
SERVER_PID=$!

# Wait for server to start
sleep 2

# Start the preview server
npm run preview &
PREVIEW_PID=$!

# Wait for preview server
sleep 2

# Open in browser
open http://localhost:4173

# Wait for both processes
wait $SERVER_PID $PREVIEW_PID
