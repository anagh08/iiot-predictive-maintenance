## 🚀 How to Run

### 1. ESP32 Setup
- Open the `.ino` file in Arduino IDE  
- Install required libraries:
  - PubSubClient  
  - ArduinoJson  
- Update WiFi credentials  
- Upload code to ESP32  

### 2. Dashboard
- Open `index.html` in a web browser  
- Ensure MQTT broker connection is active  
- View real-time temperature & vibration data  

---

## ⚠️ Limitations
- Uses simulated sensor data (no physical sensors)  
- Prediction is rule-based (no machine learning yet)  
- No persistent data storage  
- Depends on stable WiFi connectivity  

---

## 🔮 Future Scope
- Integrate real sensors (temperature, vibration)  
- Add machine learning model (TinyML / Python backend)  
- Store historical data (CSV / InfluxDB)  
- Implement alert system (SMS / Email)  
- Use secure/private MQTT broker  

---

## 🎯 Goal
Transform this system into a **fully AI-driven predictive maintenance platform** for real-world industrial applications.

---

## 📚 Key Learnings
- MQTT publish-subscribe architecture  
- ESP32-based embedded system design  
- Real-time data visualization using web dashboards  
- Designing rule-based predictive logic  

---

## 👨‍💻 Authors
- Anagh Mishra  
- Rudransh Jagtap  

---

## 📌 Note
This project currently implements a **rule-based predictive maintenance system** and serves as a foundation for future AI integration.
