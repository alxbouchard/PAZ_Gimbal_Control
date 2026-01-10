// Gimbal state types
export interface GimbalPosition {
  pitch: number;
  yaw: number;
  roll: number;
}

export interface GimbalSpeed {
  pitch?: number;
  yaw?: number;
  roll?: number;
}

export interface GimbalInfo {
  id: string;
  name: string;
  model: string;
  connected: boolean;
  ip?: string;
  mode?: 'virtual' | 'real';
  controlledBy?: string | null;  // Name of user controlling this gimbal
}

export type GimbalMode = 'virtual' | 'real' | 'unknown';

export interface TelemetryData {
  timestamp: number;
  position: GimbalPosition;
  speed: GimbalSpeed;
  temperature: number;
  batteryLevel: number;
}

export interface ControlMapping {
  joystickLeft: {
    x: 'yaw' | 'roll' | 'none';
    y: 'pitch' | 'none';
  };
  joystickRight: {
    x: 'yaw' | 'roll' | 'none';
    y: 'focus' | 'zoom' | 'none';
  };
  invertPitch: boolean;
  invertYaw: boolean;
  invertRoll: boolean;
}

// Store state types
export interface GimbalState {
  // Connection
  connected: boolean;
  connecting: boolean;
  serverUrl: string;
  connectionMode: ConnectionMode;
  clientIdentity: ClientIdentity | null;

  // Current gimbal
  activeGimbalId: string | null;
  availableGimbals: GimbalInfo[];
  gimbalMode: GimbalMode;

  // Position & movement
  position: GimbalPosition;
  speed: GimbalSpeed;

  // Control states
  controlling: boolean;
  tracking: boolean;
  speedBoost: boolean;

  // Camera controls
  zoom: number;
  focus: number;

  // Settings
  sensitivity: number;
  controlMapping: ControlMapping;

  // Telemetry history
  telemetryHistory: TelemetryData[];
}

// Store actions
export interface GimbalActions {
  // Connection
  setConnected: (connected: boolean) => void;
  setConnecting: (connecting: boolean) => void;
  setServerUrl: (url: string) => void;
  setConnectionMode: (mode: ConnectionMode) => void;
  setClientIdentity: (identity: ClientIdentity | null) => void;

  // Gimbal selection
  setActiveGimbal: (id: string | null) => void;
  setAvailableGimbals: (gimbals: GimbalInfo[]) => void;
  setGimbalMode: (mode: GimbalMode) => void;

  // Position & movement
  setPosition: (position: Partial<GimbalPosition>) => void;
  setSpeed: (speed: GimbalSpeed) => void;

  // Control states
  setControlling: (controlling: boolean) => void;
  setTracking: (tracking: boolean) => void;
  setSpeedBoost: (speedBoost: boolean) => void;

  // Camera controls
  setZoom: (zoom: number) => void;
  setFocus: (focus: number) => void;

  // Settings
  setSensitivity: (sensitivity: number) => void;
  setControlMapping: (mapping: Partial<ControlMapping>) => void;

  // Telemetry
  addTelemetryData: (data: TelemetryData) => void;
  clearTelemetryHistory: () => void;
}

export type GimbalStore = GimbalState & GimbalActions;

// WebSocket event types
export interface ServerToClientEvents {
  'gimbal:position': (position: GimbalPosition) => void;
  'gimbal:status': (status: { connected: boolean; tracking: boolean; speedBoost: boolean; mode?: 'virtual' | 'real' }) => void;
  'gimbal:telemetry': (data: TelemetryData) => void;
  'gimbal:list': (gimbals: GimbalInfo[]) => void;
  'gimbal:selected': (gimbalId: string) => void;
  'error': (message: string) => void;
}

export interface ClientToServerEvents {
  'gimbal:setSpeed': (speed: GimbalSpeed) => void;
  'gimbal:stopSpeed': () => void;
  'gimbal:setSpeedMultiplier': (value: number) => void;
  'gimbal:goHome': () => void;
  'gimbal:setHome': () => void;
  'gimbal:toggleTracking': (enabled: boolean) => void;
  'gimbal:toggleSpeedBoost': (enabled: boolean) => void;
  'gimbal:setZoom': (value: number) => void;
  'gimbal:setFocus': (value: number) => void;
  'gimbal:calibrateFocus': () => void;
  'gimbal:select': (gimbalId: string) => void;
  // Gimbal management
  'gimbal:add': (config: { name: string; ip: string }) => void;
  'gimbal:remove': (gimbalId: string) => void;
  'gimbal:update': (config: { id: string; name?: string; ip?: string }) => void;
  'gimbal:connect': (gimbalId: string) => void;
}

// Connection mode types
export type ConnectionMode = 'master' | 'client' | null;

export interface ClientIdentity {
  name: string;
  sid: string;
}

// Tab types
export type TabId = 'control' | 'visualizer' | 'dashboard' | 'settings' | 'shortcuts' | 'about';

export interface TabConfig {
  id: TabId;
  label: string;
  icon: string;
}
