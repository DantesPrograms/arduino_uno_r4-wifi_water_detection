# 🌐 API Documentation

Complete API reference for the Water Sensor Alert System web interface.

## Overview

The Arduino web server provides both a human-readable HTML interface and a machine-readable JSON API for integration with other systems.

## Base URL

```
http://[ARDUINO_IP_ADDRESS]
```

Replace `[ARDUINO_IP_ADDRESS]` with your Arduino's IP address (displayed in Serial Monitor on startup).

## Endpoints

### GET /

**Description:** Main web dashboard interface

**Response Type:** `text/html`

**Status Code:** `200 OK`

**Description:**
Returns a fully styled, responsive HTML dashboard with:
- Real-time sensor status
- Visual alert indicators
- System statistics
- Auto-refresh functionality (2-second interval)

**Example Request:**
```bash
curl http://192.168.1.100/
```

**Response:**
HTML document with embedded CSS and JavaScript

---

### GET /api/status

**Description:** JSON API endpoint for current system status

**Response Type:** `application/json`

**Status Code:** `200 OK`

**Response Schema:**

```json
{
  "waterDetected": boolean,
  "sensorValue": integer,
  "alertActive": boolean,
  "totalAlerts": integer,
  "lastAlert": string,
  "uptime": string,
  "signalStrength": integer
}
```

**Field Descriptions:**

| Field | Type | Range | Description |
|-------|------|-------|-------------|
| `waterDetected` | boolean | true/false | Current water detection state |
| `sensorValue` | integer | 0-1023 | Raw analog sensor reading |
| `alertActive` | boolean | true/false | Whether alert is currently active |
| `totalAlerts` | integer | 0+ | Total number of alerts triggered since boot |
| `lastAlert` | string | HH:MM:SS or "Never" | Timestamp of most recent alert |
| `uptime` | string | - | System uptime in human-readable format |
| `signalStrength` | integer | -100 to 0 | WiFi signal strength in dBm |

**Example Request:**
```bash
curl http://192.168.1.100/api/status
```

**Example Response (No Water):**
```json
{
  "waterDetected": false,
  "sensorValue": 45,
  "alertActive": false,
  "totalAlerts": 0,
  "lastAlert": "Never",
  "uptime": "2h 15m 32s",
  "signalStrength": -52
}
```

**Example Response (Water Detected):**
```json
{
  "waterDetected": true,
  "sensorValue": 678,
  "alertActive": true,
  "totalAlerts": 3,
  "lastAlert": "02:15:32",
  "uptime": "5h 42m 18s",
  "signalStrength": -48
}
```

## Data Types

### Boolean Values

- `true` - Condition is active/detected
- `false` - Condition is inactive/not detected

### Integer Values

#### Sensor Value (0-1023)
- **0-100:** Dry condition
- **100-300:** Slight moisture
- **300-600:** Water detected (typical threshold)
- **600-1023:** Fully submerged

#### Signal Strength (dBm)
- **-30 to -50:** Excellent
- **-50 to -60:** Good
- **-60 to -70:** Fair
- **-70 to -80:** Weak
- **Below -80:** Very weak

### String Values

#### Uptime Format
- Pattern: `[Dd ][Hh ][Mm ]Ss`
- Examples:
  - `45s` - 45 seconds
  - `5m 30s` - 5 minutes 30 seconds
  - `2h 15m 45s` - 2 hours 15 minutes 45 seconds
  - `1d 5h 30m 15s` - 1 day 5 hours 30 minutes 15 seconds

#### Alert Timestamp Format
- Pattern: `HH:MM:SS` (24-hour format)
- Example: `14:35:22`
- Special value: `"Never"` if no alerts have occurred

## Integration Examples

### Python

```python
import requests
import time

ARDUINO_IP = "192.168.1.100"
API_URL = f"http://{ARDUINO_IP}/api/status"

def check_water_sensor():
    try:
        response = requests.get(API_URL, timeout=5)
        response.raise_for_status()
        
        data = response.json()
        
        if data['waterDetected']:
            print(f"⚠️  WATER DETECTED!")
            print(f"   Sensor Value: {data['sensorValue']}")
            print(f"   Total Alerts: {data['totalAlerts']}")
        else:
            print(f"✓ All clear (reading: {data['sensorValue']})")
        
        return data
        
    except requests.exceptions.RequestException as e:
        print(f"Error connecting to sensor: {e}")
        return None

# Poll every 5 seconds
while True:
    check_water_sensor()
    time.sleep(5)
```

### JavaScript (Node.js)

```javascript
const axios = require('axios');

const ARDUINO_IP = '192.168.1.100';
const API_URL = `http://${ARDUINO_IP}/api/status`;

