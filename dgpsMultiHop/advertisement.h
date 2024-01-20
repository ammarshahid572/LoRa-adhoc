
//LoRa.write is available to write a specific byte.
//LoRa.print is for sending a string
#include "routingTable.h"
byte id = 0x03;

void sendAdvertisement() {
  
  LoRa.beginPacket();
  LoRa.write(0x0F); // header for advertisement.
  LoRa.write(id);
  Serial.println("Sending advertisement as "+String(id));
  for (int i = 0; i < tableLength; i++) {
    if (destinations[i] == 0)break; //end of list
    if (nextHops[i]!= destinations[i]) continue; //dont advertise someone you dont know directly
    LoRa.write(destinations[i]);
    LoRa.write(costs[i]/2); //send 
  }
  LoRa.endPacket();
}

void decodeAdvertisement() {
  //LoRa.read() //first byte will tell message type. Generally it is read by previous function
  
  int cost = (-1)*LoRa.packetRssi();
  byte senderID = LoRa.read();
  Serial.println("Received Packet from "+String(senderID));
  while (LoRa.available()) {

    byte destination = LoRa.read();
    byte hopCost = LoRa.read();

    byte totalCost= hopCost+cost;
    if (destination == id) {
      //Serial.println("Route Loop back");
      continue; //ignore self as a route}
    }
    
    updateTable(senderID, destination, totalCost); //add extra cost as it is further
  }

  //lastly add the sender as a neighbour/route as well
  
  updateTable(senderID, senderID, cost);
}
