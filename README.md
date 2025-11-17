<h1 align="center">B31OT – IoT Environmental Monitoring System</h1>

<p align="center">
  <img src="documentation/images/esp32_hardware.jpeg" width="500">
</p>

<p align="center">
  <b>ESP32 • MQTT • Node-RED • InfluxDB • Real-Time Environmental Monitoring</b>
</p>

---

# 📘 Overview

This project is a complete IoT environmental monitoring platform built using ESP32, MQTT, Node-RED and InfluxDB. It enables real-time monitoring of temperature, humidity and WiFi strength with alerts and dashboard recovery.

---

# 📂 Repository Structure

```
documentation/
    B31OT_Report.pdf
    images/
        dashboard_alerts.jpeg
        dashboard_main.jpeg
        esp32_hardware.jpeg
        Flow_node-rade.jpeg
        influx_query_results.jpeg
        node_red_restore_flow.jpeg
esp32/
    firmware.ino
influx/
    influx_query_results.txt
    mqtt_logs.txt
nodered/
    flow.json
README.md
```

---

# 🧠 System Architecture

## 📡 Node-RED Main Flow
<img src="documentation/images/Flow_node-rade.jpeg" width="900">

## 🔄 Dashboard Restore Flow
<img src="documentation/images/node_red_restore_flow.jpeg" width="900">

---

# 🖥 Dashboards

## 📊 Main Dashboard
<img src="documentation/images/dashboard_main.jpeg" width="900">

## 🚨 Alerts Dashboard
<img src="documentation/images/dashboard_alerts.jpeg" width="900">

---

# 📈 InfluxDB Query Results
<img src="documentation/images/influx_query_results.jpeg" width="900">

Results stored in:
```
influx/influx_query_results.txt
```

---

# 🧰 ESP32 Firmware
Located in:
```
esp32/firmware.ino
```

---

# ✔ Author
Shaan Kalani (B31OT IoT Coursework)
