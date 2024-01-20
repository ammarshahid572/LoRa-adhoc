/*
  LoRa Duplex communication

  Sends a message every half second, and polls continually
  for new incoming messages. Implements a one-byte addressing scheme,
  with 0xFF as the broadcast address.

  Uses readString() from Stream class to read payload. The Stream class'
  timeout may affect other functuons, like the radio's callback. For an

  created 28 April 2017
  by Tom Igoe
*/
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include "advertisement.h"
const int csPin = 7;          // LoRa radio chip select
const int resetPin = 6;       // LoRa radio reset
const int irqPin = 1;         // change for your board; must be a hardware interrupt pin

#ifdef ESP32
#define LORA_MISO 19
#define LORA_CS 18
#define LORA_MOSI 27
#define LORA_SCK 5
#define LORA_RST 14
#define LORA_IRQ 26
#define ledpin 13
#else
#define LORA_CS 10
#define LORA_RST 9
#define LORA_IRQ 2
#endif


String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xAB;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends


bool msgToSend=false;

void setup() {
  Serial.begin(9600);                   // initialize serial
  while (!Serial);

  Serial.println("LoRa Duplex");
#ifdef ESP32
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
#endif
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);

  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
}

void loop() {
  if (millis() - lastSendTime > interval) {
    //String message = "HeLoRa World!";   // send a message
    //sendMessage(message);
    sendAdvertisement();
    //Serial.println("Sending " + message);
    lastSendTime = millis();            // timestamp the message
    interval = random(2000) + 1000;    // 2-3 seconds
    updateTimeouts();
  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(0x0F); // header for advertisement.
  LoRa.write(id);
  // for (int i = 0; i < tableLength; i++) {
  //   if (destinations[i] == 0)break; //end of list
  //   LoRa.write(destinations[i]);
  // }
  LoRa.beginPacket();            // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
  int packetType = LoRa.read();
  Serial.println("Received a Packet");
  switch (packetType) {
    case 0x0F:
      decodeAdvertisement();
      printTable();
      break;

    default:
      Serial.println("Unsupported Packet Type");
  }
}



void csma_detect
