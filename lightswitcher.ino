#include <ESP32Servo.h>
#include "arduino_secrets.h"
#include <WiFi.h>
#include <HTTPClient.h>
 
Servo servo;
int servoPin = 22;
const char* ssid = SSIDVariable;
const char* password =  PASSWRDVariable ;
int old_status = 0;
void setup() {
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}

void servo_move(int pos){
  servo.setPeriodHertz(50); // standard 50 hz servo
  servo.attach(servoPin, 1000, 2000);// Attach the servo after it has been detatched
  Serial.println(pos);
  delay(15);
  if (pos == 0){
    servo.write(150);
    delay(1000);
  }
  else{
    servo.write(0);
    delay(1000);
  }
  
}

int isSubstring(String s1, String s2) 
{ 
    int M = s1.length(); 
    int N = s2.length(); 
  
    /* A loop to slide pat[] one by one */
    for (int i = 0; i <= N - M; i++) { 
        int j; 
  
        /* For current index i, check for pattern match */
        for (j = 0; j < M; j++) 
            if (s2[i + j] != s1[j]) 
                break; 
  
        if (j == M) 
            return i; 
    } 
  
    return -1; 
} 

void loop() {
  int new_status = old_status;
  //Serial.print("old status ");
  //Serial.println(old_status);
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin("http://153.33.16.159:5000/lightswitch/overhead/get"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        const char* payload = http.getString().c_str();
        //Serial.println(httpCode);
        if (httpCode == 200){
          if (isSubstring("on", payload) > 0){
            new_status = 1;
          }
          else{
            new_status = 0;
          }
        //Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
  //Serial.print("new status ");
  //Serial.println(new_status);
  if (new_status != old_status){
    old_status = new_status;
    servo_move(old_status);
  }
  servo.detach();
  delay(1000);
}

}
