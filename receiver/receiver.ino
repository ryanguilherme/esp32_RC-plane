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
static const int PITCH = 27;
static const int YAW = 26;

Servo pitch;
Servo yaw;

char dataReceived[32]; // this must match dataToSend in the TX
bool newData = false;

// global variables to store LY, RX, RT, DPAD, and BUTTON
int _index = -1;
unsigned int dpad = 0;
unsigned int buttons = 0;
int axisY = 0;
int axisRX = 0;
int throttle = 0;


//===CODE SECTION===
void setup() {
    Serial.begin(115200);

    //===SERVO SETUP===
    Serial.println("\nSetting up pitch and yaw servos...");
    pitch.attach(PITCH); // initialize pitch servo
    yaw.attach(YAW); // initialize yaw servo
    pitch.write(90); // positioning pitch servo in neutral position
    yaw.write(90); // positioning yaw servo in neutral position
    Serial.println("\nServos setup finished.");

    //===WIRELESS CONNECTION SETUP===
    Serial.println("\nSetting up NRF24L01...");
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);  // radio.setPALevel(RF24_PA_MIN);
    Serial.println("\nNR24L01 setup finished.");
}

//==============

void loop() {
    radio.startListening();
    while (radio.available()) {
    Serial.print("\nReceived: ");
    getData();
    //Serial.println(dataReceived);
    processData();
    //showData();
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
        Serial.println(dataReceived);
        newData = false;
    }
}

// process the received data
void processData() {

  for (int i = 0; i < strlen(dataReceived); i++) {
    if (dataReceived[i] == '\n' || dataReceived[i] == '\r') {
      dataReceived[i] = '\0';
    }
  }

  int parsed = sscanf(dataReceived, "0x%x|0x%x|%d|%d|%d|",
                      &dpad, &buttons, &axisY, &axisRX, &throttle);


  Serial.print("D-pad: ");
  Serial.print(dpad, HEX);

  Serial.print(" Buttons: ");
  Serial.print(buttons, HEX);

  Serial.print(" Axis Y: ");
  Serial.print(axisY);

  Serial.print(" Axis RX: ");
  Serial.print(axisRX);

  Serial.print(" Throttle: ");
  Serial.print(throttle);
}