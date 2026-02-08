/*
 * Water Sensor Alert System
 * Arduino UNO R4 WiFi
 * 
 * This sketch monitors a water sensor and provides:
 * - Web server with real-time status
 * - Visual alert (Red LED)
 * - Audio alert (Active Buzzer)
 * - Alert history tracking
 * 
 * Hardware Connections:
 * - A0: Water Sensor Signal
 * - D8: Red LED (with 220Ω resistor)
 * - D9: Active Buzzer
 */

#include <WiFiS3.h>

// ========== CONFIGURATION ==========
// WiFi Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Pin Definitions
const int WATER_SENSOR_PIN = A0;
const int RED_LED_PIN = 8;
const int BUZZER_PIN = 9;

// Sensor Thresholds
const int WATER_THRESHOLD = 300;  // Adjust based on your sensor (0-1023)
const int DRY_THRESHOLD = 100;    // Hysteresis to prevent flickering

// Alert Settings
const unsigned long BUZZER_DURATION = 5000;  // 5 seconds
const unsigned long ALERT_COOLDOWN = 30000;  // 30 seconds between alerts

// ========== GLOBAL VARIABLES ==========
WiFiServer server(80);

// Sensor State
int sensorValue = 0;
bool waterDetected = false;
bool alertActive = false;
unsigned long alertStartTime = 0;
unsigned long lastAlertTime = 0;

// Statistics
unsigned long totalAlerts = 0;
String lastAlertTimestamp = "Never";
unsigned long uptimeSeconds = 0;
unsigned long lastUptimeUpdate = 0;

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(WATER_SENSOR_PIN, INPUT);
  
  // Turn off alert components
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  Serial.println("\n=== Water Sensor Alert System ===");
  Serial.println("Starting up...");
  Serial.println("Firmware Version: 1.0.0");
  
  // Connect to WiFi
  connectToWiFi();
  
  // Only start server if WiFi connected AND has valid IP
  if (WiFi.status() == WL_CONNECTED && WiFi.localIP() != IPAddress(0, 0, 0, 0)) {
    server.begin();
    Serial.println("\n✓ Web server started successfully!");
    Serial.println("================================");
    Serial.print("Access dashboard at: http://");
    Serial.println(WiFi.localIP());
    Serial.print("API endpoint: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/api/status");
    Serial.println("================================\n");
  } else {
    Serial.println("\n⚠️ Web server NOT started (no WiFi connection)");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi reports connected but no IP address received!");
      Serial.println("Possible causes:");
      Serial.println("  - DHCP server not responding");
      Serial.println("  - Router DHCP pool exhausted");
      Serial.println("  - MAC address blocked");
    }
    Serial.println("Sensor monitoring will continue in offline mode\n");
  }
}

// ========== MAIN LOOP ==========
void loop() {
  // Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi disconnected! Attempting to reconnect...");
    connectToWiFi();
  }
  
  updateUptime();
  readWaterSensor();
  handleAlerts();
  handleWebServer();
  
  delay(100);  // Small delay to prevent overwhelming the system
}

