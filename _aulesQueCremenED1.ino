#include <DHT11.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

char token[] = ""; //Afegir el token generat pel sistema del lloc web d'Aules que cremen
char SSID[] = ""; //Afegir el nom de la xarxa WiFi a la que volem connectar (només xarxes de 2.4Ghz)
char wifiPass[] = ""; //Afegir la paraula de pas de la xarxa WiFi a la que volem connectar
char url[] = "https://aulesquecremen.cat/api/v1/readings";
DHT11 dht11(25);

void setup() {
    Serial.begin(9600);
    WiFi.begin(SSID, wifiPass);
    Serial.println("Intentant connectar al Wifi");
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
    }
    Serial.print("Connectat a ");
    Serial.print(SSID);
    Serial.print(" amb l'adreça IP ");
    Serial.println(WiFi.localIP());

}

void loop() {
    int temperatura = dht11.readTemperature();
    int humitat = dht11.readHumidity();
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print("ºC Humitat: ");
    Serial.print(humitat);
    Serial.println("%");
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + token);
    StaticJsonDocument<128> doc;
    doc["temperature"] = temperatura;
    doc["humidity"] = humitat;
    String request;
    serializeJson(doc, request);
    Serial.print("Intentant enviar les dades: ");
    Serial.println(request);
    int httpCode = http.POST(request);
    http.end();
    delay(60000);
}
