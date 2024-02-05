#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <Ultrasonic.h>
#include <WiFiManager.h>

#define API_KEY "AIzaSyC98sv3R-fl0zcKBdboOjnIGzdkX1x-QSQ"
#define DATABASE_URL "https://fyp-sltc-d4569-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "mihisankasandudeeptha@gmail.com"
#define USER_PASSWORD "Sandudeeptha@98"

#define echoPin 18
#define trigPin 5

FirebaseData fbdo;
FirebaseAuth auth; 
FirebaseConfig config;

Ultrasonic ultrasonic; // Initialize Ultrasonic sensor object without pins

bool carDetected = false;
unsigned long sendDataPrevMillis = 0;
String slotName = "Slot1";

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  ultrasonic.begin(trigPin, echoPin); // Initialize Ultrasonic sensor pins

  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP"); 

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
}

void loop() {
  long duration = ultrasonic.timing(); // Use Ultrasonic library's timing function
  float distance = ultrasonic.convert(distance, Ultrasonic::CM);

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance <= 100) {
    carDetected = true;
  } else {
    carDetected = false;
  }
  Serial.print("Status: ");
  Serial.println(carDetected);

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    Firebase.setBool(fbdo, "/slot1", carDetected);
  }
}
