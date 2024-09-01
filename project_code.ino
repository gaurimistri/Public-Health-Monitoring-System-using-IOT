#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
String apiKey = "3W9Y4Y9GY4ZL1511";     //  Enter your Write API key here
const char *ssid =  "Rutujaa";     // Enter your WiFi Name
const char *pass =  "rutuja26"; // Enter your WiFi Password
const char* server = "api.thingspeak.com";
#define DHTPIN 14          // GPIO Pin where the dht11 is connected
 DHT dht(DHTPIN, DHT11);
 LiquidCrystal_I2C lcd (0x27, 16,2);
 const int PulseWire = A0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
 int bo = 0;
 int bo1=0;
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;
PulseSensorPlayground pulseSensor;
WiFiClient client;
void setup() {
  Serial.begin(115200);
 
  int  bo = analogRead(PulseWire);
   pulseSensor.analogInput(PulseWire);  
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);
 dht.begin();
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, pass);
       while (WiFi.status() != WL_CONNECTED)
     {
            delay(550);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
  }

void loop() {
 int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now.
  int  bo = analogRead(PulseWire);
if (pulseSensor.sawStartOfBeat())
{            // Constantly test to see if "a beat happened".
 Serial.println("♥️  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: "
 Serial.println(myBPM);
 Serial.println(bo);

                   

}
 float h = dht.readHumidity();
      float t = dht.readTemperature();
     
              if (isnan(h) || isnan(t))
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
                         if (client.connect(server,80))  
                      {  
                         bo1=bo/6.5;
                             // Serial.println(bo1);
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(bo1);
                             postStr +="&field3=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                             Serial.print("Temperature: ");
                             Serial.print(t);
                              Serial.print("humidity: ");
                             Serial.print(h);
                             
                             Serial.print(" MY BPM: ");
                             Serial.print(myBPM);
                           
                              Serial.println(bo1);
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
           Serial.println("Waiting...");
    delay(10000);
}
