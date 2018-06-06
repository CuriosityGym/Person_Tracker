#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ArduinoOTA.h>

IPAddress ipBroadCast(0, 0, 0, 0);
unsigned int udpRemotePort=4210;
unsigned int udplocalPort=2392;
const int UDP_PACKET_SIZE = 48;
char udpBuffer[ UDP_PACKET_SIZE];
WiFiUDP udp;
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

const char* ssid = "New Curiosity Gym";
const char* password = "CuriosityTwinkle";
int button = 2;
int buttonState;
bool sleepMode = true;
//================================================================
// Setup hardware, serial port, and connect to wifi.
//================================================================

// unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
//char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back

//extern "C" {
//#include "user_interface.h" // this is for the RTC memory read/write functions
//}

void fncUdpSend()
{
  
  String value = "A1";
 value.toCharArray(udpBuffer,UDP_PACKET_SIZE);
 //Serial.println(udpBuffer); 
  udp.beginPacket(ipBroadCast, udpRemotePort);
  udp.write(udpBuffer, sizeof(udpBuffer));
  udp.endPacket();
  }

void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);
  buttonState = digitalRead(button);
  delay(100);
  buttonState = digitalRead(button);
 if(buttonState == 1)
    { 
      sleepMode = true;
    }
   if(buttonState == 0)
    { 
        sleepMode = false;
    }
  if(buttonState == 0)
    {
      Serial.begin(115200);
      Serial.println("Booting");
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
      }

     // Port defaults to 8266
     // ArduinoOTA.setPort(8266);

     // Hostname defaults to esp8266-[ChipID]
     // ArduinoOTA.setHostname("myesp8266");

     // No authentication by default
     // ArduinoOTA.setPassword("admin");

     // Password can be set with it's md5 value as well
     // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
     // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

     ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
      else // U_SPIFFS
      type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
     });
     ArduinoOTA.onEnd([]() {
     Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    }
  if(buttonState == 1)
    {  
     
     //Serial.println();
    // delay(100);
     //Serial.println('\n');
     WiFi.mode(WIFI_STA);
    // wifiMulti.addAP("BeaconRoom1", "BeaconRoom1");   // add Wi-Fi networks you want to connect to
    // wifiMulti.addAP("BeaconRoom2", "BeaconRoom2");
    // wifiMulti.addAP("BeaconRoom3", "BeaconRoom3");
     WiFi.begin("BeaconRoom1", "BeaconRoom1");
      while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
     Serial.println("Connecting ...");
     int i = 0;
    // while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    //  delay(500);
    //  Serial.print('.');
    // }
     //Serial.println('\n');
     Serial.print("Connected to ");
     Serial.println(WiFi.SSID());              // Tell us what network we're connected to
     
     udp.begin(udplocalPort);
     
     fncUdpSend();
     int packetSize = udp.parsePacket();
      if (packetSize)
         {
           
           // receive incoming UDP packets
           // Serial.printf("Received %d bytes from %s, port %d\n", packetSize, udp.remoteIP().toString().c_str(), Udp.remotePort());
           int len = udp.read(incomingPacket, 255);
           if (len > 0)
              {
                incomingPacket[len] = 0;
              }
           Serial.printf("UDP packet contents: %s\n", incomingPacket);
          }
      Serial.println("before sleep");
  
      ESP.deepSleep(18000000, WAKE_RFCAL);
  }


}
void loop() {
  if(buttonState == 0){
  ArduinoOTA.handle();
  }
   
  }

