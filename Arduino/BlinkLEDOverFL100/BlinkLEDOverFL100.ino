#include <SPI.h>
#include <Ethernet.h>
#include "XPAPProtocol.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 160); // IP address, may need to change depending on network
unsigned int localPort = 24555;
char packetBuffer[XPAP_MAX_PACKET_SIZE]; 
EthernetUDP Udp;
int ledPin = 3;



uint32_t ntohl(uint32_t value) {
  uint32_t tmp_a = (value & 0xff000000) >> 24;
  uint32_t tmp_b = (value & 0x00ff0000) >> 8;
  uint32_t tmp_c = (value & 0x0000ff00) << 8 ;
  uint32_t tmp_d = (value & 0x000000ff) << 24;
  return tmp_d | tmp_c |tmp_b | tmp_a;
}

uint16_t ntohs(uint32_t value) {
  uint16_t tmp_a = (value & 0xff00) >> 8;
  uint16_t tmp_b = (value & 0x00ff) << 8;
  return tmp_b | tmp_a;
}

float getFloatValue(XPAPPacket_t* xpap_packet) {
  int32_t numerator = ntohl(xpap_packet->payload.fraction.numerator);
  int32_t denominator = ntohl(xpap_packet->payload.fraction.denominator);
  
  return (float)numerator / (float)denominator;
}

int getIntegerValue(XPAPPacket_t* xpap_packet) {
  return (int)ntohl(xpap_packet->payload.integer.value);
}

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
//    Serial.print("Received packet of size ");
//    Serial.println(packetSize);
//    Serial.print("From ");
//    IPAddress remote = Udp.remoteIP();
//    for (int i =0; i < 4; i++)
//    {
//      Serial.print(remote[i], DEC);
//      if (i < 3)
//      {
//        Serial.print(".");
//      }
//    }
//    Serial.print(", port ");
//    Serial.println(Udp.remotePort());
    
    Udp.read(packetBuffer,XPAP_MAX_PACKET_SIZE);
    
    
    XPAPPacket_t* xpap_packet = (XPAPPacket_t*)packetBuffer;
    
    uint32_t endpoint = ntohl(xpap_packet->endpoint);
    uint16_t payload_type = ntohs(xpap_packet->payload_type);
    uint16_t payload_size = ntohs(xpap_packet->payload_size);

    switch(endpoint)
    {
      case XPAPEndpointAutopilotMode:
      {
        int autopilot_mode = getIntegerValue(xpap_packet);
        break;
      } 
      case XPAPEndpointAutopilotAltitude:
      {
        float altitude = getFloatValue(xpap_packet);
        digitalWrite(ledPin, (altitude > 10000) ? HIGH : LOW);
        break;
      }
      default:
        break;
    }
  }
  
  delay(10);
}
