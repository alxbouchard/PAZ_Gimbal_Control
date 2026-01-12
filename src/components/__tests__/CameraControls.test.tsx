import { describe, it, expect, vi, beforeEach } from 'vitest';
import { render, screen, fireEvent } from '@testing-library/react';
import { CameraControls } from '../CameraControls';
import { useAtemStore } from '../../store/atemStore';
import { useGimbalStore } from '../../store/gimbalStore';

// Mock the websocket service
vi.mock('../../services/websocket', () => ({
  gimbalSocket: {
    setAtemFocus: vi.fn(),
    setAtemAperture: vi.fn(),
    setAtemGain: vi.fn(),
    setAtemZoom: vi.fn(),
    triggerAtemAutoFocus: vi.fn(),
    triggerAtemAutoAperture: vi.fn(),
  },
}));

import { gimbalSocket } from '../../services/websocket';

describe('CameraControls', () => {
  beforeEach(() => {
    // Reset stores
    useGimbalStore.getState().setConnected(true);
    useGimbalStore.getState().setActiveGimbal('test-gimbal');
    useAtemStore.getState().setConfig({ ip: '192.168.10.240', connected: true, connecting: false });
    useAtemStore.getState().setGimbalMapping('test-gimbal', 1, 1); // MFT camera
    useAtemStore.getState().setCameraControl('focus', 50);
    useAtemStore.getState().setCameraControl('aperture', 50);
    useAtemStore.getState().setCameraControl('gain', 50);
    useAtemStore.getState().setCameraControl('zoom', 50);

    vi.clearAllMocks();
  });

  describe('Conditional rendering', () => {
    it('should render when ATEM is connected and gimbal has mapping', () => {
      render(<CameraControls />);
      expect(screen.getByText('Camera Control')).toBeInTheDocument();
    });

    it('should not render when ATEM is disconnected', () => {
      useAtemStore.getState().setConfig({ connected: false });

      const { container } = render(<CameraControls />);
      expect(container).toBeEmptyDOMElement();
    });

    it('should not render when gimbal has no ATEM mapping', () => {
      useAtemStore.getState().setMappings({});

      const { container } = render(<CameraControls />);
      expect(container).toBeEmptyDOMElement();
    });

    it('should not render when gimbal mapping port is 0', () => {
      useAtemStore.getState().setGimbalMapping('test-gimbal', 0, 1);

      const { container } = render(<CameraControls />);
      expect(container).toBeEmptyDOMElement();
    });

    it('should not render when no active gimbal', () => {
      useGimbalStore.getState().setActiveGimbal(null);

      const { container } = render(<CameraControls />);
      expect(container).toBeEmptyDOMElement();
    });
  });

  describe('Camera info display', () => {
    it('should show camera port number', () => {
      render(<CameraControls />);
      expect(screen.getByText('CAM 1')).toBeInTheDocument();
    });

    it('should show camera type', () => {
      render(<CameraControls />);
      expect(screen.getByText('MFT')).toBeInTheDocument();
    });

    it('should show EF type when configured', () => {
      useAtemStore.getState().setGimbalMapping('test-gimbal', 1, 0); // EF = 0

      render(<CameraControls />);
      expect(screen.getByText('EF')).toBeInTheDocument();
    });
  });

  describe('Camera sliders', () => {
    it('should render Focus slider', () => {
      render(<CameraControls />);
      expect(screen.getByText('Focus')).toBeInTheDocument();
    });

    it('should render Aperture slider', () => {
      render(<CameraControls />);
      expect(screen.getByText('Aperture')).toBeInTheDocument();
    });

    it('should render ISO/Gain slider', () => {
      render(<CameraControls />);
      expect(screen.getByText('ISO/Gain')).toBeInTheDocument();
    });

    it('should render Zoom slider', () => {
      render(<CameraControls />);
      expect(screen.getByText('Zoom')).toBeInTheDocument();
    });

    it('should show current value for sliders', () => {
      render(<CameraControls />);
      // All sliders start at 50%
      expect(screen.getAllByText('50%').length).toBeGreaterThan(0);
    });
  });

  describe('Focus control', () => {
    it('should call setAtemFocus on slider change', () => {
      render(<CameraControls />);

      const sliders = screen.getAllByRole('slider');
      // Focus slider should be first
      fireEvent.change(sliders[0], { target: { value: '75' } });

      expect(gimbalSocket.setAtemFocus).toHaveBeenCalledWith(1, 75, 'MFT');
    });

    it('should show AUTO button for focus', () => {
      render(<CameraControls />);
      const autoButtons = screen.getAllByText('AUTO');
      expect(autoButtons.length).toBeGreaterThan(0);
    });

    it('should call triggerAtemAutoFocus on AUTO button click', () => {
      render(<CameraControls />);

      const autoButtons = screen.getAllByText('AUTO');
      fireEvent.click(autoButtons[0]);

      expect(gimbalSocket.triggerAtemAutoFocus).toHaveBeenCalledWith(1);
    });
  });

  describe('Aperture control', () => {
    it('should call setAtemAperture on slider change', () => {
      render(<CameraControls />);

      const sliders = screen.getAllByRole('slider');
      // Aperture slider should be second
      fireEvent.change(sliders[1], { target: { value: '30' } });

      expect(gimbalSocket.setAtemAperture).toHaveBeenCalledWith(1, 30);
    });

    it('should call triggerAtemAutoAperture on AUTO button click', () => {
      render(<CameraControls />);

      const autoButtons = screen.getAllByText('AUTO');
      // Second AUTO button is for aperture
      fireEvent.click(autoButtons[1]);

      expect(gimbalSocket.triggerAtemAutoAperture).toHaveBeenCalledWith(1);
    });
  });

  describe('Gain control', () => {
    it('should call setAtemGain on slider change', () => {
      render(<CameraControls />);

      const sliders = screen.getAllByRole('slider');
      // Gain slider should be third
      fireEvent.change(sliders[2], { target: { value: '80' } });

      expect(gimbalSocket.setAtemGain).toHaveBeenCalledWith(1, 80);
    });

    it('should not have AUTO button for gain', () => {
      render(<CameraControls />);
      // Only Focus and Aperture have AUTO buttons
      const autoButtons = screen.getAllByText('AUTO');
      expect(autoButtons.length).toBe(2);
    });
  });

  describe('Zoom control', () => {
    it('should call setAtemZoom on slider change', () => {
      render(<CameraControls />);

      const sliders = screen.getAllByRole('slider');
      // Zoom slider should be fourth
      fireEvent.change(sliders[3], { target: { value: '75' } });

      expect(gimbalSocket.setAtemZoom).toHaveBeenCalledWith(1, 75);
    });

    it('should show zoom direction indicator', () => {
      render(<CameraControls />);
      // By default zoom is at center (50), so shows STOP
      expect(screen.getByText(/STOP/)).toBeInTheDocument();
    });
  });

  describe('Disabled state', () => {
    it('should disable sliders when disconnected from gimbal server', () => {
      useGimbalStore.getState().setConnected(false);

      render(<CameraControls />);

      const sliders = screen.getAllByRole('slider');
      sliders.forEach((slider) => {
        expect(slider).toBeDisabled();
      });
    });

    it('should disable AUTO buttons when disconnected', () => {
      useGimbalStore.getState().setConnected(false);

      render(<CameraControls />);

      const autoButtons = screen.getAllByText('AUTO');
      autoButtons.forEach((btn) => {
        expect(btn.closest('button')).toBeDisabled();
      });
    });
  });

  describe('Footer text', () => {
    it('should show SDI information', () => {
      render(<CameraControls />);
      expect(
        screen.getByText('Camera parameters are sent via SDI to the ATEM switcher')
      ).toBeInTheDocument();
    });
  });
});
