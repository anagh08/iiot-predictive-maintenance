Project: IIOT_LCA (Low-Cost IIoT Predictive Maintenance System)

Overview:
This project implements a 3-layer IIoT architecture for predictive maintenance using an ESP32 microcontroller, MQTT communication, and a web-based SCADA dashboard.

Architecture:
1. Edge Layer (ESP32):
   - Simulates temperature and vibration data
   - Computes weighted risk score
   - Publishes JSON data via MQTT

2. Communication Layer (MQTT - HiveMQ):
   - Handles publish-subscribe messaging
   - Topics: iiot/machine/{id}/data

3. Visualization Layer (Web Dashboard):
   - HTML + JS dashboard
   - Uses Paho MQTT + Chart.js
   - Displays real-time graphs and system states

Current Status:
- Arduino firmware implemented (.ino)
- MQTT communication working
- Web dashboard functional (index.html)
- Rule-based prediction model implemented

Limitations:
- No real sensors (simulated data)
- No machine learning model
- No data storage (no historical tracking)

Goal:
Upgrade system from rule-based logic to AI-based predictive maintenance system.

Planned Improvements:
1. Add real sensor integration (temperature + vibration)
2. Store data (CSV / database)
3. Implement ML model (anomaly detection / classification)
4. Integrate AI predictions into dashboard
5. Add alerts and automation

Current Focus:
- Understand full data flow
- Refactor project structure
- Prepare system for AI integration

Next Steps:
- Analyze Arduino code
- Extract and log sensor data
- Design ML pipeline

AI Usage Strategy:
- Use AI for debugging embedded code
- Use AI for system design decisions
- Use AI for ML model development and integration