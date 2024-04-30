#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

const char* WIFI_SSID = "juan";
const char* WIFI_PASSWORD= "juan12345";

const char* API_KEY= "AIzaSyAmUcQSTukkoqAjdLzrzuPGUo3w1jwzvWY";
const char* DATABASE_URL= "https://sumativaap5-default-rtdb.firebaseio.com";
const char* USER_EMAIL= "sumativaap5@ionic.png";
const char* USER_PASSWORD= "juan12345";

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

const int ldrPin = 34; // Pin analógico para el sensor LDR

void setup_WIFI(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Conectado con IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setupFirebase() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);
  Serial.printf("Cliente de Firebase v%s\n\n", FIREBASE_CLIENT_VERSION);
}

void setup() {
  Serial.begin(115200);
  setup_WIFI();
  setupFirebase();
  pinMode(ldrPin, INPUT);
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis >= 1000 || sendDataPrevMillis == 0)) {
    // Leer el valor del sensor LDR
    int ldrValue = analogRead(ldrPin);
    
    // Determinar si es día, tarde o noche basado en el valor del sensor LDR
    String timePeriod;
    if (ldrValue < 3000) {
      timePeriod = "dia";
    } else if (ldrValue > 1000) {
      timePeriod = "noche";
    } else {
      timePeriod = "tarde";
    }

    // Enviar el valor del sensor LDR a la variable correspondiente en Firebase
    if (Firebase.RTDB.setString(&fbdo, "/tiempo", timePeriod)) {
      Serial.println("Variable de tiempo actualizada en Firebase");
    } else {
      Serial.println("Error al actualizar la variable de tiempo en Firebase: " + fbdo.errorReason());
    }

    // Actualizar el tiempo de envío de datos
    sendDataPrevMillis = millis();
  }
}