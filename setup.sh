#!/bin/bash
set -e

echo "================================================"
echo "  PAZ Gimbal Control - Installation Script"
echo "================================================"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check OS
OS="$(uname -s)"
echo "Detected OS: $OS"

# Check for required tools
check_command() {
    if ! command -v $1 &> /dev/null; then
        echo -e "${RED}Error: $1 is not installed${NC}"
        exit 1
    fi
    echo -e "${GREEN}✓${NC} $1 found"
}

echo ""
echo "Checking required tools..."
check_command node
check_command npm
check_command python3

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# Step 1: Install Node.js dependencies
echo ""
echo -e "${YELLOW}[1/4] Installing Node.js dependencies...${NC}"
npm install

# Step 2: Install Python dependencies
echo ""
echo -e "${YELLOW}[2/4] Installing Python dependencies...${NC}"
pip3 install -r server/requirements.txt

# Step 3: Build native libraries (KMSbase + ZAP_Tracking)
echo ""
echo -e "${YELLOW}[3/4] Building native libraries...${NC}"

# Build KMSbase
echo "Building KMSbase..."
cd libs/KMSbase
if [ -f "Build.sh" ]; then
    chmod +x Build.sh
    ./Build.sh || echo -e "${YELLOW}Warning: KMSbase build had issues (may already be built)${NC}"
fi
cd "$SCRIPT_DIR"

# Build ZAP_Tracking
echo "Building ZAP_Tracking..."
cd libs/ZAP_Tracking

# Create symlink to KMSbase if needed
if [ ! -d "modules/KMSbase" ]; then
    mkdir -p modules
    ln -sf "$SCRIPT_DIR/libs/KMSbase" modules/KMSbase
fi

# Import dependencies
if [ -f "Import.sh" ]; then
    chmod +x Import.sh
    ./Import.sh || echo -e "${YELLOW}Warning: Import had issues${NC}"
fi

# Build
if [ -f "Build.sh" ]; then
    chmod +x Build.sh
    ./Build.sh || echo -e "${YELLOW}Warning: ZAP_Tracking build had issues (may already be built)${NC}"
fi
cd "$SCRIPT_DIR"

# Step 4: Verify installation
echo ""
echo -e "${YELLOW}[4/4] Verifying installation...${NC}"

# Check if binaries exist
if [ -f "libs/ZAP_Tracking/Binaries/ZT_Agent" ]; then
    echo -e "${GREEN}✓${NC} ZT_Agent binary found"
else
    echo -e "${YELLOW}!${NC} ZT_Agent binary not found (will run in virtual mode)"
fi

echo ""
echo "================================================"
echo -e "${GREEN}  Installation complete!${NC}"
echo "================================================"
echo ""
echo "To start the application:"
echo "  npm start"
echo ""
echo "Or run in development mode:"
echo "  npm run dev"
echo ""
echo "Virtual mode (no hardware):"
echo "  npm run virtual"
echo ""
