# Changelog

All notable changes to PAZ Gimbal Control will be documented in this file.

## [1.2.0] - 2025-12-20

### Added
- **Gimbal Switcher in Control Panel**: Direct numbered buttons (1, 2, 3, 4...) for instant gimbal selection
  - Visual distinction between virtual (blue) and real (purple) gimbals
  - Connection status indicator with "offline" label for disconnected gimbals
  - Animated active indicator

### Changed
- **Settings Page Cleanup**: Removed unused settings and improved organization
  - Removed dead code: joystick axis mapping dropdowns (were never used)
  - Fixed server URL to use persistent store instead of local state
  - Renamed "On-Screen Joysticks" section to "Axis Inversion" (only functional settings)
  - Removed non-functional "Save Settings" button (settings auto-save)
  - Reordered sections for better UX: Gimbal Devices first

### Fixed
- Server URL setting now persists correctly across sessions

## [1.1.0] - 2025-12-19

### Added
- Improved 3D gimbal model matching DJI Ronin RS 3 design
- Better default control mappings

### Fixed
- Camera centering on all rotation axes in 3D visualizer

## [1.0.0] - 2025-12-18

### Added
- Initial release
- Real-time gimbal control via WebSocket
- 3D gimbal visualization with Three.js
- Xbox gamepad support with full button/axis mapping
- Customizable keyboard shortcuts
- Virtual gimbal mode for testing
- Multi-gimbal support via EthCAN
- Dashboard with telemetry graphs
- Zoom and focus control
