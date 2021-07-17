#include <BlynkSimpleStream.h>
#include "HX711.h"
HX711 loadcell;
const long LOADCELL_OFFSET = 50682624;
const long LOADCELL_DIVIDER = 5895655;
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
float calibration_factor = -7050;
BlynkTimer Timer;
int pinState = 0;
float thrust = 100;
char auth[] = "Pqwj26J4lGEtDNkp-DaVCtawd3M9wI0W";
void blinkLedWidget()
{
  pinState = digitalRead(11);
  if (pinState == HIGH) 
  {
    digitalWrite(11, LOW);
  } 
  else 
  {
    digitalWrite(11, HIGH);
  }
}

void read_data()
{
  loadcell.set_scale(calibration_factor);
  thrust = loadcell.get_units();
  Blynk.virtualWrite(V3, thrust);
}

void ignitorLED()
{
  digitalWrite(12, HIGH);
  Timer.setInterval(2000L, read_data);
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  if(pinValue)
  {
   Timer.setTimer(200L, blinkLedWidget, 50);
   Timer.setTimeout(10100L, ignitorLED);
  }
}

void setup() 
{
 Serial.begin(9600);
 Blynk.begin(auth, Serial);
 pinMode(11, OUTPUT);
 pinMode(12, OUTPUT);
 loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); 
 loadcell.set_scale(LOADCELL_DIVIDER);
 loadcell.set_offset(LOADCELL_OFFSET);
 loadcell.set_scale();
 loadcell.tare();
}

void loop() 
{
 Blynk.run(); 
 Timer.run();
}
