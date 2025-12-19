import express from 'express';
import { createServer } from 'http';
import { Server } from 'socket.io';
import cors from 'cors';

const app = express();
const httpServer = createServer(app);
const io = new Server(httpServer, {
  cors: {
    origin: ['http://localhost:3000', 'http://localhost:5173'],
    methods: ['GET', 'POST'],
  },
});

app.use(cors());
app.use(express.json());

// Virtual gimbal state
let gimbalState = {
  position: { pitch: 0, yaw: 0, roll: 0 },
  speed: { pitch: 0, yaw: 0, roll: 0 },
  tracking: false,
  speedBoost: false,
  zoom: 50,
  focus: 50,
  home: { pitch: 0, yaw: 0, roll: 0 },
};

// Available gimbals (simulated)
const availableGimbals = [
  { id: 'gimbal-1', name: 'Main Camera', model: 'DJI RS 3 Pro', connected: true, ip: '192.168.0.200' },
  { id: 'gimbal-2', name: 'Secondary', model: 'DJI RS 2', connected: false, ip: '192.168.0.201' },
];

let activeGimbalId = 'gimbal-1';

// Simulation loop
const TICK_RATE = 60;
const TICK_INTERVAL = 1000 / TICK_RATE;

// Telemetry counter for regular updates
let telemetryCounter = 0;
const TELEMETRY_INTERVAL = 30; // Send telemetry every 30 ticks (~500ms at 60fps)

function updateGimbal() {
  telemetryCounter++;
  const speedMultiplier = gimbalState.speedBoost ? 2 : 1;

  // Apply speed to position
  if (gimbalState.speed.pitch !== 0) {
    gimbalState.position.pitch += gimbalState.speed.pitch * speedMultiplier * 0.5;
    gimbalState.position.pitch = Math.max(-90, Math.min(90, gimbalState.position.pitch));
  }

  if (gimbalState.speed.yaw !== 0) {
    gimbalState.position.yaw += gimbalState.speed.yaw * speedMultiplier * 0.5;
  }
  // Always normalize yaw to -180 to 180
  gimbalState.position.yaw = ((gimbalState.position.yaw % 360) + 540) % 360 - 180;

  if (gimbalState.speed.roll !== 0) {
    gimbalState.position.roll += gimbalState.speed.roll * speedMultiplier * 0.3;
    gimbalState.position.roll = Math.max(-45, Math.min(45, gimbalState.position.roll));
  }

  // Broadcast position to all clients
  io.emit('gimbal:position', gimbalState.position);

  // Send telemetry at regular intervals (~500ms)
  if (telemetryCounter >= TELEMETRY_INTERVAL) {
    telemetryCounter = 0;
    io.emit('gimbal:telemetry', {
      timestamp: Date.now(),
      position: { ...gimbalState.position },
      speed: { ...gimbalState.speed },
      temperature: 35 + Math.random() * 10,
      batteryLevel: 85 - Math.random() * 5,
    });
  }
}

// Start simulation loop
setInterval(updateGimbal, TICK_INTERVAL);

// Socket.IO connection handling
io.on('connection', (socket) => {
  console.log('Client connected:', socket.id);

  // Send initial state
  socket.emit('gimbal:list', availableGimbals);
  socket.emit('gimbal:selected', activeGimbalId);
  socket.emit('gimbal:position', gimbalState.position);
  socket.emit('gimbal:status', {
    connected: true,
    tracking: gimbalState.tracking,
    speedBoost: gimbalState.speedBoost,
  });

  // Handle gimbal commands
  socket.on('gimbal:setSpeed', (speed) => {
    gimbalState.speed = { ...gimbalState.speed, ...speed };
  });

  socket.on('gimbal:stopSpeed', () => {
    gimbalState.speed = { pitch: 0, yaw: 0, roll: 0 };
    console.log('Emergency stop');
  });

  socket.on('gimbal:goHome', () => {
    // Animate back to home position
    const animateHome = () => {
      const tolerance = 0.5;
      let done = true;

      ['pitch', 'yaw', 'roll'].forEach((axis) => {
        const diff = gimbalState.home[axis] - gimbalState.position[axis];
        if (Math.abs(diff) > tolerance) {
          gimbalState.position[axis] += diff * 0.1;
          done = false;
        } else {
          gimbalState.position[axis] = gimbalState.home[axis];
        }
      });

      if (!done) {
        setTimeout(animateHome, TICK_INTERVAL);
      }
    };

    animateHome();
    console.log('Going home');
  });

  socket.on('gimbal:setHome', () => {
    gimbalState.home = { ...gimbalState.position };
    console.log('Home position set:', gimbalState.home);
  });

  socket.on('gimbal:toggleTracking', (enabled) => {
    gimbalState.tracking = enabled;
    io.emit('gimbal:status', {
      connected: true,
      tracking: gimbalState.tracking,
      speedBoost: gimbalState.speedBoost,
    });
    console.log('Tracking:', enabled);
  });

  socket.on('gimbal:toggleSpeedBoost', (enabled) => {
    gimbalState.speedBoost = enabled;
    io.emit('gimbal:status', {
      connected: true,
      tracking: gimbalState.tracking,
      speedBoost: gimbalState.speedBoost,
    });
    console.log('Speed boost:', enabled);
  });

  socket.on('gimbal:setZoom', (value) => {
    gimbalState.zoom = value;
    console.log('Zoom:', value);
  });

  socket.on('gimbal:setFocus', (value) => {
    gimbalState.focus = value;
    console.log('Focus:', value);
  });

  socket.on('gimbal:calibrateFocus', () => {
    gimbalState.focus = 50;
    console.log('Auto focus calibrated');
  });

  socket.on('gimbal:select', (gimbalId) => {
    activeGimbalId = gimbalId;
    io.emit('gimbal:selected', gimbalId);
    console.log('Selected gimbal:', gimbalId);
  });

  socket.on('disconnect', () => {
    console.log('Client disconnected:', socket.id);
  });
});

// REST API endpoints
app.get('/api/status', (req, res) => {
  res.json({
    gimbal: gimbalState,
    activeGimbalId,
    availableGimbals,
  });
});

app.get('/api/gimbals', (req, res) => {
  res.json(availableGimbals);
});

// Health check
app.get('/health', (req, res) => {
  res.json({ status: 'ok', timestamp: Date.now() });
});

const PORT = process.env.PORT || 3001;

httpServer.listen(PORT, () => {
  console.log(`
  ╔════════════════════════════════════════════╗
  ║     ZAP Gimbal Control Server              ║
  ║     Running on http://localhost:${PORT}       ║
  ╚════════════════════════════════════════════╝
  `);
});
