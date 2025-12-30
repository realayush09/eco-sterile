# 🌱 EcoSterile pH Regulator Dashboard

A real-time web dashboard for monitoring and controlling pH levels using Arduino UNO, two motor pumps, and a pH sensor.

## Features

- 📊 **Real-time pH Monitoring** - Live pH level display with visual scale indicator
- ⚙️ **Pump Activity Tracking** - Last pump used, solution type, and concentration
- 📈 **Historical Graphs** - View pH trends over 24h, 7 days, or monthly periods
- 📝 **Activity Logs** - Complete pump activity history with timestamps
- 📱 **Responsive Design** - Works on desktop, tablet, and mobile devices
- 💾 **Data Persistence** - All data automatically saved to browser storage

## System Components
 - **Arduino UNO** - Microcontroller
 - **pH Sensor** - Analog input (A0)
 - **Relay Module** - Controls two pumps via relays
   - **Basic Pump Relay** - Pin D7 (controls Ammonium Hydroxide pump)
   - **Acidic Pump Relay** - Pin D8 (controls Acetic Acid pump)
 - **External Power Supply** - Provides power to pumps via relay contacts
 - **Solutions**:
   - Basic: Ammonium Hydroxide (NH4OH) - pH increases
   - Acidic: Acetic Acid (CH3COOH) - pH decreases

### Relay Wiring Details
- **COM** → Battery Positive (+)
- **NO (Normally Open)** → Pump Positive (+)
- **Pump Negative (-)** → External Battery Negative (-)
- **Relay IN** → Arduino Pin (D7 or D8)

## Setup Instructions

### 1. Hardware Setup
```
Arduino Pin Configuration:
- A0  → pH Sensor
- D7  → Basic Pump Relay (HIGH = ON, LOW = OFF)
- D8  → Acidic Pump Relay (HIGH = ON, LOW = OFF)

Relay Connections:
- Relay COM   → Battery Positive (+)
- Relay NO    → Pump Positive (+)
- Pump Neg    → Battery Negative (-)
- Relay IN    → Arduino Pin D7 or D8
```

### 2. Open the Dashboard
Simply open `index.html` in any modern web browser (Chrome, Firefox, Edge, Safari).

No server or backend required! The dashboard runs entirely in your browser.

### 3. Demo Mode
The dashboard starts in **demo mode** with simulated pH data for testing. You'll see:
- pH values fluctuating between 4-10
- Pumps automatically triggering when pH goes out of range
- Activity logs and graphs updating in real-time

### 4. Connect Real Arduino (Optional)

To connect your actual Arduino:

1. **Upload Arduino sketch** to your Arduino UNO
2. **Uncomment** the Arduino serial connection code in `script.js` (around line 240)
3. **Modify** the serial communication function if needed:
   ```javascript
   // Find this section and uncomment it:
   /*
   async function connectArduino() {
       try {
           const port = await navigator.serial.requestPort();
           await port.open({ baudRate: 9600 });
           // ... rest of code
       }
   }
   */
   ```
4. The browser will prompt you to select the Arduino COM port
5. Data will stream from Arduino to the dashboard in real-time

## Dashboard Sections

### Current pH Level
- Large, easy-to-read pH value
- Color-coded status (Acidic/Neutral/Basic)
- Visual scale indicator showing position on 0-14 scale

### Last Pump Used
- Which pump was activated (Basic or Acidic)
- When it was used (time ago)
- Solution type and concentration

### Quick Stats
- Average pH for the day
- pH range (min - max)
- Total basic pump activations
- Total acidic pump activations

### pH History Graph
- Visualize pH trends over time
- Filter by: 24 hours, 7 days, or monthly
- Clear all data option

### Pump Activity Log
- Detailed log of all pump activations
- Timestamp of each event
- Solution type and concentration
- Most recent 20 events displayed

## Data Storage

All data is stored in browser **localStorage**:
- Keeps up to 30 days of history
- Automatically saves after each update
- Data persists between browser sessions
- Click "Clear Data" button to reset everything

## Do You Need Python for Backend?

**Short Answer: NO!**

This system works with **no backend required** because:
1. ✅ Data stored in browser (localStorage)
2. ✅ No external server needed
3. ✅ Arduino communicates directly via USB serial
4. ✅ Perfect for small systems and local monitoring

### When Would You Need Python?

You'd only need a backend (Python Flask/FastAPI) if you want to:
- Store data in a cloud database
- Access dashboard from multiple devices on a network
- Send notifications/alerts via email
- Create a mobile app
- Have multiple users with different access levels

For now, this browser-based solution is ideal!

## File Structure
```
EcoSterile/
├── index.html       # Main webpage structure
├── style.css        # Styling and layout
├── script.js        # JavaScript functionality
└── Arduino.cpp      # Arduino firmware (uploaded to board)
```

## Browser Compatibility

- ✅ Chrome/Chromium (recommended for serial)
- ✅ Edge (recommended for serial)
- ✅ Firefox (limited serial support)
- ✅ Safari (limited serial support)

**Note:** Serial communication (Arduino connection) requires Chrome, Edge, or compatible browsers with Web Serial API support.

## Troubleshooting

### Arduino not connecting?
- Make sure Arduino drivers are installed
- Check if COM port is available in Device Manager
- Try a different USB cable
- Verify baud rate is 9600

### Demo data not showing?
- Check browser console (F12) for errors
- Make sure JavaScript is enabled
- Try clearing browser cache and reload

### Data not saving?
- Check if localStorage is enabled
- Make sure you're not in private/incognito mode
- Check browser storage quota

## Future Enhancements

Potential upgrades:
- Email/SMS alerts when pH goes out of range
- Export data to CSV
- Temperature sensor integration
- Cloud sync with Firebase
- Mobile app version
- Automated calibration wizard

## Support

For issues or questions, check:
1. Browser console (F12 → Console tab)
2. Arduino serial monitor
3. Browser localStorage data (F12 → Application → Local Storage)

---

**Created for EcoSterile Project** - Real-time pH Monitoring & Control System
