#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* ssid = "NOME REDE WIFI";
const char* senha = "SENHA WIFI;

const char* baseUrl = "https://teste-q43i.onrender.com/";
char urlTemperatura[100]; 
char urlUmidade[100]; 
float temperatura, umidade; 

#define DHT22_PINO 23
#define DHT_TIPO DHT22

DHT dht(DHT22_PINO, DHT_TIPO);
HTTPClient http;

void setup() {
  Serial.begin(9600);
  delay(1000);

  dht.begin();

  // Conectar à rede Wi-Fi
  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi!");
  
}

void getTemperaturaUmidadeAtual() {
  umidade = dht.readHumidity();   
  temperatura = dht.readTemperature(); 
}

void createTemperatura() {
  sprintf(urlTemperatura, "%stemperatura/createTemperaturaIOT?temperatura=%.1f&unidadeMedida=C",baseUrl,  temperatura);
  http.begin(urlTemperatura);
  int httpCode = http.POST("");
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.println("Falha na solicitação HTTP");
  }
  http.end();
}

void createUmidade() {
  sprintf(urlUmidade, "%sumidade/createUmidadeIOT?umidade=%.1f&unidadeMedida=%s",baseUrl, umidade,"%"); // Corrigido o formato do URL
  http.begin(urlUmidade);
  int httpCode = http.POST("");
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.println("Falha na solicitação HTTP");
  }
  http.end();
}

void loop() {
  
    getTemperaturaUmidadeAtual();
    createTemperatura();
    createUmidade();
  
  delay(900000);  // Aguarda 5 segundos antes de fazer outra leitura
}
