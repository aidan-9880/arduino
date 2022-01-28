#include <ShiftDisplay2.h>
#include <WiFi.h>
#include "time.h"

//Time
const int tzOffset = -25200;
const int dstOffset = 3600;
const char *ntpServer = "pool.ntp.org";

//Photoresistor and PWM pin, lower = brighter
const int minBrightness = 150;
const int maxBrightness = 5;
const int tubeFreq = 100;
const int bAmp = 255 - minBrightness;
int prOut = 0;
int bValue = 0;


//WiFi
const char *ssid     = "";
const char *password = "";

//Declare
ShiftDisplay2 display(
  27, 26, 25,
  COMMON_CATHODE,
  6,
  STATIC_DRIVE
);

void setup() {
  Serial.begin(9600);

  //Setup dimming
  ledcSetup(0, tubeFreq, 8);
  ledcAttachPin(14, 0);
  

  //Setup wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
   }
  Serial.print(" WiFi connected. IP is: ");
  Serial.println(WiFi.localIP());

  //Setup time
  configTime(tzOffset, dstOffset, ntpServer);
  setenv("TZ", "PST8PDT,M3.2.0,M11.1.0", 1);
  tzset();
  Serial.println("Time client initialized. Testing:"); 
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %I:%M:%S");
}

void loop() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char theTime[9];
  strftime(theTime, 9, "%I%M%S", &timeinfo);
  display.set(theTime, ALIGN_RIGHT);
  display.update();

  prOut = analogRead(34);
  bValue = map(prOut, 0, 4095, 0, 255);
  if(bValue - bAmp <= maxBrightness) {
    ledcWrite(0, maxBrightness);
    //Serial.println(maxBrightness);
  }
  else {
    ledcWrite(0, bValue - bAmp);
    //Serial.println(bValue-bAmp);
  }
  
  delay(100);
}
