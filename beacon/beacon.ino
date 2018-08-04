#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library

const char* ssid = "BeaconRoom1";
const char* password = "BeaconRoom1";
String device_Name = "beacon2";

//const char* ssid1 = "New Curiosity Gym";
//const char* password1 = "fthu@050318";


const char* host = "person-tracker.herokuapp.com";
const int httpsPort = 443;

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back
String message;
 
boolean deviceA1 = false;
boolean deviceA2 = false;
boolean newMessage = false;
int led=2;
void setup()
    {
      Serial.begin(115200);
      Serial.println();
      pinMode(led,OUTPUT);  
      WiFi.mode(WIFI_AP);
      // Serial.printf("Connecting to %s ", ssid);
      WiFi.softAP(ssid, password); 
      /*while (WiFi.status() != WL_CONNECTED)
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println(" connected");*/
      digitalWrite(led,HIGH);
      delay(1000);
      digitalWrite(led,LOW);
      Udp.begin(localUdpPort);
      Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString().c_str(), localUdpPort);
      //Serial.println("IP address: ");
      //Serial.println(WiFi.softAPIP());

      
              
 
    }

void updateHeroku(String device_id, String message)
    {
      digitalWrite(led,HIGH);
      
      newMessage = false;

      WiFi.mode(WIFI_STA);
      wifiMulti.addAP("New Curiosity Gym", "CuriosityTwinkle");   // add Wi-Fi networks you want to connect to
      wifiMulti.addAP("DT_LAB", "fthu@050318");
      wifiMulti.addAP("CGJioFi", "*8Twinkle8*");
      
      Serial.println("Connecting ...");
      int i = 0;
      while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
        delay(500);
        Serial.print('.');
       }
      Serial.println('\n');
      Serial.print("Connected to ");
      Serial.println(WiFi.SSID());              // Tell us what network we're connected to
     
          
               Serial.println("WiFi connected");
               WiFiClientSecure client;
               
               Serial.print("connecting to ");
               Serial.println(host);
               if (!client.connect(host, httpsPort)) {
                  Serial.println("connection failed");
                  return;
                 }

               String url = "/" + device_id + "/" + message;
               Serial.print("requesting URL: ");
               Serial.println(url);

               client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

                Serial.println("request sent"); 
                 WiFi.mode(WIFI_AP);
                WiFi.softAP(ssid, password); 
      
                Udp.begin(localUdpPort);
                Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString().c_str(), localUdpPort);
                digitalWrite(led,LOW);     
    }
      
void loop()
    { 
    
      int packetSize = Udp.parsePacket();
      if (packetSize)
         {
           Serial.print("#");
           // receive incoming UDP packets
           Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
           int len = Udp.read(incomingPacket, 255);
           if (len > 0)
              {
                incomingPacket[len] = 0;
              }
           message = incomingPacket;
           Serial.printf("UDP packet contents: %s\n", incomingPacket);

           newMessage = true;
           /*if(message == "A1")
              {
                deviceA1 = true;
                Serial.print("@");
              }
           if(message == "A2")
              {
                deviceA2 = true;
               
              }*/  
           // send back a reply, to the IP address and port we got the packet from
          // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
          // Udp.write(replyPacekt);
          // Udp.endPacket();
          }
       if(newMessage == true)
         {
          updateHeroku(device_Name, message);
         }
      
        
      }

