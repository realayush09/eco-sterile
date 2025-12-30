# Arduino Serial Data Format Reference

## Output Format

Your Arduino sends JSON data on each loop cycle (every 1.5 seconds by default):

```json
{"pH":7.45,"voltage":2.441,"pump":"NONE","action":"OFF"}
```

### Field Breakdown

| Field | Example | Range | Description |
|-------|---------|-------|-------------|
| `pH` | `7.45` | 4.0 - 10.0 | Calculated pH value (2 decimal places) |
| `voltage` | `2.441` | 1.999 - 3.666 | Raw analog voltage from sensor (3 decimals) |
| `pump` | `NONE` | NONE / BASIC / ACIDIC | Which pump is currently active |
| `action` | `OFF` | ON / OFF | Pump relay status (HIGH=ON, LOW=OFF) |

## Example Sequences

### Normal Operation (pH Stable)
```json
{"pH":7.34,"voltage":2.408,"pump":"NONE","action":"OFF"}
{"pH":7.38,"voltage":2.389,"pump":"NONE","action":"OFF"}
{"pH":7.45,"voltage":2.348,"pump":"NONE","action":"OFF"}
{"pH":7.41,"voltage":2.365,"pump":"NONE","action":"OFF"}
{"pH":7.39,"voltage":2.378,"pump":"NONE","action":"OFF"}
```

**Dashboard shows:**
- ✓ pH: 7.39 (displayed)
- ✓ Status: 🟢 Optimal
- ✓ Last Pump: None
- ✓ Graph: Flat line around 7.4

---

### Too Acidic Detected (pH < 6.5)
```json
{"pH":7.45,"voltage":2.348,"pump":"NONE","action":"OFF"}
{"pH":7.23,"voltage":2.512,"pump":"NONE","action":"OFF"}
{"pH":6.89,"voltage":2.889,"pump":"NONE","action":"OFF"}
{"pH":6.34,"voltage":3.456,"pump":"BASIC","action":"ON"}
{"pH":6.38,"voltage":3.421,"pump":"BASIC","action":"ON"}
{"pH":6.52,"voltage":3.301,"pump":"BASIC","action":"ON"}
{"pH":6.78,"voltage":3.045,"pump":"BASIC","action":"ON"}
{"pH":7.05,"voltage":2.789,"pump":"NONE","action":"OFF"}
```

**Dashboard shows:**
- ✓ pH: 7.05 (increasing)
- ✓ Status: 🟢 Optimal
- ✓ Last Pump: 💧 Basic Pump — adds base (raises pH)
- ✓ Log Entry: "BASIC pump activated - added 1% Ammonium Hydroxide (NH4OH)"
- ✓ Graph: Dip and recovery pattern

---

### Too Basic Detected (pH > 7.5)
```json
{"pH":7.45,"voltage":2.348,"pump":"NONE","action":"OFF"}
{"pH":7.67,"voltage":2.156,"pump":"NONE","action":"OFF"}
{"pH":7.89,"voltage":1.982,"pump":"ACIDIC","action":"ON"}
{"pH":7.84,"voltage":2.021,"pump":"ACIDIC","action":"ON"}
{"pH":7.62,"voltage":2.198,"pump":"ACIDIC","action":"ON"}
{"pH":7.41,"voltage":2.365,"pump":"NONE","action":"OFF"}
```

**Dashboard shows:**
- ✓ pH: 7.41 (decreasing)
- ✓ Status: 🟢 Optimal
- ✓ Last Pump: ⚗️ Acidic Pump — adds acid (lowers pH)
- ✓ Log Entry: "ACIDIC pump activated - added 1% Acetic Acid (CH3COOH)"
- ✓ Graph: Peak and decline pattern

---

## Real-World Test Data

### Day 1 Morning (Fish Tank Morning Routine)
```
pH:7.12|Voltage:2.689V|Pump:NONE|Action:OFF
pH:7.15|Voltage:2.671V|Pump:NONE|Action:OFF
pH:7.18|Voltage:2.653V|Pump:NONE|Action:OFF
```