// ========== WIFI FUNCTIONS ==========
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  Serial.print("Password length: ");
  Serial.println(strlen(password));
  
  // Disconnect if previously connected
  WiFi.disconnect();
  delay(100);
  
  // Begin WiFi connection
  int status = WiFi.begin(ssid, password);
  Serial.print("WiFi.begin() returned: ");
  Serial.println(status);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 60) {
    delay(500);
    Serial.print(".");
    attempts++;
    
    // Print status every 5 attempts
    if (attempts % 5 == 0) {
      Serial.print(" [Status: ");
      Serial.print(WiFi.status());
      Serial.print("]");
    }
    
    // Try to reconnect every 15 attempts
    if (attempts % 15 == 0 && attempts < 60) {
      Serial.println("\nRetrying connection...");
      Serial.print("Current status: ");
      printWiFiStatus(WiFi.status());
      WiFi.disconnect();
      delay(1000);
      WiFi.begin(ssid, password);
    }
  }
  
  Serial.println(); // New line after dots
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi connected successfully!");
    Serial.println("Network Information:");
    Serial.print("  SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("  IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("  Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("  Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("  Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    
    // Get MAC address (R4 WiFi requires buffer)
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("  MAC Address: ");
    for (int i = 0; i < 6; i++) {
      if (mac[i] < 16) Serial.print("0");
      Serial.print(mac[i], HEX);
      if (i < 5) Serial.print(":");
    }
    Serial.println();
  } else {
    Serial.println("\n✗ Failed to connect to WiFi!");
    Serial.print("Final status code: ");
    Serial.print(WiFi.status());
    Serial.print(" - ");
    printWiFiStatus(WiFi.status());
    
    Serial.println("\nTroubleshooting steps:");
    Serial.println("  1. Check SSID: '" + String(ssid) + "' is correct and visible");
    Serial.println("  2. Verify password is correct (case-sensitive)");
    Serial.println("  3. Ensure router broadcasts 2.4GHz WiFi (not 5GHz only)");
    Serial.println("  4. Check router security: use WPA2 or WPA/WPA2 mixed");
    Serial.println("  5. Disable MAC filtering or add: 34:B7:DA:5D:7C:A0");
    Serial.println("  6. Check DHCP is enabled on router");
    Serial.println("  7. Try restarting your router");
    
    // Get MAC address even on failure
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("  Your MAC Address: ");
    for (int i = 0; i < 6; i++) {
      if (mac[i] < 16) Serial.print("0");
      Serial.print(mac[i], HEX);
      if (i < 5) Serial.print(":");
    }
    Serial.println();
    
    Serial.println("\nThe system will continue without WiFi...");
  }
}

void printWiFiStatus(int status) {
  switch(status) {
    case WL_CONNECTED:
      Serial.println("Connected");
      break;
    case WL_NO_SHIELD:
      Serial.println("No WiFi shield");
      break;
    case WL_IDLE_STATUS:
      Serial.println("Idle/Scanning");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("SSID not found - Check network name!");
      break;
    case WL_SCAN_COMPLETED:
      Serial.println("Scan completed");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("Connection failed - Check password!");
      break;
    case WL_CONNECTION_LOST:
      Serial.println("Connection lost");
      break;
    case WL_DISCONNECTED:
      Serial.println("Disconnected");
      break;
    default:
      Serial.print("Unknown (");
      Serial.print(status);
      Serial.println(")");
  }
}

// ========== SENSOR FUNCTIONS ==========
void readWaterSensor() {
  sensorValue = analogRead(WATER_SENSOR_PIN);
  
  // Hysteresis logic to prevent flickering
  if (!waterDetected && sensorValue > WATER_THRESHOLD) {
    waterDetected = true;
    triggerAlert();
    Serial.println("🚨 WATER DETECTED! 🚨");
    Serial.print("Sensor value: ");
    Serial.println(sensorValue);
  } else if (waterDetected && sensorValue < DRY_THRESHOLD) {
    waterDetected = false;
    Serial.println("✓ Water cleared");
    Serial.print("Sensor value: ");
    Serial.println(sensorValue);
  }
}

// ========== ALERT FUNCTIONS ==========
void triggerAlert() {
  unsigned long currentTime = millis();
  
  // Check cooldown period
  if (currentTime - lastAlertTime < ALERT_COOLDOWN) {
    Serial.println("Alert cooldown active, skipping...");
    return;
  }
  
  alertActive = true;
  alertStartTime = currentTime;
  lastAlertTime = currentTime;
  totalAlerts++;
  
  // Generate timestamp
  lastAlertTimestamp = getTimeString();
  
  // Activate alert components
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  
  Serial.println("🔴 Alert triggered!");
  Serial.print("Total alerts: ");
  Serial.println(totalAlerts);
}

void handleAlerts() {
  if (alertActive) {
    unsigned long currentTime = millis();
    
    // Turn off buzzer after duration (keep LED on while water detected)
    if (currentTime - alertStartTime > BUZZER_DURATION) {
      digitalWrite(BUZZER_PIN, LOW);
    }
    
    // Turn off LED if water is no longer detected
    if (!waterDetected) {
      digitalWrite(RED_LED_PIN, LOW);
      alertActive = false;
      Serial.println("🟢 Alert cleared");
    }
  }
}

// ========== UTILITY FUNCTIONS ==========
void updateUptime() {
  unsigned long currentTime = millis();
  if (currentTime - lastUptimeUpdate >= 1000) {
    uptimeSeconds++;
    lastUptimeUpdate = currentTime;
  }
}

String getTimeString() {
  unsigned long seconds = uptimeSeconds;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  
  seconds %= 60;
  minutes %= 60;
  
  String timeStr = "";
  if (hours < 10) timeStr += "0";
  timeStr += String(hours) + ":";
  if (minutes < 10) timeStr += "0";
  timeStr += String(minutes) + ":";
  if (seconds < 10) timeStr += "0";
  timeStr += String(seconds);
  
  return timeStr;
}

String getUptimeString() {
  unsigned long seconds = uptimeSeconds;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  
  String uptime = "";
  if (days > 0) uptime += String(days) + "d ";
  if (hours > 0) uptime += String(hours) + "h ";
  if (minutes > 0) uptime += String(minutes) + "m ";
  uptime += String(seconds) + "s";
  
  return uptime;
}

// ========== WEB SERVER FUNCTIONS ==========
void handleWebServer() {
  WiFiClient client = server.available();
  
  if (client) {
    String request = "";
    bool currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        
        if (c == '\n' && currentLineIsBlank) {
          // Send HTTP response
          if (request.indexOf("GET /api/status") >= 0) {
            sendJSONResponse(client);
          } else {
            sendHTMLResponse(client);
          }
          break;
        }
        
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    
    delay(1);
    client.stop();
  }
}

void sendJSONResponse(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  
  client.print("{");
  client.print("\"waterDetected\":");
  client.print(waterDetected ? "true" : "false");
  client.print(",\"sensorValue\":");
  client.print(sensorValue);
  client.print(",\"alertActive\":");
  client.print(alertActive ? "true" : "false");
  client.print(",\"totalAlerts\":");
  client.print(totalAlerts);
  client.print(",\"lastAlert\":\"");
  client.print(lastAlertTimestamp);
  client.print("\",\"uptime\":\"");
  client.print(getUptimeString());
  client.print("\",\"signalStrength\":");
  client.print(WiFi.RSSI());
  client.println("}");
}

void sendHTMLResponse(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  
  // HTML content
  client.println("<!DOCTYPE html>");
  client.println("<html lang='en'>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<title>Water Sensor Monitor</title>");
  client.println("<style>");
  
  // CSS Styling
  client.println("* { margin: 0; padding: 0; box-sizing: border-box; }");
  client.println("body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; padding: 20px; }");
  client.println(".container { max-width: 800px; margin: 0 auto; }");
  client.println(".header { text-align: center; color: white; margin-bottom: 30px; }");
  client.println(".header h1 { font-size: 2.5em; margin-bottom: 10px; text-shadow: 2px 2px 4px rgba(0,0,0,0.3); }");
  client.println(".header p { font-size: 1.1em; opacity: 0.9; }");
  client.println(".card { background: white; border-radius: 15px; padding: 30px; margin-bottom: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.2); }");
  client.println(".status-indicator { display: flex; align-items: center; justify-content: center; padding: 40px; border-radius: 10px; margin-bottom: 30px; transition: all 0.3s; }");
  client.println(".status-safe { background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); }");
  client.println(".status-alert { background: linear-gradient(135deg, #eb3349 0%, #f45c43 100%); animation: pulse 1.5s infinite; }");
  client.println("@keyframes pulse { 0%, 100% { transform: scale(1); } 50% { transform: scale(1.05); } }");
  client.println(".status-icon { font-size: 4em; margin-right: 20px; }");
  client.println(".status-text { color: white; }");
  client.println(".status-text h2 { font-size: 2em; margin-bottom: 5px; }");
  client.println(".status-text p { font-size: 1.2em; opacity: 0.9; }");
  client.println(".metrics { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin-bottom: 30px; }");
  client.println(".metric { background: #f8f9fa; padding: 20px; border-radius: 10px; text-align: center; }");
  client.println(".metric-label { color: #6c757d; font-size: 0.9em; margin-bottom: 8px; text-transform: uppercase; letter-spacing: 1px; }");
  client.println(".metric-value { color: #333; font-size: 1.8em; font-weight: bold; }");
  client.println(".info-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 15px; }");
  client.println(".info-item { display: flex; justify-content: space-between; padding: 15px; background: #f8f9fa; border-radius: 8px; }");
  client.println(".info-label { color: #6c757d; font-weight: 500; }");
  client.println(".info-value { color: #333; font-weight: bold; }");
  client.println(".footer { text-align: center; color: white; margin-top: 30px; opacity: 0.8; font-size: 0.9em; }");
  client.println(".loading { text-align: center; color: white; font-size: 1.2em; margin-top: 50px; }");
  
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  
  client.println("<div class='container'>");
  client.println("<div class='header'>");
  client.println("<h1>💧 Water Sensor Monitor</h1>");
  client.println("<p>Real-time water detection system</p>");
  client.println("</div>");
  
  client.println("<div class='card'>");
  client.println("<div id='statusIndicator' class='status-indicator'>");
  client.println("<div class='loading'>Loading...</div>");
  client.println("</div>");
  
  client.println("<div class='metrics'>");
  client.println("<div class='metric'>");
  client.println("<div class='metric-label'>Sensor Reading</div>");
  client.println("<div class='metric-value' id='sensorValue'>--</div>");
  client.println("</div>");
  client.println("<div class='metric'>");
  client.println("<div class='metric-label'>Total Alerts</div>");
  client.println("<div class='metric-value' id='totalAlerts'>--</div>");
  client.println("</div>");
  client.println("<div class='metric'>");
  client.println("<div class='metric-label'>Last Alert</div>");
  client.println("<div class='metric-value' id='lastAlert' style='font-size: 1.2em;'>--</div>");
  client.println("</div>");
  client.println("</div>");
  
  client.println("<div class='info-grid'>");
  client.println("<div class='info-item'>");
  client.println("<span class='info-label'>System Uptime</span>");
  client.println("<span class='info-value' id='uptime'>--</span>");
  client.println("</div>");
  client.println("<div class='info-item'>");
  client.println("<span class='info-label'>WiFi Signal</span>");
  client.println("<span class='info-value' id='signal'>--</span>");
  client.println("</div>");
  client.println("<div class='info-item'>");
  client.println("<span class='info-label'>Alert Status</span>");
  client.println("<span class='info-value' id='alertStatus'>--</span>");
  client.println("</div>");
  client.println("<div class='info-item'>");
  client.println("<span class='info-label'>Threshold</span>");
  client.println("<span class='info-value'>" + String(WATER_THRESHOLD) + "</span>");
  client.println("</div>");
  client.println("</div>");
  client.println("</div>");
  
  client.println("<div class='footer'>");
  client.println("<p>Arduino UNO R4 WiFi | Auto-refresh every 2 seconds</p>");
  client.println("</div>");
  client.println("</div>");
  
  // JavaScript for auto-refresh
  client.println("<script>");
  client.println("function updateStatus() {");
  client.println("  fetch('/api/status')");
  client.println("    .then(response => response.json())");
  client.println("    .then(data => {");
  client.println("      const indicator = document.getElementById('statusIndicator');");
  client.println("      if (data.waterDetected) {");
  client.println("        indicator.className = 'status-indicator status-alert';");
  client.println("        indicator.innerHTML = '<div class=\"status-icon\">🚨</div><div class=\"status-text\"><h2>WATER DETECTED!</h2><p>Alert Active</p></div>';");
  client.println("      } else {");
  client.println("        indicator.className = 'status-indicator status-safe';");
  client.println("        indicator.innerHTML = '<div class=\"status-icon\">✅</div><div class=\"status-text\"><h2>All Clear</h2><p>No water detected</p></div>';");
  client.println("      }");
  client.println("      document.getElementById('sensorValue').textContent = data.sensorValue;");
  client.println("      document.getElementById('totalAlerts').textContent = data.totalAlerts;");
  client.println("      document.getElementById('lastAlert').textContent = data.lastAlert;");
  client.println("      document.getElementById('uptime').textContent = data.uptime;");
  client.println("      document.getElementById('signal').textContent = data.signalStrength + ' dBm';");
  client.println("      document.getElementById('alertStatus').textContent = data.alertActive ? 'Active' : 'Inactive';");
  client.println("    })");
  client.println("    .catch(error => console.error('Error:', error));");
  client.println("}");
  client.println("updateStatus();");
  client.println("setInterval(updateStatus, 2000);");
  client.println("</script>");
  
  client.println("</body>");
  client.println("</html>");
}
