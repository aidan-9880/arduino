#include <ShiftDisplay2.h>
#include <WiFi.h>
#include <NTPClient.h>

//Setup time
const int offsetInSeconds = -25200;

//Setup photoresistor and PWM pin
const int minBrightness = 2000;
const int tubeFreq = 100;
int prOut = 0;

//Setup wifi
const char *ssid     = "Outside Open";
const char *password = "cylonbase4starswar";

//Declare
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
ShiftDisplay2 display(
  27, 26, 25,
  COMMON_CATHODE,
  6,
  STATIC_DRIVE
);

void setup() {
  Serial.begin(115200);

  ledcSetup(0, tubeFreq, 12);
  ledcAttachPin(14, 0);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
   }
  Serial.print(" WiFi connected. IP is: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.setTimeOffset(offsetInSeconds);
  Serial.print("Time client initialized. Right now, it's ");
  timeClient.update();
  Serial.println(timeClient.getFormattedDate());
}

void loop() {
  timeClient.update();
  
  int h = (timeClient.getHours()*10000);
  int m = (timeClient.getMinutes()*100);
  int s = (timeClient.getSeconds());

/*
  prOut = analogRead(34);
  if(prOut >= minBrightness) {
    ledcWrite(0, prOut-minBrightness);
    Serial.println(prOut-minBrightness);
  }
  else { 
    ledcWrite(0, prOut);
    Serial.println(prOut);
  }
*/
  ledcWrite(0, 800);
  
  if(h <= 120000) {
    display.set(h+m+s, 0, true, ALIGN_RIGHT);
    display.update();
  }
  else {
    display.set((h-120000)+m+s, 0, true, ALIGN_RIGHT);
    display.update();
  }
  
  delay(100);
}
