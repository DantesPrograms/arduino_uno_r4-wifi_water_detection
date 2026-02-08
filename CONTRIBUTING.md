# Contributing to Water Sensor Alert System

First off, thank you for considering contributing to this project! 🎉

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check the existing issues to avoid duplicates.

When you create a bug report, please include:

- **Description**: Clear and concise description of the bug
- **Steps to Reproduce**: Step-by-step instructions
- **Expected Behavior**: What you expected to happen
- **Actual Behavior**: What actually happened
- **Hardware Setup**: Arduino version, sensor type, components used
- **Software Version**: Arduino IDE version, library versions
- **Serial Monitor Output**: Copy relevant debug output
- **Photos**: If applicable, photos of wiring

**Example Bug Report:**

```markdown
**Bug**: LED stays on permanently after first alert

**Steps to Reproduce:**
1. Upload sketch version 1.0.0
2. Trigger water detection
3. Remove water from sensor
4. LED remains on

**Expected**: LED should turn off when water cleared

**Actual**: LED stays on indefinitely

**Hardware**: Arduino UNO R4 WiFi, generic water sensor
**Software**: Arduino IDE 2.3.0
**Serial Output**: [paste output here]
```

### Suggesting Enhancements

Enhancement suggestions are welcome! Please include:

- **Use Case**: Why this enhancement would be useful
- **Detailed Description**: How it should work
- **Implementation Ideas**: If you have technical suggestions
- **Alternatives**: Other solutions you considered

**Example Enhancement:**

```markdown
**Enhancement**: Add temperature sensor support

**Use Case**: Monitor water temperature in addition to presence

**Description**: Integrate DS18B20 temperature sensor and display 
temperature on web dashboard

**Implementation**: 
- Add OneWire library
- New function readTemperature()
- Update web interface with temperature display

**Alternatives**: Could use DHT22 for temp + humidity
```

### Pull Requests

1. **Fork the Repository**
   ```bash
   git fork https://github.com/yourusername/water-sensor-alert
   ```

2. **Create a Branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make Your Changes**
   - Write clean, commented code
   - Follow existing code style
   - Test thoroughly on hardware

4. **Commit Your Changes**
   ```bash
   git commit -m "Add: Description of your changes"
   ```
   
   Use commit message prefixes:
   - `Add:` New feature
   - `Fix:` Bug fix
   - `Update:` Improvement to existing feature
   - `Docs:` Documentation changes
   - `Refactor:` Code restructuring
   - `Test:` Test additions

5. **Push to Your Fork**
   ```bash
   git push origin feature/your-feature-name
   ```

6. **Create Pull Request**
   - Use a clear, descriptive title
   - Reference any related issues
   - Include test results
   - Add screenshots if UI changes

## Code Style Guidelines

### Arduino/C++ Code

```cpp
// Use descriptive variable names
const int WATER_SENSOR_PIN = A0;  // GOOD
const int a = A0;                  // BAD

// Comment complex logic
void calculateThreshold() {
    // Use hysteresis to prevent flickering
    if (!waterDetected && sensorValue > WATER_THRESHOLD) {
        waterDetected = true;
    }
}

// Consistent indentation (2 spaces)
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

// Constants in UPPERCASE
const int MAX_ALERTS = 100;
const unsigned long TIMEOUT_MS = 5000;

// Functions in camelCase
void readWaterSensor() { }
void triggerAlert() { }

// Add function headers for complex functions
/*
 * Reads the water sensor and updates detection state
 * Uses hysteresis to prevent false triggers
 * Returns: true if state changed, false otherwise
 */
bool readWaterSensor() {
    // implementation
}
```

### Documentation

```markdown
# Use clear headings

## Organize logically

- Use bullet points for lists
- **Bold** important terms
- `code blocks` for code
- Links to [relevant resources](https://example.com)
```

## Testing Requirements

Before submitting a PR, please test:

