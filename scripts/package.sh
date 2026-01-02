#!/bin/bash
set -e

# Configuration
PORTABLE_DIR="PAZ_Gimbal_Portable"
APP_NAME="PAZ Gimbal Control.app"
DIST_ZIP="PAZ_Gimbal_Control_Mac.zip"

# Colors
GREEN='\033[0;32m'
NC='\033[0m'

echo "================================================"
echo "  Packaging $APP_NAME"
echo "================================================"

if [ ! -d "$PORTABLE_DIR" ]; then
    echo "Error: $PORTABLE_DIR not found. Run build_dist.sh first."
    exit 1
fi

# 1. Clean and Prepare App Bundle
echo ""
echo -e "${GREEN}[1/4] Preparing App Bundle structure...${NC}"
# We'll use the existing app as a template if possible, or create new
if [ -d "$APP_NAME" ]; then
    echo "Using existing app template..."
else
    echo "Creating new app bundle..."
    mkdir -p "$APP_NAME/Contents/MacOS"
    mkdir -p "$APP_NAME/Contents/Resources"
fi

# Ensure Resources/app exists and is clean
rm -rf "$APP_NAME/Contents/Resources/app"
mkdir -p "$APP_NAME/Contents/Resources/app"

# 2. Copy Portable Content to Resources
echo ""
echo -e "${GREEN}[2/4] Embedding Portable Content...${NC}"
cp -r "$PORTABLE_DIR/"* "$APP_NAME/Contents/Resources/app/"

# 3. Create Relative Launcher
echo ""
echo -e "${GREEN}[3/4] Updating Launcher Script...${NC}"
LAUNCHER="$APP_NAME/Contents/MacOS/PAZ Gimbal Control"

cat > "$LAUNCHER" << 'EOF'
#!/bin/bash
# PAZ Gimbal Control - Portable Launcher

# Get the directory where this script is located (Contents/MacOS)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Calculate path to the embedded app (Contents/Resources/app)
APP_DIR="$DIR/../Resources/app"
cd "$APP_DIR"

# Dependencies
# We use system python. If specific modules are missing, we try to install them silently to user scope 
# (not ideal for strict sandboxing but functional for "sharing scripts")
# Or we just assume they are there. Given the requirements are simple (aiohttp, python-socketio), 
# we'll try a quick check.

# Set PYTHONPATH to include our local libs if needed, though they are usually binaries.
# If you bunded python env, you'd point to it here.

# Start Server in background and capture PID
# We assume system python3 is available
echo "Starting internal server..."
python3 server/zt_bridge.py &
SERVER_PID=$!

# Wait a moment for server to start
sleep 2

# Open Browser
open "http://localhost:3001"

# Wait for server process (keeps the app "running" in dock somewhat)
# Or just exit and let the server run? 
# If we exit, the server might be orphaned or killed depending on launch context.
# Usually MacOS apps stay open.
wait $SERVER_PID
EOF

chmod +x "$LAUNCHER"

# 4. Zip the Application
echo ""
echo -e "${GREEN}[4/4] Zipping for Distribution...${NC}"
rm -f "$DIST_ZIP"
zip -r "$DIST_ZIP" "$APP_NAME" -x "*.DS_Store"

echo ""
echo "================================================"
echo -e "${GREEN}  Packaging Complete!${NC}"
echo "  Ready to share: $DIST_ZIP"
echo "================================================"
