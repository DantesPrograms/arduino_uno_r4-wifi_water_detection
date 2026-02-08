# Integration Examples

This directory contains example code for integrating with the Water Sensor Alert System API.

## Available Examples

### Python Monitor (`monitor.py`)

Real-time monitoring script with CSV logging.

**Features:**
- Polls Arduino API at configurable interval
- Logs all data to CSV file
- Console status display
- Alert detection and notifications
- Connection health monitoring
- Graceful error handling

**Requirements:**
```bash
pip install requests
```

**Usage:**
```bash
# Default IP (192.168.1.100)
python monitor.py

# Custom IP
python monitor.py 192.168.1.50
```

**Configuration:**
Edit the script to change:
- `ARDUINO_IP` - Arduino's IP address
- `POLL_INTERVAL` - Seconds between polls
- `LOG_FILE` - CSV log file path
- `ALERT_ENABLED` - Enable/disable notifications

**Output:**
```
✅ All Clear | Value:   45 | Alerts:   0 | Uptime:    1h 23m | Signal:  -52 dBm
```

### Node.js Monitor (`monitor.js`)

Similar functionality in Node.js with JSON logging.

**Features:**
- Asynchronous API polling
- JSON log format
- Alert notifications
- Connection monitoring
- Automatic log saves

**Requirements:**
```bash
npm install axios
```

**Usage:**
```bash
# Default IP
node monitor.js

# Custom IP
node monitor.js 192.168.1.50
```

**Configuration:**
Edit the script to change:
- `ARDUINO_IP` - Arduino's IP address
- `POLL_INTERVAL` - Milliseconds between polls
- `LOG_FILE` - JSON log file path

## Adding Notifications

### Email Notifications (Python)

Add to `send_notification()` function:

```python
import smtplib
from email.mime.text import MIMEText

def send_email_alert(data):
    msg = MIMEText(f"Water detected! Sensor: {data['sensorValue']}")
    msg['Subject'] = 'Water Sensor Alert'
    msg['From'] = 'sensor@example.com'
    msg['To'] = 'your@email.com'
    
    with smtplib.SMTP('smtp.gmail.com', 587) as server:
        server.starttls()
        server.login('username', 'app-password')
        server.send_message(msg)
```

### SMS Notifications (Python via Twilio)

```python
from twilio.rest import Client

def send_sms_alert(data):
    client = Client('ACCOUNT_SID', 'AUTH_TOKEN')
    message = client.messages.create(
        body=f"Water detected! Value: {data['sensorValue']}",
        from_='+1234567890',
        to='+0987654321'
    )
```

### Webhook Notifications (Node.js)

```javascript
async sendWebhook(data) {
    await axios.post('https://webhook.site/your-id', {
        event: 'water_detected',
        sensorValue: data.sensorValue,
        timestamp: new Date().toISOString()
    });
}
```

## Database Integration

### SQLite (Python)

```python
import sqlite3

def setup_database():
    conn = sqlite3.connect('sensor_data.db')
    cursor = conn.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS readings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT,
            sensor_value INTEGER,
            water_detected BOOLEAN,
            uptime TEXT
        )
    ''')
    conn.commit()
    conn.close()

def log_to_database(data):
    conn = sqlite3.connect('sensor_data.db')
    cursor = conn.cursor()
    cursor.execute('''
        INSERT INTO readings (timestamp, sensor_value, water_detected, uptime)
        VALUES (?, ?, ?, ?)
    ''', (
        datetime.now().isoformat(),
        data['sensorValue'],
        data['waterDetected'],
        data['uptime']
    ))
    conn.commit()
    conn.close()
```

### PostgreSQL (Node.js)

```javascript
const { Pool } = require('pg');

const pool = new Pool({
    host: 'localhost',
    database: 'sensor_db',
    user: 'postgres',
    password: 'password'
});

async function logToDatabase(data) {
    await pool.query(
        'INSERT INTO readings (timestamp, sensor_value, water_detected) VALUES ($1, $2, $3)',
        [new Date(), data.sensorValue, data.waterDetected]
    );
}
```

## Home Automation Integration

### Home Assistant YAML

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

automation:
  - alias: "Water Detection Alert"
    trigger:
      - platform: state
        entity_id: sensor.water_sensor
        attribute: waterDetected
        to: true
    action:
      - service: notify.mobile_app
        data:
          message: "Water detected!"
```

### IFTTT Webhook

```python
import requests

