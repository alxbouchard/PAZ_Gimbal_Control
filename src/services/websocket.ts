import { io, Socket } from 'socket.io-client';
import { useGimbalStore } from '../store/gimbalStore';
import type { GimbalSpeed, ServerToClientEvents, ClientToServerEvents } from '../types';

type TypedSocket = Socket<ServerToClientEvents, ClientToServerEvents>;

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
}

export const gimbalSocket = new GimbalSocketService();
