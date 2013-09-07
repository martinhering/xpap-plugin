#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 160); // IP address, may need to change depending on network
unsigned int localPort = 24555;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; 
EthernetUDP Udp;
int ledPin = 3;

typedef struct {
    uint32_t            endpoint;
    uint16_t            payload_type;
    uint16_t            payload_size;
    union {
      uint32_t integer;
    } payload;
} XPAPUDPHeader_t;

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
    //Serial.print("Received packet of size ");
    //Serial.println(packetSize);
    //Serial.print("From ");
//    IPAddress remote = Udp.remoteIP();
//    for (int i =0; i < 4; i++)
//    {
//      Serial.print(remote[i], DEC);
//      if (i < 3)
//      {
//        Serial.print(".");
//      }
//    }
    //Serial.print(", port ");
    //Serial.println(Udp.remotePort());
    
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    
    
    XPAPUDPHeader_t* xpap_packet = (XPAPUDPHeader_t*)packetBuffer;
    
    uint32_t endpoint = ntohl(xpap_packet->endpoint);
    //Serial.println("Endpoint:");
    //Serial.println(endpoint);
    
    uint16_t payload_type = ntohs(xpap_packet->payload_type);
    //Serial.println("Payload Type:");
    //Serial.println(payload_type);
    
    uint16_t payload_size = ntohs(xpap_packet->payload_size);
    //Serial.println("Paylpad Size:");
    //Serial.println(payload_size);
    
    if (payload_type == 1 && payload_size == 4)
    {
      if (endpoint == 1) {
        uint32_t altitude = ntohl(xpap_packet->payload.integer);
        
        digitalWrite(ledPin, (altitude > 10000) ? HIGH : LOW);
//        Serial.println("Altitude:");
//        Serial.println(altitude);
      }
    }
  }
  delay(10);
}
