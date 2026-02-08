# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- WebSocket support for real-time updates
- MQTT integration for home automation
- Email/SMS notification system
- Data logging to SD card
- Configuration web interface
- Multiple sensor support
- Temperature sensor integration
- Battery backup monitoring
- OTA (Over-The-Air) firmware updates
- Historical data visualization
- Dark mode for web interface

## [1.0.0] - 2026-02-07

### Added
- Initial release
- Water sensor monitoring on analog pin A0
- Visual alert system with red LED on D8
- Audio alert system with active buzzer on D9
- WiFi connectivity for Arduino UNO R4 WiFi
- Beautiful gradient-styled web interface
- Real-time status dashboard
- Auto-refresh web interface (2-second interval)
- JSON API endpoint at `/api/status`
- Alert cooldown mechanism (30 seconds)
- Buzzer auto-shutoff (5 seconds)
- Hysteresis for sensor readings
- Alert statistics tracking
- System uptime monitoring
- WiFi signal strength display
- Responsive mobile-friendly interface
- Serial Monitor debug output
- Comprehensive documentation
  - README.md with quick start guide
  - HARDWARE.md with wiring instructions
  - API.md with integration examples
  - CONTRIBUTING.md with development guidelines
- MIT License
- Example integration code for:
  - Python
  - Node.js
  - Browser JavaScript
  - Bash/Shell
  - Home Assistant

### Hardware Support
- Arduino UNO R4 WiFi board
- Generic water sensors (analog)
- Active buzzers (5V)
- Standard LEDs with current-limiting resistor

### Features
- Configurable water detection threshold
- Configurable dry threshold for hysteresis
- Adjustable alert duration
- Adjustable cooldown period
- Pin configuration flexibility
- Real-time sensor value display (0-1023)
- Total alert counter
- Last alert timestamp
- System uptime tracking
- WiFi signal strength monitoring

### Documentation
- Complete bill of materials (BOM)
- Circuit diagrams
- Breadboard layout guide
- Calibration instructions
- Troubleshooting guide
- API documentation with examples
- Function reference
- Configuration options
- Security considerations
- Testing procedures

## Version History Notes

### Version Numbering

This project uses [Semantic Versioning](https://semver.org/):

- **MAJOR** version (X.0.0): Incompatible changes
- **MINOR** version (0.X.0): New features, backward compatible
- **PATCH** version (0.0.X): Bug fixes, backward compatible

### Release Process

1. Update CHANGELOG.md with changes
2. Update version number in sketch comments
3. Test thoroughly on hardware
4. Create git tag with version number
5. Create GitHub release with notes
6. Update documentation if needed

### How to Read This Changelog

- **Added**: New features
- **Changed**: Changes to existing functionality
- **Deprecated**: Features to be removed in future
- **Removed**: Removed features
- **Fixed**: Bug fixes
- **Security**: Security vulnerability fixes

---

## Future Version Plans

### [1.1.0] - Planned

**Target Date:** TBD

#### Proposed Features
- Configuration web interface
- Adjustable thresholds via web
- Multiple sensor support
- Sensor naming/labeling
- Email notifications
- Data export functionality

### [1.2.0] - Planned

**Target Date:** TBD

#### Proposed Features
- MQTT integration
- Home automation compatibility
- Historical data graphing
- Data logging to SD card
- Temperature sensor support
- Advanced alert rules

### [2.0.0] - Planned

**Target Date:** TBD

#### Breaking Changes
- Potential API changes
- New hardware requirements
- Configuration file format

#### Major Features
- WebSocket support
- Mobile app
- Cloud dashboard
- OTA updates
- Advanced security features

---

## Migration Guides

### From Beta to 1.0.0

No beta version existed. This is the initial stable release.

### Future Migrations

Migration guides will be provided for any breaking changes between major versions.

---

## Known Issues

### Version 1.0.0

#### Minor Issues
- No HTTPS support (HTTP only)
- No authentication on web interface
- Limited to 4 simultaneous web connections
- No persistent storage (statistics reset on reboot)
- Sensor corrosion over time (hardware limitation)

#### Workarounds
- Use on trusted networks only
- Implement VPN for remote access
- Limit polling frequency
- Use capacitive sensor for longevity

#### Won't Fix
- HTTPS (Arduino hardware limitation)
- More than 4 connections (Arduino hardware limitation)

---

## Deprecation Notices

### Current Version (1.0.0)
No deprecated features.

### Future Deprecations
Deprecated features will be announced at least one major version in advance.

---

## Security Updates

### Version 1.0.0
- No known security vulnerabilities
- Warning: Web interface has no authentication
- Warning: Data transmitted unencrypted (HTTP)
- Recommendation: Use on trusted networks only

---

## Performance Notes

### Version 1.0.0
- Typical loop execution: ~100-110ms
- Web request response time: 50-200ms
- WiFi connection time: 2-5 seconds
- Memory usage: ~15KB SRAM, ~45KB Flash
- Power consumption: ~170mA at 5V

---

## Compatibility

### Arduino Boards
- ✅ Arduino UNO R4 WiFi
- ❌ Arduino UNO R3 (no WiFi)
- ❌ Arduino UNO R4 Minima (no WiFi)
- ⚠️ Other ESP32/ESP8266 boards (requires code modification)

### Arduino IDE Versions
- ✅ Arduino IDE 2.0.0+
- ✅ Arduino IDE 1.8.19+
- ✅ Arduino CLI
- ✅ PlatformIO

### Web Browsers
- ✅ Chrome/Chromium 90+
- ✅ Firefox 88+
- ✅ Safari 14+
- ✅ Edge 90+
- ✅ Mobile browsers (iOS Safari, Chrome Mobile)

### WiFi Standards
- ✅ 802.11 b/g/n (2.4 GHz)
- ❌ 802.11 ac/ax (5 GHz not supported by R4 WiFi)

---

## Contributors

### Version 1.0.0
- Initial development and release

---

**Questions about a specific version?** Check the documentation for that release or open an issue.

**Want to contribute?** See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.
