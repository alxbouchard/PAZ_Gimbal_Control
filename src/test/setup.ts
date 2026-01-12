import '@testing-library/jest-dom';
import { vi, afterEach } from 'vitest';

// Mock window.matchMedia
Object.defineProperty(window, 'matchMedia', {
  writable: true,
  value: vi.fn().mockImplementation(query => ({
    matches: false,
    media: query,
    onchange: null,
    addListener: vi.fn(),
    removeListener: vi.fn(),
    addEventListener: vi.fn(),
    removeEventListener: vi.fn(),
    dispatchEvent: vi.fn(),
  })),
});

// Mock ResizeObserver
class ResizeObserverMock {
  observe = vi.fn();
  unobserve = vi.fn();
  disconnect = vi.fn();
}
window.ResizeObserver = ResizeObserverMock;

// Mock requestAnimationFrame
let rafId = 0;
window.requestAnimationFrame = vi.fn((callback) => {
  rafId++;
  setTimeout(callback, 16);
  return rafId;
}) as unknown as typeof window.requestAnimationFrame;
window.cancelAnimationFrame = vi.fn((id) => {
  clearTimeout(id);
}) as unknown as typeof window.cancelAnimationFrame;

// Mock navigator.getGamepads
Object.defineProperty(navigator, 'getGamepads', {
  writable: true,
  value: vi.fn().mockReturnValue([null, null, null, null]),
});

// Mock WebSocket/Socket.io
vi.mock('socket.io-client', () => ({
  io: vi.fn(() => ({
    on: vi.fn(),
    emit: vi.fn(),
    disconnect: vi.fn(),
    connected: false,
  })),
}));

// Mock electronAPI
Object.defineProperty(window, 'electronAPI', {
  writable: true,
  value: {
    startServer: vi.fn().mockResolvedValue({ success: true }),
    isElectron: vi.fn().mockReturnValue(false),
    saveConfig: vi.fn().mockResolvedValue({ success: true }),
    loadConfig: vi.fn().mockResolvedValue({ success: false }),
    getConfigPath: vi.fn().mockResolvedValue('/mock/path'),
  },
});

// Reset all mocks after each test
afterEach(() => {
  vi.clearAllMocks();
});
