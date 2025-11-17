#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

static const char* wifiSsid_AP  = "Hotspot";
static const char* wifiPass     = "theflash";
static const char* mqttHost      = "10.236.213.10";
static const uint16_t mqttPort  = 1883;

static const char* deviceLabel          = "esp32";
static const char* topic_Telemetry      = "iot/telemetry";
static const char* topic_CmdInterval    = "iot/cmd/interval";
static const char* topic_CmdLed         = "iot/cmd/led";
static const char* topic_Ack            = "iot/ack";

#define DHTPIN   16
#define DHTTYPE  DHT11
#define NEOPIX_PIN 4
#define NEOPIX_COUNT 1

DHT dht(DHTPIN, DHTTYPE);
Adafruit_NeoPixel pixelStrip(NEOPIX_COUNT, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

unsigned long intervalMs_Telemetry = 5000;
unsigned long lastTelemetrySent_ms = 0;

const float tempLow    = 20.0;   // below → blue
const float tempMid    = 26.0;   // between → amber
const float tempHigh   = 32.0;   // above → red

void setPixelRGB(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NEOPIX_COUNT; ++i)
    pixelStrip.setPixelColor(i, pixelStrip.Color(r, g, b));
  pixelStrip.show();
}

void blinkGreen(uint8_t times = 2, uint16_t delayMs = 150) {
  for (uint8_t i = 0; i < times; ++i) {
    setPixelRGB(0, 255, 0); delay(delayMs);
    setPixelRGB(0, 0, 0);   delay(delayMs);
  }
}

void setColorForTemperature(float tempC) {
  if (isnan(tempC)) {
    setPixelRGB(0, 0, 0);
    return;
  }

  if (tempC <= tempLow) {
    setPixelRGB(0, 32, 255);
  } else if (tempC > tempLow && tempC < tempHigh) {
    float ratio = (tempC - tempLow) / (tempHigh - tempLow);
    uint8_t r = (uint8_t)(255 * ratio);
    uint8_t g = (uint8_t)(128 * (1 - ratio));
    uint8_t b = (uint8_t)(255 * (1 - ratio));
    setPixelRGB(r, g, b);
  } else {
    setPixelRGB(255, 0, 0);
  }
}

void publishAck(const char* cmdName, bool ok, const char* note = nullptr) {
  StaticJsonDocument<160> doc;
  doc["deviceLabel"] = deviceLabel;
  doc["cmd"] = cmdName;
  doc["ok"]  = ok;
  doc["ts_ms"] = (uint32_t)millis();
  if (note) doc["note"] = note;
  char buf[192];
  size_t n = serializeJson(doc, buf);
  mqttClient.publish(topic_Ack, buf, n);
}

void handleMqttMessage(char* topic, byte* payload, unsigned int len) {
  StaticJsonDocument<200> doc;
  DeserializationError e = deserializeJson(doc, payload, len);
  if (e) return;

  String t = String(topic);
  if (t == topic_CmdInterval) {
    unsigned long newInterval = doc["interval"] | intervalMs_Telemetry;
    if (newInterval < 500) newInterval = 500;
    if (newInterval > 60000) newInterval = 60000;
    intervalMs_Telemetry = newInterval;
    Serial.printf("[CMD] interval set to %lu ms\n", intervalMs_Telemetry);
    publishAck("interval", true);
  } else if (t == topic_CmdLed) {
    uint8_t r = doc["r"] | 0;
    uint8_t g = doc["g"] | 0;
    uint8_t b = doc["b"] | 0;
    setPixelRGB(r, g, b);
    Serial.printf("[CMD] LED set to R%d G%d B%d\n", r,g,b);
    publishAck("led", true);
  }
}

void ensureWifi() {
  if (WiFi.status() == WL_CONNECTED) return;
  Serial.printf("[WIFI] connecting to %s", wifiSsid_AP);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid_AP, wifiPass);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(250); Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("[WIFI] OK, IP: %s, RSSI: %d dBm\n",
                  WiFi.localIP().toString().c_str(), WiFi.RSSI());
    blinkGreen(3, 120);
  } else {
    Serial.println("[WIFI] failed to connect (timeout)");
  }
}

void ensureMqtt() {
  if (mqttClient.connected()) return;
  mqttClient.setServer(mqttHost, mqttPort);
  mqttClient.setCallback(handleMqttMessage);
  Serial.print("[MQTT] connecting");
  while (!mqttClient.connected()) {
    if (mqttClient.connect(deviceLabel)) {
      Serial.println(" OK");
      mqttClient.subscribe(topic_CmdInterval);
      mqttClient.subscribe(topic_CmdLed);
      blinkGreen(2, 120);
    } else {
      Serial.print(".");
      delay(1500);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);
  dht.begin();
  pixelStrip.begin();
  pixelStrip.clear();
  setPixelRGB(0, 0, 0); 

  ensureWifi();
  ensureMqtt();
}

void loop() {
  ensureWifi();
  ensureMqtt();
  mqttClient.loop();

  unsigned long now = millis();
  if (now - lastTelemetrySent_ms >= intervalMs_Telemetry) {
    lastTelemetrySent_ms = now;

    float tempC = dht.readTemperature();
    float humRH = dht.readHumidity();

    if (isnan(tempC) || isnan(humRH)) {
      Serial.println("[SENSOR] DHT read failed");
      return;
    }

    setColorForTemperature(tempC);  

    StaticJsonDocument<240> doc;
    doc["deviceLabel"]   = deviceLabel;
    doc["ts_ms"]         = (uint32_t)now;
    doc["tempC"]         = tempC;
    doc["humidity"]      = humRH;
    doc["wifiRssi_dBm"]  = WiFi.RSSI();

    char out[256];
    size_t n = serializeJson(doc, out);
    mqttClient.publish(topic_Telemetry, out, n);
    Serial.println(out);
  }
}