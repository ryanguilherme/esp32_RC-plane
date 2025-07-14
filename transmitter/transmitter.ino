//NRF24L01 PA/LNA Wireless LED Control
//Transmitter code
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define button 25

RF24 radio(2, 4);  // CE, CSN

const byte address[6] = "00001";
int switchstate = 0;

char dataToSend[10] = "Message 0";
char txNum = '0';

void setup() {
  Serial.begin(115200);
  Serial.println("\nTX Circuit");
  pinMode(button, INPUT);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
}
void loop() {
  switchstate=digitalRead(button);
  radio.stopListening();
  switchstate = !switchstate;
  //radio.write(&switchstate, sizeof(switchstate));
  send();
}

//====================

void send() {

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    Serial.print(dataToSend);
    if (rslt) {
        Serial.println("  Acknowledge received");
        updateMessage();
    }
    else {
        Serial.println("  Tx failed");
    }
}

//================

void updateMessage() {
        // so you can see that new data is being sent
    txNum += 1;
    if (txNum > '9') {
        txNum = '0';
    }
    dataToSend[8] = txNum;
}