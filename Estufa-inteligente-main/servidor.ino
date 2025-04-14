#include <ESP8266WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>  // Biblioteca MQTT para ESP8266

#define DHTPIN D3         // Pino do sensor DHT11
#define DHTTYPE DHT11     // Tipo de sensor DHT
#define RELAY_PIN D4      // Pino do relé para a bomba
#define SOIL_MOISTURE_PIN A0  // Pino do sensor de umidade do solo

// Configurações de rede Wi-Fi
const char* ssid = "G88407";
const char* password = "senai88407";

// Configurações do Broker MQTT
//const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topicStatus = "SENAI/G88407/status";
const char* topicControl = "SENAI/G88407/control";
const char* topicStatuscontrol = "SENAI/G88407/statuscontrol";  // Tópico para status da bomba

// Instanciando os objetos necessários
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);  // Inicializando o sensor DHT

// Função para controlar o relé
void controlRelay(bool state) {
  if (state) {
    digitalWrite(RELAY_PIN, LOW);  // Liga o relé (supondo que LOW ativa o relé)
    Serial.println("Bomba ligada");
    client.publish(topicStatuscontrol, "ON");  // Publica o status "ON" da bomba
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Desliga o relé (supondo que HIGH desativa o relé)
    Serial.println("Bomba desligada");
    client.publish(topicStatuscontrol, "OFF");  // Publica o status "OFF" da bomba
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Inicialmente a bomba está desligada (relé desativado)

  Serial.println("inicializando o Wi-Fi");
  // Conectando ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
  Serial.println("Conectado ao Wi-Fi");

  // Configurando o cliente MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);

  // Conectando ao broker MQTT
  Serial.println("inicializando o broker MQTT");
  while (!client.connected()) {
    if (client.connect("EstufaCliente")) {
      Serial.println("Conectado ao broker MQTT");
      client.subscribe(topicControl);  // Inscrevendo no tópico de controle da bomba
    } else {
      delay(1000);
      Serial.println("...");
    }
  }
}

void loop() {
  // Verificando se há mensagens MQTT
  client.loop();

  // Publicando dados dos sensores periodicamente
  if (millis() % 10000 == 0) {  // A cada 10 segundos
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    int rawSoilMoisture = analogRead(SOIL_MOISTURE_PIN);

    // Converte o valor bruto de 0 a 1023 para porcentagem invertida (0% é seco, 100% é úmido)
    int soilMoisture = map(rawSoilMoisture, 1023, 0, 0, 100);

    // Criando o JSON com os dados
    String data = "{\"temperature\": " + String(temp) + 
                  ", \"humidity\": " + String(humidity) + 
                  ", \"soilMoisture\": " + String(soilMoisture) + "}" ;

    // Publicando os dados no tópico de status
    client.publish(topicStatus, data.c_str());
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // Comandos para ligar ou desligar a bomba
  if (message == "ON") {
    controlRelay(true);  // Liga a bomba
  }
  if (message == "OFF") {
    controlRelay(false);  // Desliga a bomba
  }
}
