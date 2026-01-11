import { io, Socket } from 'socket.io-client';
import { useGimbalStore } from '../store/gimbalStore';
import { useAtemStore } from '../store/atemStore';
import type { GimbalSpeed, ServerToClientEvents, ClientToServerEvents, ClientIdentity, AtemConfig, AtemMappings, AtemCameraType } from '../types';

// Extended events to include client identity and ATEM
interface ExtendedServerToClientEvents extends ServerToClientEvents {
  'client:identity': (identity: ClientIdentity) => void;
  'atem:status': (config: AtemConfig) => void;
  'atem:mappings': (mappings: AtemMappings) => void;
  'atem:error': (message: string) => void;
}

type TypedSocket = Socket<ExtendedServerToClientEvents, ClientToServerEvents>;

class GimbalSocketService {
  private socket: TypedSocket | null = null;
  private reconnectAttempts = 0;
  private maxReconnectAttempts = 5;

  async connect(): Promise<void> {
    const store = useGimbalStore.getState();

    return new Promise((resolve, reject) => {
      try {
        this.socket = io(store.serverUrl, {
          transports: ['websocket'],
          timeout: 5000,
          reconnection: true,
          reconnectionAttempts: this.maxReconnectAttempts,
          reconnectionDelay: 1000,
        }) as TypedSocket;

        this.socket.on('connect', () => {
          console.log('Connected to gimbal server');
          this.reconnectAttempts = 0;
          store.setConnected(true);
          store.setConnecting(false);
          resolve();
        });

        this.socket.on('disconnect', (reason) => {
          console.log('Disconnected from gimbal server:', reason);
          store.setConnected(false);
        });

        this.socket.on('connect_error', (error) => {
          console.error('Connection error:', error);
          this.reconnectAttempts++;
          if (this.reconnectAttempts >= this.maxReconnectAttempts) {
            store.setConnecting(false);
            reject(error);
          }
        });

        // Listen for gimbal events
        this.socket.on('gimbal:position', (position) => {
          store.setPosition(position);
        });

        this.socket.on('gimbal:status', (status) => {
          store.setConnected(status.connected);
          store.setTracking(status.tracking);
          store.setSpeedBoost(status.speedBoost);
          if (status.mode) {
            store.setGimbalMode(status.mode);
          }
        });

        this.socket.on('gimbal:telemetry', (data) => {
          store.addTelemetryData(data);
          store.setPosition(data.position);
        });

        this.socket.on('gimbal:list', (gimbals) => {
          store.setAvailableGimbals(gimbals);
          if (gimbals.length > 0 && !store.activeGimbalId) {
            store.setActiveGimbal(gimbals[0].id);
          }
        });

        this.socket.on('gimbal:selected', (gimbalId) => {
          store.setActiveGimbal(gimbalId);
        });

        this.socket.on('error', (message) => {
          console.error('Gimbal error:', message);
        });

        // Listen for client identity
        this.socket.on('client:identity', (identity) => {
          console.log('Client identity:', identity);
          store.setClientIdentity(identity);
        });

        // Listen for ATEM events
        const atemStore = useAtemStore.getState();

        this.socket.on('atem:status', (config) => {
          atemStore.setConfig(config);
        });

        this.socket.on('atem:mappings', (mappings) => {
          atemStore.setMappings(mappings);
        });

        this.socket.on('atem:error', (message) => {
          console.error('ATEM error:', message);
        });

      } catch (error) {
        store.setConnecting(false);
        reject(error);
      }
    });
  }

  disconnect(): void {
    if (this.socket) {
      this.socket.disconnect();
      this.socket = null;
      useGimbalStore.getState().setConnected(false);
    }
  }

  isConnected(): boolean {
    return this.socket?.connected ?? false;
  }

