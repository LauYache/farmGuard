#include <WiFi.h>
#include <PubSubClient.h>


#define LED_ROJO 23     
#define DHTPIN 22        
#define LED_VERDE 21     
#define LED_AZUL 19 


#define LM35_PIN 34

const char* ssid = "";     
const char* password = "";   

const char* mqtt_server = "thingsboard.cloud"; 
const int mqtt_port = 1883;


const char* mqtt_user = ""; 
const char* mqtt_pass = ""; 

const char* topic_pub = "v1/devices/me/telemetry";
const char* topic_sub = "v1/devices/me/rpc/request/+"; 

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
bool coolerActivo = false;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
    delay(500);
    Serial.print(".");
  }
  
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("\n>>> WiFi Conectado IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n NO SE PUDO CONECTAR AL WIFI (Revisar Clave/Red)");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Mensaje recibido de Thingsboard: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a ThingsBoard...");
    String clientId = "Farmacia-" + String(random(0xffff), HEX);
    

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println(" ¡Conectado!");
      client.subscribe(topic_sub);
    } else {
      Serial.print(" fallo rc=");
      Serial.print(client.state());
      Serial.println(" reintentando en 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  randomSeed(micros());

  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 3000) { 
    lastMsg = now;

    long sumaLecturas = 0;
    for(int i = 0; i < 20; i++) {
      sumaLecturas += analogRead(LM35_PIN);
      delay(10); 
    }
    float lecturaPromedio = sumaLecturas / 20.0;
    float milivoltios = (lecturaPromedio * 3300.0) / 4095.0;
    float t = (milivoltios / 10.0); 
    
    Serial.print("Temperatura Actual: "); 
    Serial.println(t);


    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_AZUL, LOW); 

    if (t >= 20.0 && t <= 28.0) {
        digitalWrite(LED_VERDE, HIGH);
        Serial.println("--> Estado: OK (Verde)");
    } else {
        digitalWrite(LED_ROJO, HIGH);
        Serial.println("--> Estado: ALERTA (Rojo)");
    }

    String payload = "{\"temp\":" + String(t, 1) + "}";
    client.publish(topic_pub, payload.c_str());
  }
}