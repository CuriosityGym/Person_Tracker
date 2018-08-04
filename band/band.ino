#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library


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
  
  String value = "A2";
 value.toCharArray(udpBuffer,UDP_PACKET_SIZE);
 //Serial.println(udpBuffer); 
  udp.beginPacket(ipBroadCast, udpRemotePort);
  udp.write(udpBuffer, sizeof(udpBuffer));
  udp.endPacket();
  }

void setup() {
  Serial.begin(115200);
 
  
     //Serial.println();
    // delay(100);
     //Serial.println('\n');
     WiFi.mode(WIFI_STA);
     wifiMulti.addAP("BeaconRoom1", "BeaconRoom1");   // add Wi-Fi networks you want to connect to
     wifiMulti.addAP("BeaconRoom2", "BeaconRoom2");
     wifiMulti.addAP("BeaconRoom3", "BeaconRoom3");
     
     Serial.println("Connecting ...");
     int i = 0;
     while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
      delay(500);
      Serial.print('.');
     }
     Serial.println('\n');
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


void loop() {

   
  }