### Day 1 Afternoon (Temperature Rise)
```
pH:7.34|Voltage:2.508V|Pump:NONE|Action:OFF
pH:7.42|Voltage:2.451V|Pump:NONE|Action:OFF
pH:7.56|Voltage:2.356V|Pump:NONE|Action:OFF
pH:7.78|Voltage:2.189V|Pump:ACIDIC|Action:ON   ← Auto-corrected!
pH:7.51|Voltage:2.412V|Pump:NONE|Action:OFF
```

### Day 1 Evening (Back to Stable)
```
pH:7.35|Voltage:2.491V|Pump:NONE|Action:OFF
pH:7.38|Voltage:2.475V|Pump:NONE|Action:OFF
pH:7.40|Voltage:2.461V|Pump:NONE|Action:OFF
```

---

## Calibration Data

Your pH sensor calibration points (from Arduino.cpp):

```cpp
pH 4.0 (Acidic) = 3.666 V
pH 7.0 (Neutral) = 2.44 V
pH 10.0 (Basic) = 1.999 V
```

### What This Means

- **Lower voltage = Higher pH** (more basic)
- **Higher voltage = Lower pH** (more acidic)
- Linear interpolation used between calibration points

### Example Calculations

| Voltage | Calculation | Expected pH |
|---------|-----------|------------|
| 1.999 V | Basic calibration point | 10.0 |
| 2.44 V | Neutral calibration point | 7.0 |
| 3.666 V | Acidic calibration point | 4.0 |
| 2.710 V | Between neutral & acidic | ~6.5 |
| 2.150 V | Between basic & neutral | ~9.0 |

---

## Control Logic (Automatic)

```
pH Reading → Arduino Logic → Pump Action

pH < 6.5   → Too Acidic   → Activate BASIC pump (Ammonium Hydroxide - NH4OH)
6.5 ≤ pH ≤ 7.5 → Optimal    → All pumps OFF
pH > 7.5   → Too Basic    → Activate ACIDIC pump (Acetic Acid - CH3COOH)
```

**No manual control needed!** Arduino automatically regulates.

---

## What Dashboard Parses

The web dashboard extracts from each line:

```javascript
// It looks for this pattern:
const phMatch = text.match(/pH:\s*([\d.]+)/);

// Example: "pH:7.45|Voltage:2.441V|Pump:NONE|Action:OFF"
// Extracts: 7.45
```

### Automatic Actions

When dashboard receives each reading:
1. ✓ Updates pH value display
2. ✓ Updates status indicator
3. ✓ Updates last update timestamp
4. ✓ Saves to chart data
5. ✓ Detects pump changes → logs activity
6. ✓ Saves everything to localStorage

---

## Troubleshooting Format Issues

### Issue: Dashboard not updating

**Check 1:** Arduino serial monitor shows data?
```
Yes  → Problem is browser connection
No   → Problem is Arduino code/sensor
```

**Check 2:** Data format correct?
```
Expected: pH:7.45|Voltage:2.441V|Pump:NONE|Action:OFF
          ↑ Must have these fields
```

**Check 3:** Browser console shows errors?
```
Press F12 → Console tab → Look for red errors
```

---

## Custom Data Format (Advanced)

If you want to modify the format, update both:

1. **Arduino.cpp** - What data to send:
```cpp
Serial.print("pH:");
Serial.print(pH, 2);
Serial.print("|Voltage:");
Serial.print(voltage, 3);
Serial.print("V|");
Serial.print("Pump:");
Serial.print(pumpType);
```

2. **script.js** - How to parse it:
```javascript
const phMatch = text.match(/pH:\s*([\d.]+)/);
const pumpMatch = text.match(/Pump:\s*(\w+)/);
```

---

## Performance Notes

- **Serial Speed:** 9600 baud (very reliable for Arduino)
- **Update Rate:** Every 1.5 seconds (configurable)
- **Data per Reading:** ~50 bytes
- **Monthly Storage:** ~2.5 MB (localStorage limit is usually 5-10 MB)

---

## Live Monitoring Tips

### For Crops 🌾
- Morning: Check if overnight acidification needed
- Afternoon: Monitor post-watering pH changes
- Evening: Adjust for next morning

### For Aquariums 🐠
- Watch for pH swings from feeding/cleanup
- Track seasonal changes
- Correlate with temperature readings

### For Both
- Set pH sensors in same location for consistency
- Calibrate sensors weekly
- Keep concentration at 1% for safety
- Stir solution thoroughly before use

---

Good luck with your EcoSterile system! 🌱
