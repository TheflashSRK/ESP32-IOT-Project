<h1 align="center">🟧 Node-RED Dashboard & Processing Flow</h1>

---

# 📌 Overview
This directory contains the Node-RED flow used for parsing telemetry, storing it in InfluxDB, generating alerts and creating real‑time dashboards.

---

# 🔄 Main Processing Flow
<img src="../documentation/images/Flow_node-rade.png" width="900">

---

# 🔁 Dashboard Restore Flow
<img src="../documentation/images/node_red_restore_flow.png" width="900">

---

# 📊 Dashboards

## Main Dashboard
<img src="../documentation/images/dashboard_main.png" width="900">

## Alerts Dashboard
<img src="../documentation/images/dashboard_alerts.png" width="900">

---

# 📥 Import Instructions
1. Open Node‑RED  
2. Click **Menu → Import**  
3. Select `flow.json`  
4. Click **Deploy**

---

# 📨 MQTT Topics

**Published:**
```
iot/telemetry
```

**Commands:**
```
iot/cmd/interval
iot/cmd/led
```

---

# ✔ Author
Shaan Kalani