  // Movement commands
  setSpeed(speed: GimbalSpeed): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:setSpeed', speed);
      useGimbalStore.getState().setSpeed(speed);
    }
  }

  stopSpeed(): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:stopSpeed');
      useGimbalStore.getState().setSpeed({ pitch: 0, yaw: 0, roll: 0 });
    }
  }

  // Speed multiplier (affects all automated movements)
  setSpeedMultiplier(value: number): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:setSpeedMultiplier', value);
    }
  }

  // Position commands
  goHome(): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:goHome');
    }
  }

  setHome(): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:setHome');
    }
  }

  // Mode toggles
  toggleTracking(enabled: boolean): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:toggleTracking', enabled);
      useGimbalStore.getState().setTracking(enabled);
    }
  }

  toggleSpeedBoost(enabled: boolean): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:toggleSpeedBoost', enabled);
      useGimbalStore.getState().setSpeedBoost(enabled);
    }
  }

  // Camera controls
  setZoom(value: number): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:setZoom', value);
      useGimbalStore.getState().setZoom(value);
    }
  }

  setFocus(value: number): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:setFocus', value);
      useGimbalStore.getState().setFocus(value);
    }
  }

  calibrateFocus(): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:calibrateFocus');
    }
  }

  // Gimbal selection
  selectGimbal(gimbalId: string): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:select', gimbalId);
    }
  }

  // Gimbal management
  addGimbal(config: { name: string; ip: string }): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:add', config);
    }
  }

  removeGimbal(gimbalId: string): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:remove', gimbalId);
    }
  }

  updateGimbal(config: { id: string; name?: string; ip?: string }): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:update', config);
    }
  }

  connectGimbal(gimbalId: string): void {
    if (this.socket?.connected) {
      this.socket.emit('gimbal:connect', gimbalId);
    }
  }

  // ============== ATEM Controls ==============

  connectAtem(ip: string): void {
    if (this.socket?.connected) {
      this.socket.emit('atem:connect' as any, { ip });
    }
  }

  disconnectAtem(): void {
    if (this.socket?.connected) {
      this.socket.emit('atem:disconnect' as any);
    }
  }

  setAtemGimbalMapping(gimbalId: string, port: number, cameraType: AtemCameraType): void {
    if (this.socket?.connected) {
      // Convert camera type to server format (0 = EF, 1 = MFT)
      const cameraTypeNum = cameraType === 'EF' ? 0 : 1;
      this.socket.emit('atem:setGimbalMapping' as any, { gimbalId, port, cameraType: cameraTypeNum });
    }
  }

  setAtemFocus(port: number, value: number, cameraType: AtemCameraType = 'MFT'): void {
    if (this.socket?.connected) {
      const cameraTypeNum = cameraType === 'EF' ? 0 : 1;
      this.socket.emit('atem:setFocus' as any, { port, value, cameraType: cameraTypeNum });
      useAtemStore.getState().setCameraControl('focus', value);
    }
  }

  triggerAtemAutoFocus(port: number): void {
    if (this.socket?.connected) {
      this.socket.emit('atem:autoFocus' as any, { port });
    }
  }

  setAtemAperture(port: number, value: number): void {
    if (this.socket?.connected) {
      this.socket.emit('atem:setAperture' as any, { port, value });
      useAtemStore.getState().setCameraControl('aperture', value);
    }
  }

  triggerAtemAutoAperture(port: number): void {
    if (this.socket?.connected) {
      this.socket.emit('atem:autoAperture' as any, { port });
    }
  }

  setAtemGain(port: number, value: number): void {
    if (this.socket?.connected) {
      this.socket.emit('atem:setGain' as any, { port, value });
      useAtemStore.getState().setCameraControl('gain', value);
    }
  }

  setAtemZoom(port: number, value: number): void {
    if (this.socket?.connected) {
      this.socket.emit('atem:setZoom' as any, { port, value });
      useAtemStore.getState().setCameraControl('zoom', value);
    }
  }

  setAtemZoomPosition(port: number, value: number): void {
    if (this.socket?.connected) {
      this.socket.emit('atem:setZoomPosition' as any, { port, value });
      useAtemStore.getState().setCameraControl('zoom', value);
    }
  }
}

export const gimbalSocket = new GimbalSocketService();
