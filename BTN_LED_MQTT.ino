#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>

#define WIFI_SSID "Ahmad Fauzi Firmansyah"
#define WIFI_PASSWORD "Pimen2122"

// Raspberri Pi Mosquitto MQTT Broker
//#define MQTT_HOST IPAddress(192, 168, 1, XXX)
// For a cloud MQTT broker, type the domain name
#define MQTT_HOST "broker.hivemq.com"
#define MQTT_PORT 1883

// Temperature MQTT Topics
#define MQTT_PUB_BTN "afafirmansyah/esp/dht/button"

#define LED_1 1
#define LED_2 3
#define BTN_1 16
#define BTN_2 13
#define BZR 15

int BZR_enable = 0;
int btn_status = 1;

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 1000;        // Interval at which to publish sensor readings

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

/*void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
  }

  void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  }*/

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // put your setup code here, to run once:
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BZR, OUTPUT);
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  //mqttClient.onSubscribe(onMqttSubscribe);
  //mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  // If your broker requires authentication (username and password), set them below
  //mqttClient.setCredentials("REPlACE_WITH_YOUR_USER", "REPLACE_WITH_YOUR_PASSWORD");

  connectToWifi();
}

void loop() {
  unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 10 seconds)
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;

    if (digitalRead(BTN_1) == 0) {
      digitalWrite(LED_1, LOW);
      btn_status = 1;
      Serial.println(btn_status);
      BZR_enable = 1;
      BUZZER();
    }
    if (digitalRead(BTN_2) == 0) {
      digitalWrite(LED_1, HIGH);
      btn_status = 0;
      Serial.println(btn_status);
      BZR_enable = 1;
      BUZZER();
    }
    // Publish an MQTT message on topic esp/dht/button
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_BTN, 1, true, String(btn_status).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_BTN, packetIdPub1);
    Serial.printf("Message: \n", btn_status);
  }
}

void BUZZER() {
  if (BZR_enable == 1) {
    digitalWrite(BZR, HIGH);
    delay(100);
    digitalWrite(BZR, LOW);
    BZR_enable = 0;
  }
}
