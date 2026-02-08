# 💧 Water Sensor Alert System

A complete water detection system using Arduino UNO R4 WiFi with a beautiful web interface, real-time alerts, and comprehensive monitoring features.

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)
![WiFi](https://img.shields.io/badge/WiFi-Enabled-blue?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

## ✨ Features

- 🌊 **Real-time Water Detection** - Monitors water sensor on analog pin A0
- 🌐 **Beautiful Web Interface** - Responsive, gradient-styled dashboard accessible from any device
- 🚨 **Multi-Modal Alerts** - Visual (LED) and audio (buzzer) warnings
- 📊 **Live Statistics** - Sensor readings, alert history, and system uptime
- 🔄 **Auto-Refresh** - Web interface updates every 2 seconds
- 📱 **Mobile Responsive** - Works perfectly on phones and tablets
- 🔔 **Alert Cooldown** - Prevents alert spam with configurable cooldown period
- 📈 **Alert History** - Tracks total alerts and last detection time

## 🛠️ Hardware Requirements

### Components

| Component | Quantity | Notes |
|-----------|----------|-------|
| Arduino UNO R4 WiFi | 1 | Must be R4 WiFi variant |
| Water Sensor | 1 | Analog output type |
| Active Buzzer | 1 | 5V compatible |
| Red LED | 1 | Any standard 5mm LED |
| 220Ω Resistor | 1 | For LED current limiting |
| Jumper Wires | Several | Male-to-male and male-to-female |
| Breadboard | 1 | Optional but recommended |

### Circuit Diagram

```
Arduino UNO R4 WiFi
┌─────────────────┐
│                 │
│  A0 ────────────┼──── Water Sensor Signal
│                 │
│  D8 ────[220Ω]──┼──── Red LED ──── GND
│                 │
│  D9 ────────────┼──── Active Buzzer (+)
│                 │        │
│ GND ────────────┼────────┴───── Buzzer (-)
│                 │             and Sensor (-)
│                 │
│ 5V ─────────────┼──── Water Sensor VCC
│                 │
└─────────────────┘
```

## 📐 Wiring Instructions

### Water Sensor
- **VCC** → Arduino 5V
- **GND** → Arduino GND
- **Signal** → Arduino A0

### Red LED
- **Anode (+)** → Arduino D8 (through 220Ω resistor)
- **Cathode (-)** → Arduino GND

### Active Buzzer
- **Positive (+)** → Arduino D9
- **Negative (-)** → Arduino GND

## 🚀 Installation

### Step 1: Install Arduino IDE

Download and install the latest [Arduino IDE](https://www.arduino.cc/en/software) (version 2.0 or higher recommended).

### Step 2: Install Board Support

1. Open Arduino IDE
2. Go to **Tools** → **Board** → **Boards Manager**
3. Search for "Arduino UNO R4"
4. Install **Arduino UNO R4 Boards** by Arduino

### Step 3: Clone Repository

```bash
git clone https://github.com/yourusername/water-sensor-alert.git
cd water-sensor-alert
```

### Step 4: Configure WiFi

Open `WaterSensorAlert.ino` and update these lines with your WiFi credentials:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

### Step 5: Adjust Sensor Threshold (Optional)

Calibrate the water detection threshold based on your sensor:

```cpp
const int WATER_THRESHOLD = 300;  // Increase if too sensitive
const int DRY_THRESHOLD = 100;    // For hysteresis
```

**Calibration Steps:**
1. Upload the sketch with Serial Monitor open
2. Test sensor in dry condition - note the reading
3. Test sensor in wet condition - note the reading
4. Set `WATER_THRESHOLD` between these values
5. Set `DRY_THRESHOLD` below the dry reading

### Step 6: Upload Sketch

1. Connect Arduino UNO R4 WiFi via USB
2. Select **Tools** → **Board** → **Arduino UNO R4 WiFi**
3. Select **Tools** → **Port** → [Your Arduino Port]
4. Click **Upload** button

### Step 7: Access Web Interface

1. Open Serial Monitor (115200 baud)
2. Note the IP address displayed (e.g., `192.168.1.100`)
3. Open web browser and navigate to: `http://[IP_ADDRESS]`

## 🖥️ Web Interface

The web interface provides:

- **Status Indicator** - Large visual display showing current state
- **Real-time Metrics**:
  - Current sensor reading (0-1023)
  - Total alert count
  - Last alert timestamp
- **System Information**:
  - System uptime
  - WiFi signal strength
  - Alert status
  - Configured threshold

### API Endpoint

Access raw JSON data at: `http://[IP_ADDRESS]/api/status`

**Response Format:**
```json
{
  "waterDetected": false,
  "sensorValue": 45,
  "alertActive": false,
  "totalAlerts": 3,
  "lastAlert": "00:15:32",
  "uptime": "1h 23m 45s",
  "signalStrength": -45
}
```

## ⚙️ Configuration Options

### Alert Behavior

```cpp
// Alert duration (milliseconds)
const unsigned long BUZZER_DURATION = 5000;  // 5 seconds

// Cooldown between alerts (milliseconds)
const unsigned long ALERT_COOLDOWN = 30000;  // 30 seconds
```

### Pin Configuration

```cpp
const int WATER_SENSOR_PIN = A0;  // Analog pin for sensor
const int RED_LED_PIN = 8;        // Digital pin for LED
const int BUZZER_PIN = 9;         // Digital pin for buzzer
```

## 🔧 Troubleshooting

### IP Address Shows 0.0.0.0

**Problem:** Serial Monitor shows IP address as `0.0.0.0` or `(IP unset)`

**This means WiFi is NOT connected. Common causes:**
- Incorrect SSID or password (case-sensitive!)
- Using 5GHz network (Arduino only supports 2.4GHz)
- Router security set to WPA3-only (use WPA2 or mixed mode)
- Weak WiFi signal
- MAC filtering enabled on router

**Quick fixes:**
1. Double-check WiFi credentials in the code
2. Ensure you're connecting to a 2.4GHz network
3. Move Arduino closer to router
4. Try mobile hotspot to test
5. Check Serial Monitor for detailed connection status

**For detailed solutions, see [TROUBLESHOOTING.md](TROUBLESHOOTING.md)**

### WiFi Connection Issues

**Problem:** Arduino doesn't connect to WiFi
- Verify SSID and password are correct
- Ensure 2.4GHz WiFi is enabled (R4 doesn't support 5GHz)
- Check router firewall settings
- Try moving Arduino closer to router

### Sensor Not Detecting Water

**Problem:** Sensor reads low values when wet
- Check wiring connections
- Verify sensor power (5V and GND)
- Adjust `WATER_THRESHOLD` value lower
- Test sensor with multimeter

### False Alerts

**Problem:** Alerts trigger without water
- Increase `WATER_THRESHOLD` value
- Check for loose connections
- Ensure sensor is clean and dry
- Add shielded cable if electrical noise present

### Web Page Not Loading

**Problem:** Cannot access web interface
- Verify IP address from Serial Monitor
- Ensure device is on same network
- Check Arduino hasn't lost WiFi connection
- Try resetting Arduino

### Buzzer Always On

**Problem:** Buzzer doesn't turn off
- Check `BUZZER_DURATION` setting
- Verify buzzer wiring
- Look for stuck alert condition in Serial Monitor

## 📊 Serial Monitor Output

The Serial Monitor provides detailed logging:

```
=== Water Sensor Alert System ===
Starting up...
Connecting to WiFi: MyNetwork
........
WiFi connected!
IP address: 192.168.1.100
Signal strength: -52 dBm
Web server started!
Access at: http://192.168.1.100
================================

🚨 WATER DETECTED! 🚨
Sensor value: 456
🔴 Alert triggered!
Total alerts: 1
🟢 Alert cleared
✓ Water cleared
Sensor value: 78
```

## 🎨 Customization

### Change LED Color

Replace the red LED with any color. Update variable names for clarity:

```cpp
const int GREEN_LED_PIN = 8;
```

### Add Multiple Sensors

Expand to monitor multiple locations:

```cpp
const int SENSOR_1_PIN = A0;
const int SENSOR_2_PIN = A1;
const int SENSOR_3_PIN = A2;
```

### Modify Web Theme

Edit the CSS in `sendHTMLResponse()` function to change colors:

```cpp
// Change gradient colors
client.println("background: linear-gradient(135deg, #YOUR_COLOR_1 0%, #YOUR_COLOR_2 100%);");
```

## 📝 Function Reference

### Core Functions

#### `void setup()`
Initializes hardware, WiFi connection, and web server.

#### `void loop()`
Main program loop - reads sensor, handles alerts, processes web requests.

### Sensor Functions

#### `void readWaterSensor()`
Reads analog sensor value and updates detection state with hysteresis.

### Alert Functions

#### `void triggerAlert()`
Activates LED, buzzer, and updates statistics when water is detected.

#### `void handleAlerts()`
Manages alert timing and deactivation based on sensor state.

### Utility Functions

#### `void updateUptime()`
Tracks system uptime in seconds.

#### `String getTimeString()`
Returns formatted timestamp (HH:MM:SS).

#### `String getUptimeString()`
Returns human-readable uptime (e.g., "1d 5h 23m 45s").

### Network Functions

#### `void connectToWiFi()`
Establishes WiFi connection with retry logic.

#### `void handleWebServer()`
Processes incoming HTTP requests.

#### `void sendHTMLResponse(WiFiClient &client)`
Generates and sends the main web interface.

#### `void sendJSONResponse(WiFiClient &client)`
Sends JSON data for API endpoint.

## 🔐 Security Considerations

- The web server is **unencrypted** (HTTP not HTTPS)
- No authentication is implemented
- Only use on trusted local networks
- Do not expose directly to the internet
- Consider adding password protection for production use

## 📄 License

This project is licensed under the MIT License - see below for details:

```
MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

### Development Setup

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 🌟 Future Enhancements

Potential features for future versions:

- [ ] Email/SMS notifications
- [ ] Data logging to SD card
- [ ] Integration with home automation (MQTT)
- [ ] Battery backup support
- [ ] Multiple sensor support
- [ ] Historical data graphing
- [ ] Mobile app
- [ ] Cloud dashboard
- [ ] Temperature sensor integration
- [ ] OTA (Over-The-Air) updates

## 📞 Support

If you encounter issues or have questions:

1. Check the [Troubleshooting](#-troubleshooting) section
2. Review [Issues](https://github.com/yourusername/water-sensor-alert/issues) on GitHub
3. Open a new issue with detailed information

## 🙏 Acknowledgments

- Arduino community for excellent documentation
- Contributors and testers
- Open source community

## 📸 Screenshots

### Normal State
![Normal State](docs/images/normal-state.png)

### Alert State
![Alert State](docs/images/alert-state.png)

---

**Made with ❤️ using Arduino UNO R4 WiFi**

*Star this repo if you found it helpful! ⭐*
