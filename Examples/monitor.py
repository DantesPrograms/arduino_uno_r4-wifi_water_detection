#!/usr/bin/env python3
"""
Water Sensor Monitor - Python Integration Example
Polls the Arduino water sensor API and logs data
"""

import requests
import time
import json
import csv
from datetime import datetime
import sys

# Configuration
ARDUINO_IP = "192.168.1.100"  # Change to your Arduino's IP
API_URL = f"http://{ARDUINO_IP}/api/status"
POLL_INTERVAL = 5  # seconds
LOG_FILE = "sensor_log.csv"
ALERT_ENABLED = True


class WaterSensorMonitor:
    """Monitor Arduino water sensor via API"""
    
    def __init__(self, arduino_ip):
        self.api_url = f"http://{arduino_ip}/api/status"
        self.last_alert_count = 0
        self.consecutive_failures = 0
        self.max_failures = 5
        
        # Initialize CSV log file
        self.init_log_file()
    
    def init_log_file(self):
        """Create CSV log file with headers if it doesn't exist"""
        try:
            with open(LOG_FILE, 'x', newline='') as f:
                writer = csv.writer(f)
                writer.writerow([
                    'Timestamp',
                    'Water Detected',
                    'Sensor Value',
                    'Alert Active',
                    'Total Alerts',
                    'Last Alert',
                    'Uptime',
                    'Signal Strength'
                ])
            print(f"Created log file: {LOG_FILE}")
        except FileExistsError:
            print(f"Using existing log file: {LOG_FILE}")
    
    def fetch_status(self):
        """Fetch current status from Arduino API"""
        try:
            response = requests.get(self.api_url, timeout=5)
            response.raise_for_status()
            
            data = response.json()
            self.consecutive_failures = 0
            return data
            
        except requests.exceptions.Timeout:
            print("⚠️  Connection timeout - Arduino may be offline")
            self.consecutive_failures += 1
            return None
            
        except requests.exceptions.ConnectionError:
            print("⚠️  Connection error - Check network and Arduino")
            self.consecutive_failures += 1
            return None
            
        except requests.exceptions.RequestException as e:
            print(f"⚠️  Request error: {e}")
            self.consecutive_failures += 1
            return None
            
        except json.JSONDecodeError:
            print("⚠️  Invalid JSON response")
            self.consecutive_failures += 1
            return None
    
    def log_data(self, data):
        """Log sensor data to CSV file"""
        if data is None:
            return
        
        try:
            with open(LOG_FILE, 'a', newline='') as f:
                writer = csv.writer(f)
                writer.writerow([
                    datetime.now().isoformat(),
                    data['waterDetected'],
                    data['sensorValue'],
                    data['alertActive'],
                    data['totalAlerts'],
                    data['lastAlert'],
                    data['uptime'],
                    data['signalStrength']
                ])
        except Exception as e:
            print(f"⚠️  Logging error: {e}")
    
    def check_new_alert(self, data):
        """Check if a new alert has occurred"""
        if data is None:
            return False
        
        current_alerts = data['totalAlerts']
        if current_alerts > self.last_alert_count:
            self.last_alert_count = current_alerts
            return True
        return False
    
    def send_notification(self, data):
        """Send notification for new alert (placeholder)"""
        print("\n" + "=" * 50)
        print("🚨 NEW WATER ALERT!")
        print("=" * 50)
        print(f"Time: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print(f"Sensor Value: {data['sensorValue']}")
        print(f"Total Alerts: {data['totalAlerts']}")
        print("=" * 50 + "\n")
        
        # Add your notification code here:
        # - Send email
        # - Send SMS via Twilio
        # - Push notification
        # - Webhook to other service
    
    def display_status(self, data):
        """Display current status to console"""
        if data is None:
            return
        
        # Status symbol
        status = "🚨 WATER DETECTED" if data['waterDetected'] else "✅ All Clear"
        
        # Build status line
        print(f"{status} | "
              f"Value: {data['sensorValue']:4d} | "
              f"Alerts: {data['totalAlerts']:3d} | "
              f"Uptime: {data['uptime']:>12s} | "
              f"Signal: {data['signalStrength']:4d} dBm")
    
    def check_health(self):
        """Check if connection to Arduino is healthy"""
        if self.consecutive_failures >= self.max_failures:
            print(f"\n⚠️  WARNING: {self.consecutive_failures} consecutive failures!")
            print("Arduino may be offline or unreachable")
            print("Check power, WiFi, and network connectivity\n")
            return False
        return True
    
    def run(self, poll_interval=5):
        """Main monitoring loop"""
        print("=" * 60)
        print("Water Sensor Monitor - Python Integration")
        print("=" * 60)
        print(f"Arduino IP: {ARDUINO_IP}")
        print(f"Polling interval: {poll_interval} seconds")
        print(f"Log file: {LOG_FILE}")
        print("=" * 60)
        print("\nPress Ctrl+C to stop\n")
        
        try:
            while True:
                # Fetch current status
                data = self.fetch_status()
                
                # Check connection health
                if not self.check_health():
                    time.sleep(poll_interval * 2)  # Wait longer on failures
                    continue
                
                # Display status
                self.display_status(data)
                
                # Log data
                self.log_data(data)
                
                # Check for new alerts
                if ALERT_ENABLED and self.check_new_alert(data):
                    self.send_notification(data)
                
                # Wait before next poll
                time.sleep(poll_interval)
                
        except KeyboardInterrupt:
            print("\n\nMonitoring stopped by user")
            print(f"Log saved to: {LOG_FILE}")
            sys.exit(0)


def main():
    """Main entry point"""
    # Check if custom IP provided
    if len(sys.argv) > 1:
        arduino_ip = sys.argv[1]
    else:
        arduino_ip = ARDUINO_IP
    
    # Create monitor and run
    monitor = WaterSensorMonitor(arduino_ip)
    monitor.run(poll_interval=POLL_INTERVAL)


if __name__ == "__main__":
    main()
