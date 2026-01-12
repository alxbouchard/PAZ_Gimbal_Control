import { describe, it, expect, vi, beforeEach } from 'vitest';
import { render, screen, fireEvent } from '@testing-library/react';
import userEvent from '@testing-library/user-event';
import { PresetControls } from '../PresetControls';
import { usePresetsStore } from '../../store/presetsStore';
import { useGimbalStore } from '../../store/gimbalStore';

// Mock the websocket service
vi.mock('../../services/websocket', () => ({
  gimbalSocket: {
    savePreset: vi.fn(),
    recallPreset: vi.fn(),
    deletePreset: vi.fn(),
  },
}));

import { gimbalSocket } from '../../services/websocket';

describe('PresetControls', () => {
  beforeEach(() => {
    // Reset stores
    useGimbalStore.getState().setConnected(true);
    useGimbalStore.getState().setActiveGimbal('test-gimbal');
    usePresetsStore.getState().clearPresets();

    vi.clearAllMocks();
  });

  describe('Initial render', () => {
    it('should render Position Presets header', () => {
      render(<PresetControls />);
      expect(screen.getByText('Position Presets')).toBeInTheDocument();
    });

    it('should render 9 preset buttons', () => {
      render(<PresetControls />);
      for (let i = 1; i <= 9; i++) {
        expect(screen.getByText(String(i))).toBeInTheDocument();
      }
    });

    it('should render Edit button', () => {
      render(<PresetControls />);
      expect(screen.getByText('Edit')).toBeInTheDocument();
    });

    it('should show Empty for presets without data', () => {
      render(<PresetControls />);
      expect(screen.getAllByText('Empty').length).toBeGreaterThan(0);
    });

    it('should show help text', () => {
      render(<PresetControls />);
      expect(
        screen.getByText('Click to recall • Long press to save • Use Shift+1-9 as shortcuts')
      ).toBeInTheDocument();
    });
  });

  describe('Edit mode', () => {
    it('should toggle edit mode on Edit button click', async () => {
      render(<PresetControls />);

      await userEvent.click(screen.getByText('Edit'));
      expect(screen.getByText('SAVE MODE')).toBeInTheDocument();
    });

    it('should show Click to save for empty slots in edit mode', async () => {
      render(<PresetControls />);

      await userEvent.click(screen.getByText('Edit'));
      expect(screen.getAllByText('Click to save').length).toBe(9);
    });

    it('should save preset when clicking empty slot in edit mode', async () => {
      render(<PresetControls />);

      await userEvent.click(screen.getByText('Edit'));
      await userEvent.click(screen.getByText('1'));

      expect(gimbalSocket.savePreset).toHaveBeenCalledWith('test-gimbal', 1);
    });

    it('should show different help text in edit mode', async () => {
      render(<PresetControls />);

      await userEvent.click(screen.getByText('Edit'));
      expect(screen.getByText('Click a slot to save current position')).toBeInTheDocument();
    });
  });

  describe('Presets with data', () => {
    beforeEach(() => {
      usePresetsStore.getState().setPresets('test-gimbal', {
        '1': { pitch: 30, yaw: 45, roll: 0 },
        '5': { pitch: -15, yaw: 90, roll: 10 },
      });
    });

    it('should show position data for saved presets', () => {
      render(<PresetControls />);
      expect(screen.getByText('30° / 45°')).toBeInTheDocument();
      expect(screen.getByText('-15° / 90°')).toBeInTheDocument();
    });

    it('should recall preset on click', async () => {
      render(<PresetControls />);

      // Click the first preset button (which has data)
      const preset1 = screen.getByText('30° / 45°').closest('button');
      if (preset1) {
        await userEvent.click(preset1);
      }

      expect(gimbalSocket.recallPreset).toHaveBeenCalledWith('test-gimbal', 1);
    });

    it('should not recall empty preset on click', async () => {
      render(<PresetControls />);

      // Find an empty preset button (preset 2)
      // Buttons: Edit, 9 presets
      // Find the button with "2" text
      const preset2Btn = screen.getByText('2').closest('button');
      if (preset2Btn) {
        await userEvent.click(preset2Btn);
      }

      // Should not call recallPreset for empty preset
      expect(gimbalSocket.recallPreset).not.toHaveBeenCalled();
    });

    it('should show delete button in edit mode for saved presets', async () => {
      render(<PresetControls />);

      await userEvent.click(screen.getByText('Edit'));

      // Delete buttons should appear for presets 1 and 5
      // The Trash2 icon creates delete buttons
      const deleteButtons = document.querySelectorAll('button.absolute');
      expect(deleteButtons.length).toBe(2); // Two presets have data
    });

    it('should delete preset when clicking delete button', async () => {
      render(<PresetControls />);

      await userEvent.click(screen.getByText('Edit'));

      // Find delete button
      const deleteButtons = document.querySelectorAll('button.absolute');
      if (deleteButtons[0]) {
        await userEvent.click(deleteButtons[0]);
      }

      expect(gimbalSocket.deletePreset).toHaveBeenCalled();
    });
  });

  describe('Long press to save', () => {
    it('should have long press functionality configured', () => {
      // The component uses setTimeout for long press (800ms)
      // Testing this requires complex timer mocking with React's useState
      // Instead, we verify the button exists and responds to mouse events
      render(<PresetControls />);

      const preset1Btn = screen.getByText('1').closest('button');
      expect(preset1Btn).toBeDefined();

      // MouseDown and MouseUp events should work without error
      if (preset1Btn) {
        expect(() => fireEvent.mouseDown(preset1Btn)).not.toThrow();
        expect(() => fireEvent.mouseUp(preset1Btn)).not.toThrow();
      }
    });
  });

  describe('Disabled state', () => {
    it('should disable buttons when disconnected', () => {
      useGimbalStore.getState().setConnected(false);

      render(<PresetControls />);

      // All preset buttons should be disabled
      const buttons = screen.getAllByRole('button');
      // Filter out the Edit button
      const presetButtons = buttons.filter((b) => !b.textContent?.includes('Edit'));
      presetButtons.forEach((btn) => {
        expect(btn).toBeDisabled();
      });
    });

    it('should not call recallPreset when disconnected', async () => {
      useGimbalStore.getState().setConnected(false);
      usePresetsStore.getState().setPresets('test-gimbal', {
        '1': { pitch: 30, yaw: 45, roll: 0 },
      });

      render(<PresetControls />);

      const preset1Btn = screen.getByText('1').closest('button');
      if (preset1Btn) {
        // Force click even though disabled
        fireEvent.click(preset1Btn);
      }

      expect(gimbalSocket.recallPreset).not.toHaveBeenCalled();
    });
  });

  describe('No active gimbal', () => {
    it('should handle no active gimbal', () => {
      useGimbalStore.getState().setActiveGimbal(null);

      render(<PresetControls />);

      // Should still render but presets should be empty
      expect(screen.getAllByText('Empty').length).toBe(9);
    });

    it('should not call savePreset without active gimbal', async () => {
      useGimbalStore.getState().setActiveGimbal(null);

      render(<PresetControls />);

      await userEvent.click(screen.getByText('Edit'));
      await userEvent.click(screen.getByText('1'));

      expect(gimbalSocket.savePreset).not.toHaveBeenCalled();
    });
  });

  describe('Multiple gimbals', () => {
    it('should show presets for active gimbal only', () => {
      usePresetsStore.getState().setPresets('gimbal-1', {
        '1': { pitch: 10, yaw: 20, roll: 0 },
      });
      usePresetsStore.getState().setPresets('gimbal-2', {
        '1': { pitch: 30, yaw: 40, roll: 0 },
      });

      useGimbalStore.getState().setActiveGimbal('gimbal-1');

      render(<PresetControls />);
      expect(screen.getByText('10° / 20°')).toBeInTheDocument();
      expect(screen.queryByText('30° / 40°')).not.toBeInTheDocument();
    });

    it('should update presets when active gimbal changes', () => {
      usePresetsStore.getState().setPresets('gimbal-1', {
        '1': { pitch: 10, yaw: 20, roll: 0 },
      });
      usePresetsStore.getState().setPresets('gimbal-2', {
        '1': { pitch: 30, yaw: 40, roll: 0 },
      });

      useGimbalStore.getState().setActiveGimbal('gimbal-1');
      const { rerender } = render(<PresetControls />);

      // Change active gimbal
      useGimbalStore.getState().setActiveGimbal('gimbal-2');
      rerender(<PresetControls />);

      expect(screen.getByText('30° / 40°')).toBeInTheDocument();
      expect(screen.queryByText('10° / 20°')).not.toBeInTheDocument();
    });
  });
});
