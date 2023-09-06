#include <Arduino.h>
#include "CoboldCore.hpp"


void setup() {
  // put your setup code here, to run once:

  cobold::app->preSetup();
  cobold::app->setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  cobold::app->loop();
}