### Hardware Testing
- [ ] Fresh upload to Arduino
- [ ] Sensor reading accuracy
- [ ] LED activation/deactivation
- [ ] Buzzer activation/deactivation
- [ ] WiFi connection stability
- [ ] Web interface accessibility
- [ ] Multiple alert cycles
- [ ] Sensor dry/wet transitions
- [ ] Extended runtime (1+ hour)

### Software Testing
- [ ] Code compiles without errors
- [ ] No compiler warnings
- [ ] Serial Monitor output correct
- [ ] API returns valid JSON
- [ ] Web interface displays correctly
- [ ] Auto-refresh works
- [ ] Multiple browser compatibility

### Documentation Testing
- [ ] README instructions accurate
- [ ] Code comments accurate
- [ ] Wiring diagram correct
- [ ] Hardware list complete
- [ ] Troubleshooting section helpful

## Development Setup

### Required Software

1. **Arduino IDE 2.0+**
   - Download from [arduino.cc](https://www.arduino.cc/en/software)

2. **Arduino UNO R4 WiFi Board Support**
   - Install via Boards Manager

3. **Git**
   - For version control

### Optional Tools

- **PlatformIO**: Alternative to Arduino IDE
- **Serial Monitor Tools**: PuTTY, CoolTerm
- **API Testing**: Postman, cURL
- **Multimeter**: For hardware debugging

## Project Structure

```
water-sensor-alert/
├── WaterSensorAlert.ino    # Main sketch
├── README.md                # Main documentation
├── HARDWARE.md              # Hardware guide
├── API.md                   # API documentation
├── CONTRIBUTING.md          # This file
├── LICENSE                  # MIT License
├── .gitignore              # Git ignore rules
└── docs/                   # Additional documentation
    └── images/             # Diagrams and screenshots
```

## Feature Branches

Create feature branches for:

- New hardware support (e.g., `feature/multiple-sensors`)
- Major UI changes (e.g., `feature/dark-mode`)
- New protocols (e.g., `feature/mqtt-support`)
- Performance improvements (e.g., `feature/optimize-memory`)

## Code Review Process

1. **Automated Checks**
   - Code compiles successfully
   - No syntax errors

2. **Maintainer Review**
   - Code quality and style
   - Documentation completeness
   - Testing thoroughness
   - Backward compatibility

3. **Community Feedback**
   - Other contributors may comment
   - Address feedback promptly
   - Be respectful and constructive

## Documentation Standards

### Code Comments

```cpp
// Single line comments for brief explanations
void simpleFunction() { }

/*
 * Multi-line comments for complex functions
 * Explain parameters, return values, and behavior
 */
void complexFunction(int param) { }

/**
 * @brief Brief description
 * @param param Description of parameter
 * @return Description of return value
 */
int documentedFunction(int param) { }
```

### README Updates

When adding features, update:
- Features list
- Installation instructions
- Configuration options
- Usage examples
- Troubleshooting section

## Hardware Contributions

If contributing hardware designs:

- Provide clear wiring diagrams
- List all components with part numbers
- Test on actual hardware
- Include photos of working setup
- Document any modifications needed

## Questions?

- Open an issue for questions
- Use Discussions for general chat
- Tag maintainers if urgent

## Code of Conduct

### Our Pledge

We are committed to providing a welcoming and inclusive environment for all contributors.

### Expected Behavior

- Be respectful and considerate
- Welcome newcomers
- Accept constructive criticism
- Focus on what is best for the community
- Show empathy towards others

### Unacceptable Behavior

- Harassment or discrimination
- Trolling or insulting comments
- Publishing others' private information
- Other conduct inappropriate for a professional setting

### Enforcement

Violations may result in:
1. Warning
2. Temporary ban
3. Permanent ban

Report violations to: [your-email@example.com]

## Recognition

Contributors will be acknowledged in:
- README.md Contributors section
- Release notes
- Documentation credits

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for contributing!** 🙏

Every contribution, no matter how small, makes this project better for everyone.
