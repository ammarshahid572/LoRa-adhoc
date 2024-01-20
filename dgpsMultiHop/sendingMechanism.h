#include <LoRa.h>

struct dataPacket {
  byte packetType;  // 0x0F for advertisement, 0xF0 for Data, 0xFF for Alert
  byte senderId; 
  byte receiverId;
  byte hopID;
  float latitude;
  float longitude;
};

struct alertPacket {
  byte packetType;
  byte senderId; 
  byte receiverId;
  byte hopID;
  byte alertType; //ranges between 0x01 to 0x07
};

void sendData(dataPacket data) {
  LoRa.beginPacket();
  LoRa.write(data.packetType);
  LoRa.write(data.senderId);
  LoRa.write(data.receiverId);
  LoRa.write(data.hopID);

  // Convert double to float before sending
  float latFloat = static_cast<float>(data.latitude);
  float lonFloat = static_cast<float>(data.longitude);

  LoRa.write(reinterpret_cast<byte*>(&latFloat), sizeof(latFloat));
  LoRa.write(reinterpret_cast<byte*>(&lonFloat), sizeof(lonFloat));

  LoRa.endPacket();
}

void receiveData() {
  if (LoRa.parsePacket() > 0) {
    dataPacket receivedData;

     //receivedAlert.packetType = LoRa.read(); //packet type already read in prev function
    receivedData.senderId = LoRa.read();
    receivedData.receiverId = LoRa.read();
    receivedData.hopID = LoRa.read();

    float latFloat;
    float lonFloat;

    LoRa.read(reinterpret_cast<byte*>(&latFloat), sizeof(latFloat));
    LoRa.read(reinterpret_cast<byte*>(&lonFloat), sizeof(lonFloat));

    receivedData.latitude = static_cast<double>(latFloat);
    receivedData.longitude = static_cast<double>(lonFloat);

    // Process receivedData as needed
  }
}

struct alertPacket {
  byte packetType;
  byte senderId;
  byte receiverId;
  byte hopID;
  byte alertType; //ranges between 0x01 to 0x07
};

void sendAlert(alertPacket alertData) {
  LoRa.beginPacket();
  LoRa.write(alertData.packetType);
  LoRa.write(alertData.senderId);
  LoRa.write(alertData.receiverId);
  LoRa.write(alertData.hopID);
  LoRa.write(alertData.alertType);

  LoRa.endPacket();
}

void receiveAlert() {
  if (LoRa.parsePacket() > 0) {
    alertPacket receivedAlert;

    //receivedAlert.packetType = LoRa.read(); //packet type already read in prev function
    receivedAlert.senderId = LoRa.read();
    receivedAlert.receiverId = LoRa.read();
    receivedAlert.hopID = LoRa.read();
    receivedAlert.alertType = LoRa.read();

    // Process receivedAlert as needed
  }
}



void receivePacket() {
  if (LoRa.parsePacket() > 0) {
    byte packetType = LoRa.read();

    switch (packetType) {
      case 0x0F:
        //receiveAdvertisement();
        decodeAdvertisement(); 
        break;
      case 0xF0:
        receiveData();
        break;
      case 0xFF:
        receiveAlert();
        break;
      default:
        // Handle unknown packet type
        break;
    }
  }
}
