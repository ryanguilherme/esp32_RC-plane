//NRF24L01 PA/LNA Wireless LED Control
//Receiver code
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(2, 4);  // CE, CSN
const byte address[6] = "00001";
boolean switchstate = 0;

char dataReceived[32]; // this must match dataToSend in the TX
bool newData = false;

void setup() {
  Serial.begin(115200);
  Serial.println("\n[BOOTING RECEIVER ESP32]");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);  // radio.setPALevel(RF24_PA_MIN);
}
void loop() {
  radio.startListening();
  while (radio.available()) {
    //radio.read(&switchstate, sizeof(switchstate));
    Serial.print("\nReceived");
    // if (switchstate == HIGH) {
    //   digitalWrite(led, HIGH);
    //   Serial.print(" HIGH Signal");
    // } else {
    //   digitalWrite(led, LOW);
    //   Serial.print(" LOW Signal");
    // }
    getData();
    showData();
  }
}

//==============

void getData()
{
    radio.read( &dataReceived, sizeof(dataReceived) );
    newData = true;
}

void showData()
{
   if (newData == true)
   {
      Serial.print("[RECEIVED DATA]: ");
      Serial.println(dataReceived);
      newData = false;
   }
}