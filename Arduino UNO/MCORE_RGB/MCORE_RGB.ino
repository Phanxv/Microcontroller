#include "MeMCore.h"

MeRGBLed led(PORT_7);


void setup()
{

}

void loop()
{
  led.setColorAt(1,0, 100, 0);
  led.setColorAt(0,0, 0, 100);
  led.show();
}


