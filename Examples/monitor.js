#!/usr/bin/env node
/**
 * Water Sensor Monitor - Node.js Integration Example
 * Polls the Arduino water sensor API and provides notifications
 */

const axios = require('axios');
const fs = require('fs');

// Configuration
const ARDUINO_IP = '192.168.1.100'; // Change to your Arduino's IP
const API_URL = `http://${ARDUINO_IP}/api/status`;
const POLL_INTERVAL = 5000; // milliseconds
const LOG_FILE = 'sensor_log.json';

class WaterSensorMonitor {
    constructor(arduinoIp) {
        this.apiUrl = `http://${arduinoIp}/api/status`;
        this.lastAlertCount = 0;
        this.consecutiveFailures = 0;
        this.maxFailures = 5;
        this.logData = [];
    }

    /**
     * Fetch current status from Arduino API
     */
    async fetchStatus() {
        try {
            const response = await axios.get(this.apiUrl, { timeout: 5000 });
            this.consecutiveFailures = 0;
            return response.data;
        } catch (error) {
            this.consecutiveFailures++;
            
            if (error.code === 'ECONNABORTED') {
                console.log('⚠️  Connection timeout - Arduino may be offline');
            } else if (error.code === 'ECONNREFUSED') {
                console.log('⚠️  Connection refused - Check Arduino IP and port');
            } else {
                console.log(`⚠️  Error: ${error.message}`);
            }
            
            return null;
        }
    }

    /**
     * Log sensor data to JSON file
     */
    logReading(data) {
        if (!data) return;

        const logEntry = {
            timestamp: new Date().toISOString(),
            ...data
        };

        this.logData.push(logEntry);

        // Write to file every 10 readings to reduce I/O
        if (this.logData.length % 10 === 0) {
            this.saveLog();
        }
    }

    /**
     * Save log data to file
     */
    saveLog() {
        try {
            fs.writeFileSync(LOG_FILE, JSON.stringify(this.logData, null, 2));
        } catch (error) {
            console.log(`⚠️  Error saving log: ${error.message}`);
        }
    }

    /**
     * Check if a new alert has occurred
     */
    checkNewAlert(data) {
        if (!data) return false;

        const currentAlerts = data.totalAlerts;
        if (currentAlerts > this.lastAlertCount) {
            this.lastAlertCount = currentAlerts;
            return true;
        }
        return false;
    }

    /**
     * Send notification for new alert
     */
    sendNotification(data) {
        console.log('\n' + '='.repeat(50));
        console.log('🚨 NEW WATER ALERT!');
        console.log('='.repeat(50));
        console.log(`Time: ${new Date().toLocaleString()}`);
        console.log(`Sensor Value: ${data.sensorValue}`);
        console.log(`Total Alerts: ${data.totalAlerts}`);
        console.log('='.repeat(50) + '\n');

        // Add your notification code here:
        // - Send email
        // - Send SMS
        // - Push notification
        // - Webhook
    }

    /**
     * Display current status to console
     */
    displayStatus(data) {
        if (!data) return;

        const status = data.waterDetected ? '🚨 WATER DETECTED' : '✅ All Clear';
        const value = data.sensorValue.toString().padStart(4);
        const alerts = data.totalAlerts.toString().padStart(3);
        const uptime = data.uptime.padStart(12);
        const signal = data.signalStrength.toString().padStart(4);

        console.log(
            `${status} | ` +
            `Value: ${value} | ` +
            `Alerts: ${alerts} | ` +
            `Uptime: ${uptime} | ` +
            `Signal: ${signal} dBm`
        );
    }

    /**
     * Check connection health
     */
    checkHealth() {
        if (this.consecutiveFailures >= this.maxFailures) {
            console.log(`\n⚠️  WARNING: ${this.consecutiveFailures} consecutive failures!`);
            console.log('Arduino may be offline or unreachable');
            console.log('Check power, WiFi, and network connectivity\n');
            return false;
        }
        return true;
    }

    /**
     * Main monitoring loop
     */
    async run(pollInterval = 5000) {
        console.log('='.repeat(60));
        console.log('Water Sensor Monitor - Node.js Integration');
        console.log('='.repeat(60));
        console.log(`Arduino IP: ${ARDUINO_IP}`);
        console.log(`Polling interval: ${pollInterval / 1000} seconds`);
        console.log(`Log file: ${LOG_FILE}`);
        console.log('='.repeat(60));
        console.log('\nPress Ctrl+C to stop\n');

        // Main loop
        const intervalId = setInterval(async () => {
            // Fetch current status
            const data = await this.fetchStatus();

            // Check connection health
            if (!this.checkHealth()) {
                return;
            }

            // Display status
            this.displayStatus(data);

            // Log data
            this.logReading(data);

            // Check for new alerts
            if (this.checkNewAlert(data)) {
                this.sendNotification(data);
            }
        }, pollInterval);

        // Handle graceful shutdown
        process.on('SIGINT', () => {
            clearInterval(intervalId);
            this.saveLog();
            console.log('\n\nMonitoring stopped by user');
            console.log(`Log saved to: ${LOG_FILE}`);
            process.exit(0);
        });
    }
}

// Main entry point
async function main() {
    // Check if custom IP provided
    const arduinoIp = process.argv[2] || ARDUINO_IP;

    // Create monitor and run
    const monitor = new WaterSensorMonitor(arduinoIp);
    await monitor.run(POLL_INTERVAL);
}

// Run if called directly
if (require.main === module) {
    main().catch(error => {
        console.error('Fatal error:', error);
        process.exit(1);
    });
}

module.exports = WaterSensorMonitor;
