# 🔌 Hardware Documentation

Complete hardware setup guide for the Water Sensor Alert System.

## 📦 Bill of Materials (BOM)

| Item | Quantity | Specifications | Estimated Cost | Purchase Links |
|------|----------|----------------|----------------|----------------|
| Arduino UNO R4 WiFi | 1 | WiFi variant required | $27.50 | [Arduino Store](https://store.arduino.cc/) |
| Water Sensor | 1 | Analog output, 5V | $2-5 | Amazon, AliExpress |
| Active Buzzer | 1 | 5V, 30mA typical | $0.50-1 | Electronics suppliers |
| Red LED | 1 | 5mm, 20mA, 2V forward voltage | $0.10 | Electronics suppliers |
| 220Ω Resistor | 1 | 1/4W or 1/2W | $0.05 | Electronics suppliers |
| Breadboard | 1 | 400 or 830 tie-points | $2-5 | Electronics suppliers |
| Jumper Wires | 10-15 | Male-to-male, male-to-female | $3-5 | Electronics suppliers |
| USB-C Cable | 1 | For Arduino power/programming | $3-5 | Electronics suppliers |

**Total Estimated Cost:** $40-55 USD

## 🔍 Component Details

### Arduino UNO R4 WiFi

**Specifications:**
- Microcontroller: Renesas RA4M1 (Arm Cortex-M4)
- WiFi: ESP32-S3 module
- Operating Voltage: 5V
- Digital I/O Pins: 14 (D0-D13)
- Analog Input Pins: 6 (A0-A5)
- Clock Speed: 48 MHz
- Flash Memory: 256 KB
- SRAM: 32 KB
- USB: USB-C

**Why R4 WiFi?**
- Built-in WiFi eliminates need for external modules
- More powerful than older UNO boards
- Native USB-C connector
- 3.3V and 5V logic support

### Water Sensor Module

**Common Types:**

#### 1. Resistive Water Sensor (Recommended)
- Analog output (0-1023 on Arduino)
- Simple parallel traces that change resistance when wet
- Cost: $2-3
- **Pros:** Cheap, simple, works well
- **Cons:** Corrosion over time, not food-safe

#### 2. Capacitive Water Sensor
- Analog output
- Uses capacitance change detection
- Cost: $5-8
- **Pros:** No exposed traces, corrosion-resistant
- **Cons:** More expensive, requires coating

#### 3. Float Switch
- Digital output (on/off)
- Mechanical float mechanism
- **Note:** Not compatible with this analog-based project

**Typical Pinout:**
```
┌─────────────┐
│  + (VCC)    │ → 5V
│  - (GND)    │ → GND
│  S (Signal) │ → A0
└─────────────┘
```

**Sensor Behavior:**
- Dry: 0-100 (typical)
- Damp: 100-300
- Wet: 300-600
- Submerged: 600-1023

### Active Buzzer

**Specifications:**
- Operating Voltage: 3-5V DC
- Current Draw: 20-30mA
- Sound Pressure: 85-95 dB at 10cm
- Type: Active (includes oscillator circuit)

**Active vs Passive:**
- **Active Buzzer:** Has internal oscillator, just apply DC voltage
- **Passive Buzzer:** Requires PWM signal to produce tone
- This project uses **ACTIVE** buzzer for simplicity

**Polarity:**
- Positive (+): Usually longer lead or marked with "+"
- Negative (-): Shorter lead

### LED (Light Emitting Diode)

**Specifications:**
- Color: Red (or any color you prefer)
- Forward Voltage (Vf): 1.8-2.2V (red)
- Forward Current (If): 20mA (typical)
- Size: 5mm standard

**Why 220Ω Resistor?**
Calculation: R = (Vs - Vf) / If
- Vs = 5V (Arduino output)
- Vf = 2V (LED forward voltage)
- If = 20mA (0.02A)
- R = (5 - 2) / 0.02 = 150Ω
- 220Ω provides safety margin

**LED Polarity:**
- Anode (+): Longer lead
- Cathode (-): Shorter lead, flat edge on LED body

## 🔧 Assembly Instructions

### Step 1: Prepare Components

1. Identify all components
2. Check LED polarity (longer lead = positive)
3. Check buzzer polarity (marked with + or longer lead)
4. Organize jumper wires by length

### Step 2: Breadboard Layout

**Recommended Layout:**
```
        Arduino UNO R4 WiFi
    ┌─────────────────────┐
    │                     │
    │                  5V ●──┬──→ Water Sensor VCC
    │                 GND ●──┴──→ Common Ground Rail
    │                  A0 ●─────→ Water Sensor Signal
    │                     │
    │                  D8 ●─────→ 220Ω → LED → GND
    │                  D9 ●─────→ Buzzer (+)
    │                     │
    └─────────────────────┘

    Breadboard:
    ┌─────────────────────────────┐
    │ + + + + + + + + + + + + + + │ ← Power Rail (5V)
    │ - - - - - - - - - - - - - - │ ← Ground Rail
    │                             │
    │  [Water Sensor]             │
    │                             │
    │  [220Ω]──[LED]              │
    │                             │
    │  [Buzzer]                   │
    │                             │
    └─────────────────────────────┘
```

### Step 3: Power Rails

1. Connect Arduino **5V** to breadboard **+ rail**
2. Connect Arduino **GND** to breadboard **- rail**
3. This provides easy power distribution

### Step 4: Water Sensor Connection

1. Insert water sensor pins into breadboard
2. Connect sensor **VCC** to **+ rail** (red wire)
3. Connect sensor **GND** to **- rail** (black wire)
4. Connect sensor **Signal** to Arduino **A0** (yellow/white wire)

### Step 5: LED Circuit

1. Place 220Ω resistor in breadboard
2. Connect one end of resistor to Arduino **D8**
3. Insert LED with **anode (+)** to other end of resistor
4. Connect LED **cathode (-)** to **- rail**

**Circuit:**
```
D8 ──→ [220Ω] ──→ LED+ ──→ LED- ──→ GND
```

### Step 6: Buzzer Connection

1. Insert buzzer into breadboard
2. Connect buzzer **positive (+)** to Arduino **D9**
3. Connect buzzer **negative (-)** to **- rail**

### Step 7: Double-Check Connections

**Verification Checklist:**
- [ ] Water sensor has 3 wires connected
- [ ] Sensor receives 5V power
- [ ] Sensor signal goes to A0
- [ ] LED has resistor in series
- [ ] LED polarity is correct
- [ ] Buzzer polarity is correct
- [ ] All grounds connected to - rail
- [ ] No short circuits (5V touching GND)

## 📸 Wiring Diagram

### Schematic View

```
                    Arduino UNO R4 WiFi
                   ┌──────────────────┐
                   │                  │
    Water Sensor   │                  │
    ┌──────────┐   │                  │
    │ VCC ─────┼───┤ 5V              │
    │ GND ─────┼───┤ GND             │
    │ SIG ─────┼───┤ A0              │
    └──────────┘   │                  │
                   │                  │
    LED Circuit    │                  │
    ┌──────────┐   │                  │
    │       ┌──┼───┤ D8              │
    │   220Ω   │   │                  │
    │       └──●   │                  │
    │        LED   │                  │
    │       ───┼───┤ GND             │
    └──────────┘   │                  │
                   │                  │
    Buzzer         │                  │
    ┌──────────┐   │                  │
    │  (+) ────┼───┤ D9              │
    │  (-) ────┼───┤ GND             │
    └──────────┘   │                  │
                   │                  │
                   └──────────────────┘
```

### Fritzing Diagram

For a visual Fritzing diagram, see `docs/images/fritzing-diagram.png`

## 🔬 Testing Procedure

### Basic Functionality Test

1. **Power Test:**
   - Connect Arduino to USB
   - Verify green power LED on Arduino lights up
   - Check if water sensor has indicator LED (if equipped)

2. **LED Test:**
   - Upload a simple blink sketch
   - Change pin to 8
   - Verify LED blinks

3. **Buzzer Test:**
   - Upload a simple tone sketch
   - Change pin to 9
   - Verify buzzer sounds

4. **Sensor Test:**
   - Upload main sketch
   - Open Serial Monitor (115200 baud)
   - Watch sensor readings (should be low ~0-100 when dry)
   - Touch sensor with wet finger
   - Readings should increase significantly

### Calibration

1. **Dry Reading:**
   - Keep sensor completely dry
   - Note the reading (e.g., 45)
   - This is your baseline

2. **Wet Reading:**
   - Apply water to sensor surface
   - Note the reading (e.g., 456)
   - This is your detection level

3. **Set Threshold:**
   - Calculate midpoint: (45 + 456) / 2 = 250
   - Set `WATER_THRESHOLD` to this value
   - Set `DRY_THRESHOLD` to below baseline (e.g., 100)

## ⚡ Power Considerations

### USB Power

- Arduino draws ~100mA
- Water sensor: ~10-20mA
- LED: ~20mA
- Buzzer: ~30mA
- **Total:** ~160-170mA
- USB can provide 500mA - **plenty of headroom**

### Battery Power (Optional)

For portable operation:

**Option 1: USB Power Bank**
- Simplest solution
- Connect via USB-C cable
- 10,000mAh bank = ~60+ hours runtime

**Option 2: Battery Pack**
- 4x AA batteries = 6V
- Connect to VIN pin
- Add voltage regulator for longer life
- 2,500mAh = ~15 hours runtime

**Option 3: LiPo Battery**
- 7.4V 2S LiPo with JST connector
- Requires LiPo charger
- Rechargeable solution

## 🛡️ Protection Circuits (Advanced)

### LED Protection (Already Included)

Current-limiting resistor prevents LED burnout:
```
[Arduino Pin] ──→ [220Ω] ──→ [LED] ──→ [GND]
```

### Buzzer Protection (Optional)

Add flyback diode for inductive loads:
```
         ┌──────────┐
    D9 ──┤ Buzzer   │
         └──────────┘
              │
         ┌────┴────┐
         │  Diode  │ (1N4007)
         │ ◄─────  │
         └─────────┘
              │
            GND
```

### Water Sensor Protection (Optional)

Prevent corrosion with waterproofing:
1. Apply clear nail polish to traces
2. Use conformal coating spray
3. Upgrade to capacitive sensor

## 📏 Physical Installation

### Sensor Placement

**Do's:**
- ✅ Place at lowest point where water accumulates
- ✅ Ensure sensor contacts are fully exposed
- ✅ Orient sensor vertically for quick drainage
- ✅ Secure with zip ties or mounting bracket
- ✅ Keep wires away from water flow

**Don'ts:**
- ❌ Don't submerge entire sensor permanently
- ❌ Don't place where it blocks drainage
- ❌ Don't expose to direct sunlight (plastic degradation)
- ❌ Don't use near saltwater (accelerated corrosion)

### Arduino & Electronics

- Mount in weatherproof enclosure
- Use cable glands for wire entry
- Ensure adequate ventilation
- Keep power supply dry
- Label all connections

### Recommended Enclosure

- IP65 or higher rated box
- Transparent lid (to see LED)
- Ventilation holes with mesh
- Size: 150mm x 100mm x 70mm minimum

## 🔧 Troubleshooting Hardware

### LED Not Lighting

**Check:**
1. LED polarity (swap if needed)
2. Resistor connection
3. Test LED with 3V battery
4. Measure voltage at D8 (should be 5V when HIGH)

### Buzzer Not Sounding

**Check:**
1. Buzzer polarity
2. Test buzzer directly with 5V
3. Verify it's an active buzzer
4. Measure voltage at D9

### Sensor Always Reads 0 or 1023

**Check:**
1. Sensor power (5V and GND)
2. Signal wire to A0
3. Try different analog pin
4. Test sensor with multimeter (resistance mode)

### Erratic Readings

**Solutions:**
1. Add 0.1µF capacitor between sensor signal and GND
2. Use shielded cable for sensor wire
3. Keep sensor wire away from power wires
4. Check for loose connections

## 📐 PCB Design (Advanced)

For a permanent installation, consider designing a custom PCB:

### Features to Include:
- Arduino header sockets
- Screw terminals for sensor
- LED and buzzer sockets
- Power regulation circuit
- Protection diodes
- Status LEDs
- Reset button

### Recommended PCB Software:
- KiCad (free, open-source)
- EasyEDA (free, web-based)
- Eagle (paid, professional)

## 🔌 Alternative Connections

### Using Different Pins

If D8 or D9 are needed for other purposes:

```cpp
const int RED_LED_PIN = 10;  // Any digital pin
const int BUZZER_PIN = 11;   // Any digital pin
```

### Multiple Sensors

Expand to multiple rooms:

```cpp
const int SENSOR_1_PIN = A0;  // Basement
const int SENSOR_2_PIN = A1;  // Bathroom
const int SENSOR_3_PIN = A2;  // Kitchen
```

Add corresponding LEDs and buzzers, or use single shared indicators.

## 🌡️ Environmental Considerations

### Operating Conditions

- **Temperature:** 0°C to 50°C
- **Humidity:** 0-95% RH (non-condensing on electronics)
- **Water:** Sensor can be wet, electronics must stay dry
- **Altitude:** Not typically a concern

### Long-term Reliability

**Sensor Maintenance:**
- Clean sensor contacts monthly
- Check for corrosion
- Replace every 6-12 months in harsh conditions

**Electronics Maintenance:**
- Inspect connections quarterly
- Clean dust from enclosure
- Check cable integrity
- Tighten screws if loose

## 📖 Additional Resources

- [Arduino UNO R4 WiFi Documentation](https://docs.arduino.cc/hardware/uno-r4-wifi)
- [Water Sensor Tutorial](https://www.arduino.cc/education/water-sensor)
- [LED Resistor Calculator](https://www.digikey.com/en/resources/conversion-calculators/conversion-calculator-led-series-resistor)
- [Breadboard Tutorial](https://learn.sparkfun.com/tutorials/how-to-use-a-breadboard)

---

**Need help?** Open an issue on GitHub or check the main README troubleshooting section.
