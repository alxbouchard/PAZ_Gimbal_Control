#!/bin/bash
set -e

# Configuration
APP_NAME="PAZ_Gimbal_Portable"
DIST_DIR="$APP_NAME"

# Colors
GREEN='\033[0;32m'
NC='\033[0m'

echo "================================================"
echo "  Building $APP_NAME"
echo "================================================"

# 1. Build Frontend
echo ""
echo -e "${GREEN}[1/5] Building Frontend...${NC}"
npm install
npm run build

# 2. Prepare Directory Structure
echo ""
echo -e "${GREEN}[2/5] Creating Distribution Directory...${NC}"
rm -rf "$DIST_DIR"
mkdir -p "$DIST_DIR"
mkdir -p "$DIST_DIR/server"

# 3. Copy Backend & Libraries
echo ""
echo -e "${GREEN}[3/5] Copying Backend Files...${NC}"
# Copy server files
cp server/zt_bridge.py "$DIST_DIR/server/"
cp server/gimbals.json "$DIST_DIR/server/" 2>/dev/null || true
cp server/requirements.txt "$DIST_DIR/server/"

# Copy zt_bridge.py dependencies if any (none distinct from single file + libs)

# Copy Libraries (ZAP_Tracking, etc)
echo "Copying Libraries..."
cp -r libs "$DIST_DIR/"

# 4. Copy Compiled Frontend
echo ""
echo -e "${GREEN}[4/5] Copying Frontend...${NC}"
# Copy dist content directly to server/dist so python finds it easily relative to itself
cp -r dist "$DIST_DIR/server/dist"

# 5. Create Portable Launcher
echo ""
echo -e "${GREEN}[5/5] Creating Launcher Script...${NC}"
cat > "$DIST_DIR/Start_PAZ_Gimbal.sh" << 'EOF'
#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"

echo "Starting PAZ Gimbal Control..."

# Check Python dependencies
# We assume the user has python3. If you want venv, we could set one up here, 
# but for "portable" on user's machine, using system python is often simplest if libs are minimal.
# Ideally, we would check for packages.

echo "Checking dependencies..."
pip3 install -r server/requirements.txt > /dev/null 2>&1

echo "Opening Browser..."
# Open browser after a short delay
(sleep 2 && open "http://localhost:3001") &

echo "Starting Server..."
python3 server/zt_bridge.py
EOF

chmod +x "$DIST_DIR/Start_PAZ_Gimbal.sh"

echo ""
echo "================================================"
echo -e "${GREEN}  Build Complete!${NC}"
echo "  Distribution is ready in: ./$DIST_DIR"
echo "  To test, run: ./$DIST_DIR/Start_PAZ_Gimbal.sh"
echo "================================================"