async function checkWaterSensor() {
    try {
        const response = await axios.get(API_URL);
        const data = response.data;
        
        if (data.waterDetected) {
            console.log('⚠️  WATER DETECTED!');
            console.log(`   Sensor Value: ${data.sensorValue}`);
            console.log(`   Alert Active: ${data.alertActive}`);
            
            // Send notification, email, etc.
            await sendAlert(data);
        } else {
            console.log(`✓ All clear (reading: ${data.sensorValue})`);
        }
        
        return data;
        
    } catch (error) {
        console.error('Error connecting to sensor:', error.message);
        return null;
    }
}

// Poll every 5 seconds
setInterval(checkWaterSensor, 5000);
```

### JavaScript (Browser)

```javascript
const ARDUINO_IP = '192.168.1.100';
const API_URL = `http://${ARDUINO_IP}/api/status`;

async function updateStatus() {
    try {
        const response = await fetch(API_URL);
        const data = await response.json();
        
        // Update UI elements
        document.getElementById('sensorValue').textContent = data.sensorValue;
        document.getElementById('waterStatus').textContent = 
            data.waterDetected ? 'WATER DETECTED' : 'All Clear';
        document.getElementById('totalAlerts').textContent = data.totalAlerts;
        document.getElementById('uptime').textContent = data.uptime;
        
        // Change status color
        const statusElement = document.getElementById('status');
        statusElement.className = data.waterDetected ? 'alert' : 'safe';
        
    } catch (error) {
        console.error('Error fetching status:', error);
    }
}

// Auto-refresh every 2 seconds
setInterval(updateStatus, 2000);
updateStatus(); // Initial load
```

### Bash/Shell Script

```bash
#!/bin/bash

ARDUINO_IP="192.168.1.100"
API_URL="http://${ARDUINO_IP}/api/status"

while true; do
    # Fetch status
    response=$(curl -s "${API_URL}")
    
    # Parse JSON (requires jq)
    water_detected=$(echo "$response" | jq -r '.waterDetected')
    sensor_value=$(echo "$response" | jq -r '.sensorValue')
    
    if [ "$water_detected" = "true" ]; then
        echo "⚠️  WATER DETECTED! (Value: $sensor_value)"
        # Send notification
        notify-send "Water Alert" "Water detected by Arduino sensor!"
    else
        echo "✓ All clear (Value: $sensor_value)"
    fi
    
    sleep 5
done
```

### Home Assistant Integration

```yaml
# configuration.yaml

sensor:
  - platform: rest
    name: "Water Sensor"
    resource: "http://192.168.1.100/api/status"
    method: GET
    scan_interval: 5
    value_template: '{{ value_json.sensorValue }}'
    json_attributes:
      - waterDetected
      - alertActive
      - totalAlerts
      - uptime
      - signalStrength

binary_sensor:
  - platform: template
    sensors:
      water_detected:
        friendly_name: "Water Detected"
        value_template: >-
          {{ state_attr('sensor.water_sensor', 'waterDetected') }}
        device_class: moisture

automation:
  - alias: "Water Detection Alert"
    trigger:
      - platform: state
        entity_id: binary_sensor.water_detected
        to: 'on'
    action:
      - service: notify.mobile_app
        data:
          message: "Water detected by Arduino sensor!"
          title: "Water Alert"
```

## Rate Limiting

**Current Implementation:** No rate limiting

**Recommendations:**
- Limit polling to once per 1-2 seconds minimum
- Consider WebSocket implementation for real-time updates (future enhancement)
- Avoid excessive concurrent connections

**Arduino Limitations:**
- Maximum 4 simultaneous connections
- Each request blocks briefly (~100ms)
- Excessive requests may cause WiFi instability

## Error Handling

### Connection Timeout

**Symptoms:**
- Request hangs indefinitely
- No response received

**Possible Causes:**
- Arduino offline or rebooting
- WiFi connection lost
- Network congestion
- IP address changed

**Handling:**
```python
try:
    response = requests.get(API_URL, timeout=5)
except requests.Timeout:
    print("Connection timeout - Arduino may be offline")
```

### Invalid JSON

**Symptoms:**
- Parsing error
- Unexpected response format

**Possible Causes:**
- Arduino mid-reboot
- Corrupted transmission
- HTML error page instead of JSON

**Handling:**
```python
try:
    data = response.json()
except json.JSONDecodeError:
    print("Invalid JSON response")
