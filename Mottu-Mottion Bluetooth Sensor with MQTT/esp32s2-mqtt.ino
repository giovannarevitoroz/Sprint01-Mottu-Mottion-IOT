#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// Configurações de WiFi
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";  

// Configurações de MQTT
const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "esp32_mqtt";
const char *TOPIC_SUBSCRIBE_LED = "iot/mottu-mottion/status";
const char *TOPIC_PUBLISH_TEMP_HUMI = "iot/mottu-mottion/sensormo";

// Configurações de Hardware
#define PIN_LED_VERMELHO 15
#define PIN_LED_AZUL 2
#define PIN_LED_VERDEC 4
#define PIN_LED_VERDEESCURO 5 
#define PIN_LED_AMARELO 18
#define PIN_LED_CINZA 19
#define PIN_LED_ROXO 21
#define PIN_LED_BORDO 22

#define PUBLISH_DELAY 2000

// Variáveis globais
WiFiClient espClient;
PubSubClient MQTT(espClient);
unsigned long publishUpdate = 0;
const int TAMANHO = 200;

// Definição da moto
const char* id_sensor = "01111";
const char* id_moto = "45124";
const char* setor_moto = "PATIO";
const char* observacao_moto = "Agendada para manutenção";

// Protótipos de funções
void initWiFi();
void initMQTT();
void callbackMQTT(char *topic, byte *payload, unsigned int length);
void reconnectMQTT();
void reconnectWiFi();
void checkWiFIAndMQTT();
void publishData(const char* observacao);

// Publica os dados (sem temperatura e umidade)
void publishData(const char* observacao) {
  StaticJsonDocument<TAMANHO> doc;
  doc["id_sensor"] = id_sensor;
  doc["id_moto"] = id_moto;
  doc["setor"] = setor_moto;
  doc["observacao"] = observacao;
  doc["timestamp"] = millis();

  char buffer[TAMANHO];
  serializeJsonPretty(doc, buffer);
  MQTT.publish(TOPIC_PUBLISH_TEMP_HUMI, buffer);

  Serial.println("========== DADOS PUBLICADOS ==========");
  Serial.println(buffer);
  Serial.println("=====================================");
}

void initWiFi() {
  Serial.print("Conectando com a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}

void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  String msg = String((char*)payload).substring(0, length);
  Serial.print("Mensagem recebida: ");
  Serial.println(msg);

  StaticJsonDocument<TAMANHO> json;
  DeserializationError error = deserializeJson(json, msg);

  if (error) {
    Serial.println("Erro ao ler JSON.");
    return;
  }

  if (json.containsKey("led")) {
    String cor = json["led"];
    Serial.print("Comando para LED: ");
    Serial.println(cor);

    // Desliga todos os LEDs
    digitalWrite(PIN_LED_VERMELHO, LOW);
    digitalWrite(PIN_LED_AZUL, LOW);
    digitalWrite(PIN_LED_VERDEC, LOW);
    digitalWrite(PIN_LED_VERDEESCURO, LOW);
    digitalWrite(PIN_LED_AMARELO, LOW);
    digitalWrite(PIN_LED_CINZA, LOW);
    digitalWrite(PIN_LED_ROXO, LOW);
    digitalWrite(PIN_LED_BORDO, LOW);

    String observacao;

    if (cor == "vermelho") {
      digitalWrite(PIN_LED_VERMELHO, HIGH);
      observacao = "Danos Estruturais";
    } else if (cor == "azul") {
      digitalWrite(PIN_LED_AZUL, HIGH);
      observacao = "Reparos Simples";
    } else if (cor == "verdec") {
      digitalWrite(PIN_LED_VERDEC, HIGH);
      observacao = "Minha Mottu (Verde Claro)";
    } else if (cor == "verdeescuro") {
      digitalWrite(PIN_LED_VERDEESCURO, HIGH);
      observacao = "Pronta para aluguel";
    } else if (cor == "amarelo") {
      digitalWrite(PIN_LED_AMARELO, HIGH);
      observacao = "Pendência";
    } else if (cor == "cinza") {
      digitalWrite(PIN_LED_CINZA, HIGH);
      observacao = "Agendada para manutenção";
    } else if (cor == "roxo") {
      digitalWrite(PIN_LED_ROXO, HIGH);
      observacao = "Sem placa";
    } else if (cor == "bordo") {
      digitalWrite(PIN_LED_BORDO, HIGH);
      observacao = "Motor com defeito";
    } else {
      observacao = "Desconhecido";
    }

    publishData(observacao.c_str());
  }
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar ao broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");
      MQTT.subscribe(TOPIC_SUBSCRIBE_LED);
    } else {
      Serial.println("Falha. Tentando novamente em 2 segundos.");
      delay(2000);
    }
  }
}

void checkWiFIAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}

void reconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("WiFi reconectado com sucesso: ");
  Serial.println(SSID);
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n========== INICIALIZANDO SISTEMA ==========");

  // Configura os pinos como saída
  pinMode(PIN_LED_VERMELHO, OUTPUT);
  pinMode(PIN_LED_AZUL, OUTPUT);
  pinMode(PIN_LED_VERDEC, OUTPUT);
  pinMode(PIN_LED_VERDEESCURO, OUTPUT);
  pinMode(PIN_LED_AMARELO, OUTPUT);
  pinMode(PIN_LED_CINZA, OUTPUT);
  pinMode(PIN_LED_ROXO, OUTPUT);
  pinMode(PIN_LED_BORDO, OUTPUT);

  // Desliga todos os LEDs
  digitalWrite(PIN_LED_VERMELHO, LOW);
  digitalWrite(PIN_LED_AZUL, LOW);
  digitalWrite(PIN_LED_VERDEC, LOW);
  digitalWrite(PIN_LED_VERDEESCURO, LOW);
  digitalWrite(PIN_LED_AMARELO, LOW);
  digitalWrite(PIN_LED_CINZA, LOW);
  digitalWrite(PIN_LED_ROXO, LOW);
  digitalWrite(PIN_LED_BORDO, LOW);

  initWiFi();
  initMQTT();

  Serial.println("Sistema inicializado com sucesso!");
  Serial.println("=====================================");
}

void loop() {
  checkWiFIAndMQTT();
  MQTT.loop();

  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();
    publishData(observacao_moto);
    digitalWrite(PIN_LED_CINZA, HIGH);
  }
}
