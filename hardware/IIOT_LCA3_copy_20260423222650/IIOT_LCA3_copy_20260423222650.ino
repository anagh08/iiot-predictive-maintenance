#include <WiFi.h>
#include <PubSubClient.h>

// -------- WIFI --------
const char* ssid = "AM";
const char* password = "12345678";

// -------- MQTT --------
const char* mqtt_server = "broker.hivemq.com";
WiFiClient espClient;
PubSubClient client(espClient);

// -------- STATE --------
struct MachineState {
  float hist[5];
  float ema;
  bool emaInit;
};

MachineState m1 = {{50,55,60,65,70}, 60, false};
MachineState m2 = {{48,52,55,58,60}, 58, false};

int stepCounter = 0;

// -------- TEMPERATURE MODEL --------
float genTemp(MachineState &m, int machineID) {

  float base;
  int phase = stepCounter % 60;

  if (phase < 15) {
    base = 50 + random(-2, 3); // NORMAL
  }
  else if (phase < 30) {
    base = 55 + (phase - 15) * 1.5 + random(-2, 2); // DEGRADATION
  }
  else if (phase < 45) {
    base = 75 + random(5, 15); // FAILURE
  }
  else {
    base = 60 - (phase - 45) * 1.2 + random(-2, 2); // RECOVERY
  }

  if (machineID == 2) base -= 5; // Machine 2 healthier

  base = constrain(base, 40, 100);

  // shift history
  for (int i = 0; i < 4; i++) m.hist[i] = m.hist[i + 1];
  m.hist[4] = base;

  return base;
}

// -------- VIBRATION MODEL --------
float genVib(int machineID) {

  int phase = stepCounter % 60;
  float vib;

  if (phase < 15) {
    vib = 0.6 + random(-10, 10) / 100.0;
  }
  else if (phase < 30) {
    vib = 0.8 + (phase - 15) * 0.03;
  }
  else if (phase < 45) {
    vib = 1.4 + random(0, 50) / 100.0;
  }
  else {
    vib = 1.0 - (phase - 45) * 0.03;
  }

  // Random spike (realistic fault)
  if (random(0, 20) == 1) {
    vib += random(50, 100) / 100.0;
  }

  if (machineID == 2) vib *= 0.9;

  return constrain(vib, 0.5, 2.0);
}

// -------- STATUS --------
String getStatus(float temp, float vib) {
  if (temp > 80 || vib > 1.5) return "FAILURE";
  else if (temp > 60) return "WARNING";
  else return "NORMAL";
}

// -------- PREDICTION --------
String getPrediction(float temp, float vib, MachineState &m) {

  float tempFactor = constrain((temp - 40) / 60.0, 0, 1);
  float vibFactor  = constrain(vib / 2.0, 0, 1);

  float trend = 0;
  for (int i = 1; i < 5; i++) trend += (m.hist[i] - m.hist[i - 1]);
  trend /= 4.0;

  float trendFactor = constrain(trend / 5.0, 0, 1);

  float risk = (0.5 * tempFactor) + (0.3 * trendFactor) + (0.2 * vibFactor);

  if (risk > 0.75) return "FAILURE LIKELY SOON";
  else if (risk > 0.5) return "WARNING";
  else return "STABLE";
}

// -------- ANOMALY --------
float computeAnomaly(float temp, MachineState &m) {

  float alpha = 0.3;

  if (!m.emaInit) {
    m.ema = temp;
    m.emaInit = true;
  } else {
    m.ema = alpha * temp + (1 - alpha) * m.ema;
  }

  float mean = 0;
  for (int i = 0; i < 5; i++) mean += m.hist[i];
  mean /= 5.0;

  float var = 0;
  for (int i = 0; i < 5; i++) {
    float d = m.hist[i] - mean;
    var += d * d;
  }
  var /= 5.0;

  float stddev = sqrt(var) + 0.001;

  return fabs(temp - m.ema) / stddev;
}

// -------- MQTT --------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    if (client.connect("ESP32_IIOT")) {
      Serial.println("Connected");
    } else {
      delay(2000);
    }
  }
}

// -------- SETUP --------
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  client.setServer(mqtt_server, 1883);
}

// -------- PUBLISH --------
void publishMachine(const char* topic, MachineState &m, int machineID) {

  float temp = genTemp(m, machineID);
  float vib  = genVib(machineID);

  String status = getStatus(temp, vib);
  String pred   = getPrediction(temp, vib, m);
  float anomaly = computeAnomaly(temp, m);

  String payload = "{";
  payload += "\"temp\":" + String(temp) + ",";
  payload += "\"vibration\":" + String(vib) + ",";
  payload += "\"status\":\"" + status + "\",";
  payload += "\"prediction\":\"" + pred + "\",";
  payload += "\"anomaly\":" + String(anomaly, 2);
  payload += "}";

  Serial.print(topic);
  Serial.print(" -> ");
  Serial.println(payload);

  client.publish(topic, payload.c_str());
}

// -------- LOOP --------
void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  publishMachine("iiot/machine1", m1, 1);
  publishMachine("iiot/machine2", m2, 2);

  stepCounter++;

  delay(2000);
}