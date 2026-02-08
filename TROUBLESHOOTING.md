# 🔧 Troubleshooting Guide

Complete troubleshooting guide for the Water Sensor Alert System.

## WiFi Issues

### IP Address is 0.0.0.0

**Symptoms:**
- Serial Monitor shows `IP address: 0.0.0.0` or `(IP unset)`
- Cannot access web interface
- WiFi fails to connect

**Root Cause:** Arduino is not successfully connecting to WiFi network.

#### Solution 1: Verify Credentials

**Check your code:**
```cpp
const char* ssid = "YOUR_WIFI_SSID";       // Must match exactly (case-sensitive)
const char* password = "YOUR_WIFI_PASSWORD"; // Check for typos
```

**Common mistakes:**
- Extra spaces: `"MyNetwork "` instead of `"MyNetwork"`
- Case sensitivity: `"MyNetwork"` vs `"mynetwork"`
- Special characters in password not escaped
- Using 5GHz network name instead of 2.4GHz

**Test:**
1. Take a photo of your WiFi settings on another device
2. Type credentials exactly as shown
3. Use Serial.println() to print credentials (remove before sharing code)

#### Solution 2: Check WiFi Frequency

**Problem:** Arduino R4 WiFi only supports 2.4GHz networks

**Steps:**
1. Log into your router admin panel
2. Check if you have separate 2.4GHz and 5GHz networks
3. Common setups:
   - Single network name with band steering (may cause issues)
   - Separate networks: "MyNetwork-2.4" and "MyNetwork-5G"
   - 5GHz only (won't work)

**Solutions:**
- Connect to the 2.4GHz network specifically
- Disable band steering in router
- Create a dedicated 2.4GHz SSID

**Router Settings:**
- **TP-Link:** Wireless → Wireless Settings → Enable 2.4GHz
- **Netgear:** Advanced → Wireless Settings → 2.4GHz
- **ASUS:** Wireless → Professional → Band selection
- **Linksys:** Wireless → Basic Settings → Network Mode

#### Solution 3: Router Security Settings

**Compatible security types:**
- ✅ WPA2-PSK (recommended)
- ✅ WPA/WPA2 Mixed Mode
- ✅ WPA2-Personal
- ⚠️ WPA3 (may have issues)
- ❌ WPA3-only (usually won't work)
- ❌ WEP (too old)
- ⚠️ Open/No security (works but unsafe)

**Change security type:**
1. Access router admin panel
2. Go to Wireless Security settings
3. Set to WPA2-PSK or WPA/WPA2 Mixed
4. Save and reboot router
5. Try Arduino connection again

#### Solution 4: Signal Strength

**Check signal:**
- Move Arduino within 10-15 feet of router
- Avoid obstacles (walls, metal objects)
- Keep away from microwaves, cordless phones

**Test signal:**
- Use phone WiFi analyzer app
- Signal should be > -70 dBm for reliable connection
- If weak, try:
  - Moving router
  - Using WiFi extender
  - Upgrading router antenna

#### Solution 5: MAC Address Filtering

**Check if enabled:**
1. Log into router admin
2. Look for "MAC Filtering" or "Access Control"
3. If enabled, add Arduino's MAC address

**Get Arduino MAC address:**
Add this to setup():
```cpp
byte mac[6];
WiFi.macAddress(mac);
Serial.print("MAC Address: ");
for (int i = 0; i < 6; i++) {
  if (mac[i] < 16) Serial.print("0");
  Serial.print(mac[i], HEX);
  if (i < 5) Serial.print(":");
}
Serial.println();
```

**Whitelist MAC:**
1. Copy the MAC address (format: AA:BB:CC:DD:EE:FF)
2. Add to router's allowed devices list
3. Save settings

#### Solution 6: Router Connection Limits

**Problem:** Too many devices connected

**Check:**
- Some routers limit simultaneous connections (often 10-32 devices)
- Disconnect unused devices
- Reboot router to clear stale connections

#### Solution 7: DHCP Issues

**Problem:** Router not assigning IP address

**Solutions:**

**Option A: Extend DHCP pool**
1. Router admin → DHCP Settings
2. Increase address pool size
3. Save and reboot

**Option B: Static IP (Advanced)**
```cpp
// Add this before WiFi.begin()
IPAddress staticIP(192, 168, 1, 100);  // Choose unused IP
IPAddress gateway(192, 168, 1, 1);     // Your router IP
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);             // Google DNS

WiFi.config(staticIP, dns, gateway, subnet);
WiFi.begin(ssid, password);
```

**Find your network settings:**
- Windows: `ipconfig` in Command Prompt
- Mac: System Preferences → Network → Advanced → TCP/IP
- Linux: `ip addr show` or `ifconfig`

#### Solution 8: Firmware Issues

**Try:**
1. Update Arduino IDE to latest version
2. Update Arduino UNO R4 board package
3. Re-upload sketch with "Erase All Flash Before Sketch Upload" enabled

**Steps:**
- Tools → Board → Boards Manager
- Search "Arduino UNO R4"
- Update if available

### WiFi Connects but Loses Connection

**Symptoms:**
- Initially connects, gets valid IP
- Later shows 0.0.0.0
- Intermittent connectivity

**Solutions:**

#### 1. Power Issues

### WiFi Shows Connected but IP is 0.0.0.0

**Symptoms:**
- Signal strength shows good value (e.g., -42 dBm)
- WiFi status shows connected
- But IP address is 0.0.0.0
- This means WiFi is associated but DHCP failed

**This is a DHCP (IP assignment) issue, not WiFi connection issue.**

**Solutions:**

#### 1. Check DHCP Server
- Log into router admin panel
- Verify DHCP is enabled
- Check DHCP address range/pool
- Look for "DHCP Server" setting

**Example settings:**
- Start IP: 192.168.1.100
- End IP: 192.168.1.200
- Lease time: 24 hours

#### 2. MAC Address Filtering/Blocking
**Your MAC: 34:B7:DA:5D:7C:A0**

Check if router is blocking unknown devices:
1. Router admin → Security or Access Control
2. Look for MAC filtering, Access Control List (ACL)
3. If enabled, add your Arduino's MAC address
4. Or temporarily disable MAC filtering to test

#### 3. DHCP Pool Exhausted
- Too many devices connected
- DHCP address pool full
- Solutions:
  - Disconnect unused devices
  - Expand DHCP pool range in router
  - Reboot router to clear lease table

#### 4. Static IP Reservation Conflict
- Check if 192.168.1.x is reserved for another device
- Free up addresses in DHCP pool
- Reserve a specific IP for Arduino's MAC

#### 5. Use Static IP (Workaround)
If DHCP keeps failing, assign static IP:

```cpp
// Add BEFORE WiFi.begin() in connectToWiFi()
IPAddress staticIP(192, 168, 1, 150);     // Choose unused IP
IPAddress gateway(192, 168, 1, 1);         // Your router IP  
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

WiFi.config(staticIP, dns, gateway, subnet);
WiFi.begin(ssid, password);
```

**To find available IP:**
- Check router DHCP range (e.g., 192.168.1.100-200)
- Pick IP outside that range (e.g., 192.168.1.50)
- Or reserve IP within range for Arduino's MAC

**To find your gateway:**
- Windows: `ipconfig` → Default Gateway
- Mac: System Preferences → Network → Router
- Linux: `ip route | grep default`

#### 6. Router Firewall
- Some routers block new devices by default
- Check "Device Isolation" or "Client Isolation"
- Disable temporarily to test
- Add Arduino to allowed devices list

#### 7. Network Congestion
- Reboot router (unplug 30 seconds)
- Wait for router to fully boot (2-3 minutes)
- Then power on Arduino

### WiFi Connects but Loses Connection

**Symptoms:**
- Initially connects, gets valid IP
- Later shows 0.0.0.0
- Intermittent connectivity

**Solutions:**

#### 1. Power Issues
- Use quality USB cable
- Use 5V 2A power supply (not computer USB)
- Add capacitor between 5V and GND (100µF)

#### 2. Code Issues
- The updated sketch now auto-reconnects
- Check Serial Monitor for reconnection attempts

#### 3. Router Issues
- Enable "Keep Alive" in router settings
- Disable "Power Save Mode" on router WiFi
- Increase DHCP lease time

### Detailed Debug Output

Add this function to get more WiFi diagnostics:

```cpp
void printWiFiDiagnostics() {
  Serial.println("\n=== WiFi Diagnostics ===");
  
  Serial.print("Status: ");
  switch(WiFi.status()) {
    case WL_CONNECTED:
      Serial.println("Connected");
      break;
    case WL_NO_SHIELD:
      Serial.println("No WiFi shield");
      break;
    case WL_IDLE_STATUS:
      Serial.println("Idle");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("SSID not available");
      break;
    case WL_SCAN_COMPLETED:
      Serial.println("Scan completed");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("Connection failed");
      break;
    case WL_CONNECTION_LOST:
      Serial.println("Connection lost");
      break;
    case WL_DISCONNECTED:
      Serial.println("Disconnected");
      break;
    default:
      Serial.println("Unknown");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Subnet: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
  }
  
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  
  Serial.println("=======================\n");
}
```

Call in setup() after WiFi connection attempt.

## Sensor Issues

### Sensor Always Reads 0

**Causes:**
1. Sensor not powered
2. Wrong pin connection
3. Faulty sensor

**Solutions:**
1. Check 5V and GND connections
2. Verify signal wire on A0
3. Measure voltage at signal pin (should be 0-5V)
4. Test with multimeter in resistance mode
5. Try different analog pin

### Sensor Always Reads 1023

**Causes:**
1. Signal pin floating (not connected)
2. Sensor damaged
3. Wrong sensor type (digital instead of analog)

**Solutions:**
1. Check signal wire firmly connected to A0
2. Test sensor with multimeter
3. Verify sensor has analog output (not just digital)

### Erratic/Jumping Readings

**Causes:**
1. Loose connections
2. Electrical noise
3. Long wire runs

**Solutions:**
1. Add 0.1µF capacitor between A0 and GND
2. Use shielded cable
3. Twist signal and ground wires together
4. Keep sensor wire away from power cables
5. Add software filtering:

```cpp
// Averaging filter
const int NUM_SAMPLES = 10;
int samples[NUM_SAMPLES];
int sampleIndex = 0;

int getFilteredReading() {
  samples[sampleIndex] = analogRead(WATER_SENSOR_PIN);
  sampleIndex = (sampleIndex + 1) % NUM_SAMPLES;
  
  long sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += samples[i];
  }
  return sum / NUM_SAMPLES;
}
```

### False Alerts (No Water Present)

**Causes:**
1. Threshold too low
2. Sensor contamination
3. Humidity/condensation
4. Electrical interference

**Solutions:**
1. Increase WATER_THRESHOLD value
2. Clean sensor with isopropyl alcohol
3. Apply waterproof coating (conformal spray)
4. Test in controlled environment
5. Add more aggressive hysteresis

### Sensor Not Detecting Water

**Causes:**
1. Threshold too high
2. Poor water contact
3. Corroded sensor traces

**Solutions:**
1. Lower WATER_THRESHOLD value
2. Ensure water covers sensor traces fully
3. Clean sensor contacts
4. Replace sensor if corroded
5. Calibrate properly (see README)

## Alert Issues

### LED Doesn't Light

**Check:**
1. LED polarity (long lead to resistor)
2. Resistor value (220Ω)
3. Connection to D8 and GND
4. Test LED with 3V battery
5. Try different LED

### LED Stays On

**Causes:**
1. Code stuck in alert state
2. Sensor reading stuck high
3. Pin configuration error

**Solutions:**
1. Check Serial Monitor for debug info
2. Manually check sensor value
3. Reset Arduino
4. Verify alert clearing logic

### Buzzer Doesn't Sound

**Check:**
1. Buzzer polarity (+ to D9)
2. Active vs passive buzzer (needs active)
3. Test buzzer with 5V directly
4. Check pin D9 connection
5. Verify buzzer voltage rating (3-5V)

### Buzzer Always On

**Solutions:**
1. Check BUZZER_DURATION setting
2. Verify buzzer connected to D9 (not D8)
3. Check for code errors in handleAlerts()

### Buzzer Too Quiet

**Solutions:**
1. Use powered speaker/buzzer
2. Add transistor driver circuit:

```
D9 ─── 1kΩ ─── Base (NPN transistor)
                 │
              Collector ─── Buzzer+ ─── 5V
                 │
              Emitter ─── GND
```

## Web Interface Issues

### Cannot Access Dashboard

**Causes:**
1. WiFi not connected (IP is 0.0.0.0)
2. Wrong IP address
3. Firewall blocking
4. Wrong network

**Solutions:**
1. Verify IP from Serial Monitor
2. Check device on same WiFi network
3. Ping Arduino: `ping [IP_ADDRESS]`
4. Disable firewall temporarily
5. Try different device/browser

### Dashboard Not Updating

**Check:**
1. JavaScript console for errors (F12 in browser)
2. Network tab shows API calls
3. Arduino still connected to WiFi
4. Serial Monitor for errors

**Solutions:**
1. Refresh browser (Ctrl+R)
2. Clear browser cache
3. Check API endpoint directly: `http://[IP]/api/status`
4. Try different browser

### API Returns Invalid Data

**Solutions:**
1. Check Arduino still running (Serial Monitor)
2. Reset Arduino
3. Verify API URL is correct
4. Check for JSON syntax errors

### Multiple Devices Can't Connect

**Limitation:** Arduino can handle maximum 4 simultaneous connections

**Solutions:**
1. Close unused browser tabs
2. Reduce polling frequency
3. Use single dashboard for monitoring
4. Consider upgrading to ESP32 (future)

## Performance Issues

### Slow Response Time

**Causes:**
1. Weak WiFi signal
2. Network congestion
3. Too many simultaneous connections

**Solutions:**
1. Move Arduino closer to router
2. Reduce API polling frequency
3. Limit number of clients
4. Check Serial Monitor for delays

### Arduino Resets/Crashes

**Causes:**
1. Power supply insufficient
2. Memory overflow
3. Software bug
4. Hardware short circuit

**Solutions:**
1. Use 2A power supply
2. Check all wiring for shorts
3. Re-upload sketch
4. Monitor Serial output for errors
5. Add decoupling capacitors

## Serial Monitor Issues

### No Output

**Check:**
1. Correct baud rate (115200)
2. Right COM port selected
3. USB cable connected
4. Arduino powered on
5. Try different USB cable/port

### Garbled Output

**Solutions:**
1. Set baud rate to 115200
2. Check "Both NL & CR" line ending setting
3. Try different USB cable
4. Update USB drivers

## Hardware Issues

### Arduino Won't Power On

1. Check USB cable (try different one)
2. Check power supply (needs 5V)
3. Look for physical damage
4. Check for shorts with multimeter
5. Measure voltage at 5V pin

### Overheating

**Causes:**
1. Short circuit
2. Too much current draw
3. Power supply issues

**Solutions:**
1. Check all wiring
2. Remove components one by one
3. Check for damaged components
4. Add heatsink if needed

## Getting Help

If none of these solutions work:

1. **Open GitHub Issue** with:
   - Full Serial Monitor output
   - Photos of wiring
   - Description of problem
   - What you've tried
   - Hardware/software versions

2. **Arduino Forums:**
   - https://forum.arduino.cc/

3. **Reddit:**
   - r/arduino
   - r/esp32

4. **Discord:**
   - Arduino Official Discord

**Before asking for help:**
- ✅ Read this guide completely
- ✅ Try all relevant solutions
- ✅ Document what you tried
- ✅ Provide clear photos/code
- ✅ Include Serial Monitor output

---

**Remember:** Most issues are simple connection or configuration problems. Take your time, check everything systematically, and you'll get it working! 🔧
