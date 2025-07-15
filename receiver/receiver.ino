//NRF24L01 Wireless Control
//Receiver code

//===LIBRARIES===
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP32Servo.h>

//===MACROS===
RF24 radio(2, 4);  // CE, CSN
const byte address[6] = "00001";
boolean switchstate = 0;
static const int PITCH;
static const int YAW;

Servo pitch;
Servo yaw;

char dataReceived[32]; // this must match dataToSend in the TX
bool newData = false;


//===CODE SECTION===
void setup() {
    Serial.begin(115200);

    //===SERVO SETUP===
    Serial.println("\nSetting up pitch and yaw servos...")
    pitch.attatch(PITCH); // initialize pitch servo
    yaw.attatch(YAW); // initialize yaw servo
    pitch.write(90); // positioning pitch servo in neutral position
    yaw.write(90); // positioning yaw servo in neutral position
    Serial.println("\nServos setup finished.")

    //===WIRELESS CONNECTION SETUP===
    Serial.println("\nSetting up NRF24L01...");
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);  // radio.setPALevel(RF24_PA_MIN);
    Serial.println("\nNR24L01 setup finished.")
}

//==============

void loop() {
    radio.startListening();
    while (radio.available()) {
    Serial.print("\nReceived");
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

//==============

void showData()
{
    if (newData == true)
    {
        Serial.print("[RECEIVED DATA]: ");
        Serial.println(dataReceived);
        newData = false;
    }
}