def send_ifttt_notification(data):
    webhook_url = "https://maker.ifttt.com/trigger/water_alert/with/key/YOUR_KEY"
    requests.post(webhook_url, json={
        'value1': data['sensorValue'],
        'value2': data['totalAlerts'],
        'value3': datetime.now().isoformat()
    })
```

## Data Visualization

### Matplotlib (Python)

```python
import matplotlib.pyplot as plt
import pandas as pd

# Read CSV log
df = pd.read_csv('sensor_log.csv')
df['Timestamp'] = pd.to_datetime(df['Timestamp'])

# Plot sensor values over time
plt.figure(figsize=(12, 6))
plt.plot(df['Timestamp'], df['Sensor Value'])
plt.axhline(y=300, color='r', linestyle='--', label='Threshold')
plt.xlabel('Time')
plt.ylabel('Sensor Value')
plt.title('Water Sensor Readings')
plt.legend()
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig('sensor_plot.png')
```

### Chart.js (Web)

```html
<!DOCTYPE html>
<html>
<head>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
    <canvas id="sensorChart"></canvas>
    <script>
        async function fetchAndPlot() {
            const response = await fetch('http://192.168.1.100/api/status');
            const data = await response.json();
            
            // Update chart with new data point
            chart.data.labels.push(new Date().toLocaleTimeString());
            chart.data.datasets[0].data.push(data.sensorValue);
            chart.update();
        }
        
        const ctx = document.getElementById('sensorChart').getContext('2d');
        const chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: 'Sensor Value',
                    data: [],
                    borderColor: 'rgb(75, 192, 192)',
                }]
            }
        });
        
        setInterval(fetchAndPlot, 2000);
    </script>
</body>
</html>
```

## Testing Examples

### Simple Test Script (Bash)

```bash
#!/bin/bash
# test_api.sh

ARDUINO_IP="192.168.1.100"

echo "Testing Arduino API..."
curl -s "http://${ARDUINO_IP}/api/status" | jq

echo -e "\nSensor Value:"
curl -s "http://${ARDUINO_IP}/api/status" | jq -r '.sensorValue'

echo -e "\nWater Detected:"
curl -s "http://${ARDUINO_IP}/api/status" | jq -r '.waterDetected'
```

### Pytest Unit Tests (Python)

```python
import pytest
import requests

ARDUINO_IP = "192.168.1.100"
API_URL = f"http://{ARDUINO_IP}/api/status"

def test_api_response():
    """Test API returns valid response"""
    response = requests.get(API_URL, timeout=5)
    assert response.status_code == 200

def test_json_format():
    """Test API returns valid JSON"""
    response = requests.get(API_URL, timeout=5)
    data = response.json()
    assert isinstance(data, dict)

def test_required_fields():
    """Test all required fields are present"""
    response = requests.get(API_URL, timeout=5)
    data = response.json()
    
    required_fields = [
        'waterDetected', 'sensorValue', 'alertActive',
        'totalAlerts', 'lastAlert', 'uptime', 'signalStrength'
    ]
    
    for field in required_fields:
        assert field in data

def test_sensor_value_range():
    """Test sensor value is in valid range"""
    response = requests.get(API_URL, timeout=5)
    data = response.json()
    assert 0 <= data['sensorValue'] <= 1023

def test_boolean_fields():
    """Test boolean fields are actually booleans"""
    response = requests.get(API_URL, timeout=5)
    data = response.json()
    assert isinstance(data['waterDetected'], bool)
    assert isinstance(data['alertActive'], bool)
```

## Performance Monitoring

### Response Time Checker (Python)

```python
import requests
import time
import statistics

def measure_response_times(count=100):
    times = []
    for _ in range(count):
        start = time.time()
        requests.get(API_URL, timeout=5)
        elapsed = (time.time() - start) * 1000  # Convert to ms
        times.append(elapsed)
        time.sleep(0.1)
    
    print(f"Requests: {count}")
    print(f"Average: {statistics.mean(times):.2f} ms")
    print(f"Median: {statistics.median(times):.2f} ms")
    print(f"Min: {min(times):.2f} ms")
    print(f"Max: {max(times):.2f} ms")
    print(f"Std Dev: {statistics.stdev(times):.2f} ms")
```

## Contributing Examples

Have a useful integration? Submit a pull request!

See [CONTRIBUTING.md](../CONTRIBUTING.md) for guidelines.

## Support

Questions about these examples? Open an issue on GitHub.
