# Integration Guide - Arduino to Web Dashboard

## How to Integrate Arduino with the Web Dashboard

### Step 1: Upload Arduino Code to Board

1. Open Arduino IDE
2. Open `Arduino.cpp` from the EcoSterile folder
3. Copy all content and paste into Arduino IDE
4. Verify hardware connections:
   - D7 → Basic Pump Relay IN
   - D8 → Acidic Pump Relay IN
   - A0 → pH Sensor
   - Relay COM → Battery Positive
   - Relay NO → Pump Positive
   - Pump Negative → Battery Negative
5. Select your Arduino board type (Tools → Board → Arduino UNO)
6. Select COM port (Tools → Port → COM#)
7. Click **Upload**
8. Open Serial Monitor (Tools → Serial Monitor) and set baud rate to **9600**
   - You should see JSON output like: `{"pH":7.45,"voltage":2.441,"pump":"NONE","action":"OFF"}`
   - Continuous readings every 1.5 seconds

### Step 2: Open Dashboard

1. Navigate to the EcoSterile folder
2. Double-click `index.html` to open in browser
3. You should see the dashboard with demo data

### Step 3: Connect Arduino (Two Options)

#### Option A: Web Serial API (Recommended - Chrome/Edge)

1. In `script.js`, find line ~240 (search for `async function connectArduino()`)
2. Uncomment the entire function (remove `/*` and `*/`)
3. Comment out or remove the `startSimulation()` line on line ~402
4. Refresh the browser
5. You'll see a popup asking to select the Arduino port
6. Real data will start flowing!

**Code to uncomment in script.js:**
```javascript
async function connectArduino() {
    try {
        const port = await navigator.serial.requestPort();
        await port.open({ baudRate: 9600 });
        
        const reader = port.readable.getReader();
        
        while (true) {
            const { value, done } = await reader.read();
            if (done) break;
            
            const text = new TextDecoder().decode(value);
            const phMatch = text.match(/pH:\s*([\d.]+)/);
            
            if (phMatch) {
                const pH = parseFloat(phMatch[1]);
                addPHReading(pH);
                simulatePumpControl(pH);
            }
        }
    } catch (error) {
        console.error('Serial error:', error);
        alert('Could not connect to Arduino. Starting in demo mode.');
        startSimulation();
    }
}
```

#### Option B: Demo Mode (Test Without Arduino)

The dashboard runs in **demo mode** by default:
- Generates realistic pH fluctuations (4-10)
- Automatically logs pump activity
- Perfect for testing the UI and features
- No Arduino connection needed

### Step 4: Data Format Reference

Your Arduino sends JSON data in this format:
```json
{"pH":7.45,"voltage":2.441,"pump":"NONE","action":"OFF"}
{"pH":6.38,"voltage":3.456,"pump":"BASIC","action":"ON"}
{"pH":7.89,"voltage":2.250,"pump":"ACIDIC","action":"ON"}
```

The dashboard parses:
- **pH** - The calculated pH level (2 decimal places)
- **voltage** - Raw analog voltage from sensor
- **pump** - Which pump is active (NONE / BASIC / ACIDIC)
- **action** - Pump status (ON / OFF)
- Automatically logs pump activity when pump changes

### Troubleshooting Arduino Connection

**Issue: "Could not connect to Arduino"**
- ✓ Check USB cable is properly connected
- ✓ Verify device in Device Manager (look for "Arduino UNO" or "COM#")
- ✓ Install Arduino drivers if not appearing
- ✓ Try a different USB cable or port
- ✓ Restart Arduino IDE and try uploading again

**Issue: COM port not showing in Device Manager**
- Download and install CH340 drivers (common Arduino clone driver)
- Restart computer
- Reconnect Arduino

**Issue: Serial data not readable**
- Verify baud rate is 9600 in both Arduino sketch and script.js
- Check Arduino serial monitor to confirm data is being sent
- Try different USB port on your computer

**Issue: Dashboard not updating**
- Check browser console (F12 → Console) for JavaScript errors
- Make sure Web Serial API is enabled (Chrome/Edge)
- Try demo mode first - if that works, Arduino connection is the issue
- Check if data format matches expected pattern

### What the Dashboard Displays

**From Arduino pH Data:**
- ✓ Real-time pH value (0-14 scale)
- ✓ Status: Too Acidic / Optimal / Too Basic
- ✓ Last pump activated (BASIC/ACIDIC)
- ✓ Solution type and concentration

**Auto-Logged:**
- ✓ Timestamp of each pump activation
- ✓ Complete activity history (up to 30 days)
- ✓ Statistics: Average pH, pH range, pump counts

**Generated:**
- ✓ Historical graph of pH over time
- ✓ Filter by time range (24h, 7d, month)

### Advanced: Custom Modifications

#### Change Pump Trigger Thresholds
In `Arduino.cpp`, modify these lines:
```cpp
if (pH < 6.5) {           // Change 6.5 to different threshold
    // Too acidic → turn ON BASIC pump
```

#### Change Data Update Frequency
In `Arduino.cpp`:
```cpp
delay(1500);  // Change 1500 (milliseconds) to desired interval
```

#### Change Chart Update Time Range
In `script.js`, find the `switch(timeRange)` statement in `updateChart()` function

#### Add More Solutions/Pumps
- Modify HTML to add more cards
- Add pump variables to Arduino
- Update the logging function

### Example: Reading from Serial Monitor

When running your Arduino with this code, you should see:

```
EcoSterile System Started
========================
pH:7.34|Voltage:2.408V|Pump:NONE|Action:OFF
pH:7.38|Voltage:2.389V|Pump:NONE|Action:OFF
pH:7.45|Voltage:2.348V|Pump:NONE|Action:OFF
pH:6.23|Voltage:3.521V|Pump:BASIC|Action:ON
pH:6.78|Voltage:3.045V|Pump:BASIC|Action:ON
pH:7.12|Voltage:2.789V|Pump:NONE|Action:OFF
pH:7.89|Voltage:2.156V|Pump:ACIDIC|Action:ON
pH:7.54|Voltage:2.421V|Pump:NONE|Action:OFF
```

Each line represents one sensor reading (updates every 1.5 seconds by default).

### Storage & Data Persistence

- All dashboard data saved to browser **localStorage**
- 30 days of historical data retained
- Data persists even after closing browser
- "Clear Data" button wipes all stored data

### Security Notes

- This dashboard is local-only (no internet transmission)
- Safe for private monitoring
- Data never leaves your computer
- Perfect for garden/aquarium setups

---

## Next Steps

1. ✓ Upload Arduino code
2. ✓ Open index.html in browser
3. ✓ Test with demo mode
4. ✓ Enable Arduino connection (uncomment connectArduino)
5. ✓ Monitor real pH data!

Enjoy monitoring your crops and aquatic organisms! 🌱
