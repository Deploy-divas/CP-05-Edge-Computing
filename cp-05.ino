#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// --- Pinos e Sensores ---
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
const int ldrPin = 34;

// --- Configurações Wi-Fi e MQTT ---
const char* ssid = "Wokwi-GUEST"; 
const char* password = ""; 
const char* mqtt_server = "broker.emqx.io"; 
const int mqtt_port = 1883;
const char* id_mqtt = "fiware_080";
const char* topicPrefix = "device080";

WiFiClient espClient;
PubSubClient client(espClient);

// --- Tópicos MQTT ---
const char* topic_publish = "/TEF/device080/attrs";
const char* topic_subscribe = "/TEF/device080/cmd";

// --- Vinho ---
float tempMin = 12.0; 
float tempMax = 16.0;
float umidMin = 50.0;
float umidMax = 75.0; 
int luzMax = 200;    

unsigned long lastMsg = 0;
const long interval = 5000; 

// ---MQTT ---
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        msg += (char)payload[i];
    }
    Serial.print("- Comando recebido do MyMQTT: ");
    Serial.println(msg);
}

// --- Reconexão MQTT ---
void reconnect() {
    while (!client.connected()) {
        Serial.print("Conectando ao MQTT...");
        // Cria um ID único para o Wokwi não derrubar a conexão de outros alunos
        String clientId = String(id_mqtt) + "_" + String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str())) { 
            Serial.println("Conectado!");
            client.subscribe(topic_subscribe); // Assina para ouvir os comandos
        } else {
            Serial.print("Falha, rc=");
            Serial.print(client.state());
            Serial.println(" Retentando em 5s...");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Conexão Wi-Fi
    Serial.print("Conectando na rede: ");
    WiFi.mode(WIFI_STA);
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado!");

    // Configuração MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqtt_callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop(); 

    unsigned long now = millis();
    if (now - lastMsg > interval) {
        lastMsg = now;

        // Leitura
        float temperatura = dht.readTemperature();
        float umidade = dht.readHumidity();
        int luminosidade = analogRead(ldrPin);

        if (isnan(temperatura) || isnan(umidade)) {
            Serial.println("Falha ao ler o sensor DHT!");
            return;
        }

        String status = "";
        
        if (temperatura < tempMin) status += "FRIO_DEMAIS ";
        else if (temperatura > tempMax) status += "QUENTE_DEMAIS ";

        if (umidade < umidMin) status += "SECO_DEMAIS ";
        else if (umidade > umidMax) status += "UMIDO_DEMAIS ";

        if (luminosidade > luzMax) status += "LUZ_ALTA "; 
        
        if (status == "") status = "AMBIENTE_OK ";

        String tempStr = String(temperatura, 2);
        String umidStr = String(umidade, 1);
        String luzStr = String(luminosidade);

        String payload = "t|" + String(temperatura, 2) + 
                         "|h|" + String(umidade, 1) + 
                         "|l|" + String(luminosidade) + 
                         "|st|" + status;

        //publica
        Serial.println("Enviando dados FIWARE: " + payload);
        client.publish(topic_publish, payload.c_str());
    }
}