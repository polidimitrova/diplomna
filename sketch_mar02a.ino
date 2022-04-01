#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include <BlynkSimpleEsp32.h>



// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
#define BLYNK_TEMPLATE_ID "TMPLV0Dwz-wz"
#define BLYNK_DEVICE_NAME "Watering"
#define BLYNK_AUTH_TOKEN "DNMRjOnVaWthw6aTVHAqLLuS6YmjDU8w"

// Your WiFi credentials.
// Set password to "" for open networks.
char auth[] = "DNMRjOnVaWthw6aTVHAqLLuS6YmjDU8w";
char ssid[] = "Tarator";
char pass[] = "K@rune$h1";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"europe.pool.ntp.org"); 
//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


boolean state = false;
int moistureData = 0;

void setup()
{
  // Debug console
  Serial.begin(9600);
   
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(10800);
  Blynk.begin(auth, ssid, pass, "blynk.cloud",80);

 //time.setInterval(100L, humidity);
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection LOST");
  }
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);  

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute); 
   
  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);  

  String weekDay = weekDays[timeClient.getDay()];
  Serial.print("Week Day: ");
  Serial.println(weekDay);    

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  String currentMonthName = months[currentMonth-1];
  Serial.print("Month name: ");
  Serial.println(currentMonthName);

  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

  Serial.println("");

  delay(2000);
  Blynk.run();
  moistureData = digitalRead(2);
  if(moistureData == 1)
  {
    Blynk.notify("You need to water the plants!");
  }
 
}

BLYNK_WRITE(V1)
{
  if (state == false) {
    state = true;
    Blynk.notify("Congratulations! The plants are watered.");
    digitalWrite(13,HIGH);
    delay(10000);
  }
  else {
    state = false;
    digitalWrite(13,LOW);
  }
}
