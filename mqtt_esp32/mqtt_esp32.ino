#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi settings
char* ssid = "your ssid name";
char* password = "password";

// MQTT settings
const char* mqtt_server = ""; // IP address of your MQTT broker
const char* mqtt_topic = ""; // MQTT topic to publish to


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setupWifi();
  client.setServer(mqtt_server, 1883); // MQTT broker port is typically 1883
  client.setCallback(callback);
  
 
}

void setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.print("Message content: ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Check if the received message matches "red" or "blue"
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    client.publish(mqtt_topic, input.c_str());
    Serial.println("Published: " + input);
  }

  client.loop();
}