```

### Network Errors

**HTTP Status Codes:**
- `200 OK` - Success
- `400 Bad Request` - Invalid request (shouldn't occur with GET)
- `404 Not Found` - Endpoint doesn't exist
- `500 Internal Server Error` - Arduino error (rare)

## CORS (Cross-Origin Resource Sharing)

**Current Status:** Not implemented

**Implications:**
- Web apps from different domains cannot access API
- Browser security restrictions apply
- Use same-origin or server-side proxy

**Future Enhancement:**
Add CORS headers to Arduino response:
```cpp
client.println("Access-Control-Allow-Origin: *");
```

## WebSocket Support

**Current Status:** Not available

**Future Feature:**
- Real-time bidirectional communication
- Push notifications instead of polling
- Reduced network overhead

## MQTT Support

**Current Status:** Not available

**Future Feature:**
- Publish sensor data to MQTT broker
- Subscribe to configuration commands
- Better home automation integration

## Security Considerations

⚠️ **Important Security Notes:**

1. **No Authentication**
   - API is completely open
   - Anyone on network can access
   - Consider adding HTTP Basic Auth

2. **No Encryption**
   - Data transmitted in plain text
   - Not suitable for public networks
   - HTTPS not supported on Arduino

3. **No Write Operations**
   - API is read-only
   - No configuration changes via API
   - Prevents unauthorized modifications

**Recommendations:**
- Use on trusted local network only
- Implement VPN for remote access
- Consider adding authentication token
- Do not expose directly to internet

## Monitoring & Alerting

### Setting Up Alerts

**Email Notifications (Python):**
```python
import smtplib
from email.mime.text import MIMEText

def send_email_alert(sensor_data):
    msg = MIMEText(f"Water detected! Sensor value: {sensor_data['sensorValue']}")
    msg['Subject'] = 'Water Sensor Alert'
    msg['From'] = 'sensor@example.com'
    msg['To'] = 'your@email.com'
    
    server = smtplib.SMTP('smtp.gmail.com', 587)
    server.starttls()
    server.login('username', 'password')
    server.send_message(msg)
    server.quit()
```

**SMS Notifications (Twilio):**
```python
from twilio.rest import Client

def send_sms_alert(sensor_data):
    client = Client('ACCOUNT_SID', 'AUTH_TOKEN')
    
    message = client.messages.create(
        body=f"Water detected! Value: {sensor_data['sensorValue']}",
        from_='+1234567890',
        to='+0987654321'
    )
```

### Data Logging

**Log to CSV:**
```python
import csv
from datetime import datetime

def log_reading(data):
    with open('sensor_log.csv', 'a', newline='') as f:
        writer = csv.writer(f)
        writer.writerow([
            datetime.now().isoformat(),
            data['sensorValue'],
            data['waterDetected'],
            data['uptime']
        ])
```

**Log to Database (SQLite):**
```python
import sqlite3
from datetime import datetime

def log_to_database(data):
    conn = sqlite3.connect('sensor_data.db')
    cursor = conn.cursor()
    
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS readings (
            timestamp TEXT,
            sensor_value INTEGER,
            water_detected BOOLEAN,
            uptime TEXT
        )
    ''')
    
    cursor.execute('''
        INSERT INTO readings VALUES (?, ?, ?, ?)
    ''', (
        datetime.now().isoformat(),
        data['sensorValue'],
        data['waterDetected'],
        data['uptime']
    ))
    
    conn.commit()
    conn.close()
```

## Testing

### Manual API Testing

**Using cURL:**
```bash
# Basic request
curl http://192.168.1.100/api/status

# Pretty print JSON
curl -s http://192.168.1.100/api/status | jq

# Check response time
curl -w "@curl-format.txt" -o /dev/null -s http://192.168.1.100/api/status

# Continuous monitoring
watch -n 2 'curl -s http://192.168.1.100/api/status | jq'
```

**Using Postman:**
1. Create new GET request
2. Enter URL: `http://192.168.1.100/api/status`
3. Send request
4. View formatted JSON response

**Using HTTPie:**
```bash
http GET http://192.168.1.100/api/status
```

## Troubleshooting

### API Not Responding

**Check:**
1. Arduino is powered on
2. Serial Monitor shows IP address
3. Ping Arduino: `ping 192.168.1.100`
4. Check WiFi connection
5. Verify firewall settings

### Incorrect Data

**Verify:**
1. Compare with Serial Monitor output
2. Check sensor connections
3. Calibrate sensor threshold
4. Review Arduino logs

### Slow Response

**Optimize:**
1. Reduce polling frequency
2. Check network latency
3. Minimize concurrent requests
4. Upgrade WiFi router

## Changelog

### Version 1.0.0 (Current)
- Initial API implementation
- JSON status endpoint
- HTML dashboard
- Real-time auto-refresh

### Planned Features
- [ ] WebSocket support
- [ ] MQTT integration
- [ ] Authentication
- [ ] Historical data endpoint
- [ ] Configuration API
- [ ] Firmware update API

---

**Questions?** Open an issue on GitHub or check the main README.
