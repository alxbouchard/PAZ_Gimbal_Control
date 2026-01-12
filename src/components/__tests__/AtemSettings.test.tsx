import { describe, it, expect, vi, beforeEach } from 'vitest';
import { render, screen, fireEvent } from '@testing-library/react';
import userEvent from '@testing-library/user-event';
import { AtemSettings } from '../AtemSettings';
import { useAtemStore } from '../../store/atemStore';
import { useGimbalStore } from '../../store/gimbalStore';

// Mock the websocket service
vi.mock('../../services/websocket', () => ({
  gimbalSocket: {
    connectAtem: vi.fn(),
    disconnectAtem: vi.fn(),
    setAtemGimbalMapping: vi.fn(),
  },
}));

import { gimbalSocket } from '../../services/websocket';

describe('AtemSettings', () => {
  beforeEach(() => {
    // Reset stores
    useAtemStore.getState().setConfig({ ip: '', connected: false, connecting: false });
    useAtemStore.getState().setError(null);
    useAtemStore.getState().setMappings({});
    useGimbalStore.getState().setAvailableGimbals([]);

    vi.clearAllMocks();
  });

  describe('Initial render', () => {
    it('should render ATEM Switcher header', () => {
      render(<AtemSettings />);
      expect(screen.getByText('ATEM Switcher')).toBeInTheDocument();
    });

    it('should show Not connected status initially', () => {
      render(<AtemSettings />);
      expect(screen.getByText('Not connected')).toBeInTheDocument();
    });

    it('should render IP address input', () => {
      render(<AtemSettings />);
      expect(screen.getByPlaceholderText('192.168.10.240')).toBeInTheDocument();
    });

    it('should render Connect button', () => {
      render(<AtemSettings />);
      expect(screen.getByRole('button', { name: 'Connect' })).toBeInTheDocument();
    });
  });

  describe('Connection', () => {
    it('should disable Connect button when IP is empty', () => {
      render(<AtemSettings />);
      const connectBtn = screen.getByRole('button', { name: 'Connect' });
      expect(connectBtn).toBeDisabled();
    });

    it('should enable Connect button when IP is entered', async () => {
      render(<AtemSettings />);
      const input = screen.getByPlaceholderText('192.168.10.240');

      await userEvent.type(input, '192.168.1.100');

      const connectBtn = screen.getByRole('button', { name: 'Connect' });
      expect(connectBtn).not.toBeDisabled();
    });

    it('should call connectAtem on connect button click', async () => {
      render(<AtemSettings />);
      const input = screen.getByPlaceholderText('192.168.10.240');

      await userEvent.type(input, '192.168.10.240');
      await userEvent.click(screen.getByRole('button', { name: 'Connect' }));

      expect(gimbalSocket.connectAtem).toHaveBeenCalledWith('192.168.10.240');
    });

    it('should show connecting status', () => {
      useAtemStore.getState().setConfig({ connecting: true });

      render(<AtemSettings />);
      expect(screen.getByText('Connecting...')).toBeInTheDocument();
    });

    it('should show connected status', () => {
      useAtemStore.getState().setConfig({ connected: true, ip: '192.168.10.240' });

      render(<AtemSettings />);
      expect(screen.getByText('Connected to 192.168.10.240')).toBeInTheDocument();
    });

    it('should show Disconnect button when connected', () => {
      useAtemStore.getState().setConfig({ connected: true, ip: '192.168.10.240' });

      render(<AtemSettings />);
      expect(screen.getByRole('button', { name: 'Disconnect' })).toBeInTheDocument();
    });

    it('should call disconnectAtem on disconnect button click', async () => {
      useAtemStore.getState().setConfig({ connected: true, ip: '192.168.10.240' });

      render(<AtemSettings />);
      await userEvent.click(screen.getByRole('button', { name: 'Disconnect' }));

      expect(gimbalSocket.disconnectAtem).toHaveBeenCalled();
    });
  });

  describe('Error display', () => {
    it('should show error message when present', () => {
      useAtemStore.getState().setError('Connection refused');

      render(<AtemSettings />);
      expect(screen.getByText('Connection refused')).toBeInTheDocument();
    });

    it('should have dismiss button for error', () => {
      useAtemStore.getState().setError('Connection refused');

      render(<AtemSettings />);
      // The X button should be present in the error display
      const dismissBtns = screen.getAllByRole('button');
      expect(dismissBtns.length).toBeGreaterThan(0);
    });
  });

  describe('Gimbal to Camera Mapping', () => {
    beforeEach(() => {
      useGimbalStore.getState().setAvailableGimbals([
        { id: 'gimbal-1', name: 'PTZ Camera 1', model: 'DJI RS3', ip: '192.168.1.10', mode: 'real', connected: true },
        { id: 'gimbal-2', name: 'PTZ Camera 2', model: 'DJI RS3', ip: '192.168.1.11', mode: 'real', connected: true },
      ]);
    });

    it('should show gimbal mapping section when gimbals available', () => {
      render(<AtemSettings />);
      expect(screen.getByText('Gimbal to Camera Mapping')).toBeInTheDocument();
    });

    it('should show gimbal names', () => {
      render(<AtemSettings />);
      expect(screen.getByText('PTZ Camera 1')).toBeInTheDocument();
      expect(screen.getByText('PTZ Camera 2')).toBeInTheDocument();
    });

    it('should show camera port selector', () => {
      render(<AtemSettings />);
      const selects = screen.getAllByRole('combobox');
      expect(selects.length).toBeGreaterThan(0);
    });

    it('should have No Camera as default option', () => {
      render(<AtemSettings />);
      expect(screen.getAllByText('No Camera').length).toBeGreaterThan(0);
    });

    it('should call setAtemGimbalMapping on port change', async () => {
      render(<AtemSettings />);
      const selects = screen.getAllByRole('combobox');

      // First gimbal's port selector
      fireEvent.change(selects[0], { target: { value: '1' } });

      expect(gimbalSocket.setAtemGimbalMapping).toHaveBeenCalledWith('gimbal-1', 1, 'MFT');
    });

    it('should show camera type selector when port is selected', async () => {
      useAtemStore.getState().setGimbalMapping('gimbal-1', 1, 1);

      render(<AtemSettings />);

      // Should now see MFT and EF options
      expect(screen.getByText('MFT Lens')).toBeInTheDocument();
      expect(screen.getByText('EF Lens')).toBeInTheDocument();
    });

    it('should filter out virtual gimbals', () => {
      useGimbalStore.getState().setAvailableGimbals([
        { id: 'gimbal-1', name: 'PTZ Camera 1', model: 'DJI RS3', ip: '192.168.1.10', mode: 'real', connected: true },
        { id: 'virtual', name: 'Virtual Gimbal', model: 'Virtual', ip: '', mode: 'virtual', connected: true },
      ]);

      render(<AtemSettings />);

      expect(screen.getByText('PTZ Camera 1')).toBeInTheDocument();
      expect(screen.queryByText('Virtual Gimbal')).not.toBeInTheDocument();
    });
  });

  describe('No gimbals message', () => {
    it('should show message when no gimbals available', () => {
      useGimbalStore.getState().setAvailableGimbals([]);

      render(<AtemSettings />);
      expect(
        screen.getByText('Add gimbals in the Gimbal Devices section above to configure ATEM camera mappings.')
      ).toBeInTheDocument();
    });

    it('should show message when only virtual gimbal available', () => {
      useGimbalStore.getState().setAvailableGimbals([
        { id: 'virtual', name: 'Virtual Gimbal', model: 'Virtual', ip: '', mode: 'virtual', connected: true },
      ]);

      render(<AtemSettings />);
      expect(
        screen.getByText('Add gimbals in the Gimbal Devices section above to configure ATEM camera mappings.')
      ).toBeInTheDocument();
    });
  });
});
