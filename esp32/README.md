<h1 align="center">🔧 ESP32 Firmware Documentation</h1>

---

# 📌 Overview
The ESP32 firmware collects temperature, humidity and WiFi RSSI while supporting LED control and update‑interval configuration using MQTT commands.

---

# 🛠 Hardware Setup

<img src="../documentation/images/esp32_hardware.png" width="500">

### Components:
- ESP32 DevKitC  
- DHT11 Sensor  
- NeoPixel RGB LED  
- 3.3V Power  

### Wiring Table:
| Component | ESP32 Pin |
|----------|-----------|
| DHT11 Data | GPIO 16 |
| LED Data | GPIO 4 |
| Power | 3.3V |
| Ground | GND |

---

# 📡 MQTT Communication

### Publishes:
```
iot/telemetry
```

### Subscribes:
```
iot/cmd/interval
iot/cmd/led
```

### Acknowledges:
```
iot/ack
```

---

# ▶️ Running the Firmware
1. Open Arduino IDE  
2. Install ESP32 board support  
3. Upload `firmware.ino`  
4. Power ESP32 and view dashboard  

---

# ✔ Author
Shaan Kalani
