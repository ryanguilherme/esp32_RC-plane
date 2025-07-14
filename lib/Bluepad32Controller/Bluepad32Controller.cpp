#include "Bluepad32Controller.h"
#include <Arduino.h>

// Array to store connected controllers
ControllerPtr myControllers[MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not find empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d,
        ctl->index(), 
        ctl->dpad(), 
        ctl->buttons(), 
        ctl->axisX(), 
        ctl->axisY(),
        ctl->axisRX(), 
        ctl->axisRY(), 
        ctl->brake(), 
        ctl->throttle());
}

void dumpMouse(ControllerPtr ctl) {
    Serial.printf("idx=%d, buttons: 0x%04x, scrollWheel=0x%04x, delta X: %4d, delta Y: %4d\n",
                   ctl->index(), ctl->buttons(), ctl->scrollWheel(), ctl->deltaX(), ctl->deltaY());
}

void dumpKeyboard(ControllerPtr ctl) {
    static const char* key_names[] = {
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V",
        "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "Enter", "Escape", "Backspace", "Tab", "Spacebar"};

    Serial.printf("idx=%d, Pressed keys: ", ctl->index());
    for (int key = Keyboard_A; key <= Keyboard_UpArrow; key++) {
        if (ctl->isKeyPressed(static_cast<KeyboardKey>(key))) {
            Serial.printf("%s,", key_names[key-4]);
        }
    }
    Serial.println();
}

void dumpBalanceBoard(ControllerPtr ctl) {
    Serial.printf("idx=%d,  TL=%u, TR=%u, BL=%u, BR=%u, temperature=%d\n",
                   ctl->index(), ctl->topLeft(), ctl->topRight(), ctl->bottomLeft(), ctl->bottomRight(), ctl->temperature());
}

void processGamepad(ControllerPtr ctl) {
    if (ctl->a()) {
        static int colorIdx = 0;
        switch (colorIdx % 3) {
            case 0: ctl->setColorLED(255, 0, 0); break;
            case 1: ctl->setColorLED(0, 255, 0); break;
            case 2: ctl->setColorLED(0, 0, 255); break;
        }
        colorIdx++;
    }

    if (ctl->b()) {
        static int led = 0;
        led++;
        ctl->setPlayerLEDs(led & 0x0f);
    }

    if (ctl->x()) {
        ctl->playDualRumble(0, 250, 0x80, 0x40);
    }

    dumpGamepad(ctl);
}

void processMouse(ControllerPtr ctl) {
    if (ctl->scrollWheel() > 0) {
        // Do something
    } else if (ctl->scrollWheel() < 0) {
        // Do something else
    }

    dumpMouse(ctl);
}

void processKeyboard(ControllerPtr ctl) {
    if (!ctl->isAnyKeyPressed())
        return;

    if (ctl->isKeyPressed(Keyboard_A)) {
        Serial.println("Key 'A' pressed");
    }

    if (ctl->isKeyPressed(Keyboard_LeftShift)) {
        Serial.println("Key 'LEFT SHIFT' pressed");
    }

    if (ctl->isKeyPressed(Keyboard_LeftArrow)) {
        Serial.println("Key 'Left Arrow' pressed");
    }

    dumpKeyboard(ctl);
}

void processBalanceBoard(ControllerPtr ctl) {
    if (ctl->topLeft() > 10000) {
        // Do Something
    }

    dumpBalanceBoard(ctl);
}

void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            } else if (myController->isMouse()) {
                processMouse(myController);
            } else if (myController->isKeyboard()) {
                processKeyboard(myController);
            } else if (myController->isBalanceBoard()) {
                processBalanceBoard(myController);
            } else {
                Serial.println("Unsupported controller");
            }
        }
    }
}
