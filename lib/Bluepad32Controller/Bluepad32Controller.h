#ifndef BLUEPAD32CONTROLLER_H
#define BLUEPAD32CONTROLLER_H

#include <Bluepad32.h>

// Max number of gamepads
#define MAX_GAMEPADS BP32_MAX_GAMEPADS

// Array of controller pointers
extern ControllerPtr myControllers[MAX_GAMEPADS];

// Function declarations
void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);

void dumpGamepad(ControllerPtr ctl);
void dumpMouse(ControllerPtr ctl);
void dumpKeyboard(ControllerPtr ctl);
void dumpBalanceBoard(ControllerPtr ctl);

void processGamepad(ControllerPtr ctl);
void processMouse(ControllerPtr ctl);
void processKeyboard(ControllerPtr ctl);
void processBalanceBoard(ControllerPtr ctl);
void processControllers();

#endif // BLUEPAD32CONTROLLER_H
