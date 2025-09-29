#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "3-JAGOANKU"
#define WIFI_PASSWORD "janganlupabismillah"
#define API_KEY "AIzaSyC0A3jnD9DZ6GNPFZe3zsc0Gv_ityKVM6U"
#define DATABASE_URL "https://praktikum-1-613a3-default-rtdb.firebaseio.com/" 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

int LED_RED = 13;
int LED_YELLOW = 12;
int LED_GREEN = 14;

unsigned long sendDataPrevMillis = 0;

bool signupOK = false;

void setup(){
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
    }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
    }
    else{
  Serial.printf("%s\n",config.signer.signupError.message.c_str());
  }
  
  config.token_status_callback = tokenStatusCallback;  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);  
}

void otomatis(){
    Serial.println("Mode Otomatis");                                     
    digitalWrite(LED_GREEN, HIGH); 
    digitalWrite(LED_YELLOW, LOW); 
    digitalWrite(LED_RED, LOW);
    Firebase.RTDB.setString(&fbdo, "LED/GREEN", "1");
    Firebase.RTDB.setString(&fbdo, "LED/YELLOW", "0");
    Firebase.RTDB.setString(&fbdo, "LED/RED", "0");
    delay (10000);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    Firebase.RTDB.setString(&fbdo, "LED/RED", "0");
    Firebase.RTDB.setString(&fbdo, "LED/YELLOW", "1");
    Firebase.RTDB.setString(&fbdo, "LED/GREEN", "0");
    delay (10000);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    Firebase.RTDB.setString(&fbdo, "LED/GREEN", "0");
    Firebase.RTDB.setString(&fbdo, "LED/YELLOW", "0");
    Firebase.RTDB.setString(&fbdo, "LED/RED", "1");
    delay (10000);
}

void manual(){
    Serial.println("Mode Manual");
    Firebase.RTDB.getString(&fbdo, "LED/RED");
    String red = fbdo.stringData();
    if (red == "1") {
    Serial.println("LED_RED Menyala");                                     
    digitalWrite(LED_RED, HIGH);  
    }  else if (red == "0")  {                                   
    Serial.println("LED RED Mati");
    digitalWrite(LED_RED, LOW);
    } else {
    Serial.println(fbdo.errorReason());}

    Firebase.RTDB.getString(&fbdo, "LED/YELLOW");
    String yellow = fbdo.stringData();
    if (yellow == "1") {
    Serial.println("LED_YELLOW Menyala");                                     
    digitalWrite(LED_YELLOW, HIGH);  
    }  else if (yellow == "0")  {                                   
    Serial.println("LED_YELLOW Mati");
    digitalWrite(LED_YELLOW, LOW);
    } else {
    Serial.println(fbdo.errorReason());}

    Firebase.RTDB.getString(&fbdo, "LED/GREEN");
    String green = fbdo.stringData();
    if (green == "1") {
    Serial.println("LED_GREEN Menyala");                                     
    digitalWrite(LED_GREEN, HIGH);  
    }  else if (green == "0")  {                                   
    Serial.println("LED_GREEN Mati");
    digitalWrite(LED_GREEN, LOW);
    } else {
    Serial.println(fbdo.errorReason());}
    delay (1000);
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() -   sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

 Firebase.RTDB.getString(&fbdo, "LED/Mode");
    String mode = fbdo.stringData();
    if (mode == "1") {
      otomatis();
    }  else if (mode == "0")  {
      manual();
    } else {
      Serial.println(fbdo.errorReason());
      }
  delay(1000);
}
}
