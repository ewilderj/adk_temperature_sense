/*
  Sense temperature and light on the Google ADK, adjust LEDs to show
  what we're doing.
  
  Calculation of temperature with the MCP9701 referenced from
  http://www.instructables.com/id/Arduino-Wind-Chill-Machine/step8/Using-MCP9701-Temperature-sensor/ 
  
  Code requires the files from the Android ADK kit, see:
  http://developer.android.com/tools/adk/index.html
  

  Copyright (C) 2013 Edd Dumbill

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This program use some code that is Copyright (C) 2012 The Android Open Source Project,
  used under the terms of the Apache License, Version 2.0.
  
*/

#include <Max3421e.h>
#include <Usb.h>
#include <AndroidAccessory.h>


#define  LED1_RED       8
#define  LED1_GREEN     10
#define  LED1_BLUE      9

#define  LED3_RED       2
#define  LED3_GREEN     4
#define  LED3_BLUE      3

#define  LED2_RED       5
#define  LED2_GREEN     7
#define  LED2_BLUE      6

AndroidAccessory acc("Google, Inc.",
		     "DemoKit",
		     "DemoKit Arduino Board",
		     "1.0",
		     "http://www.android.com",
		     "0000000012345678");

 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
void led_on();
void init_leds();

// the setup routine runs once when you press reset:
void setup() { 
	Serial.begin(115200);
	Serial.print("\r\nStart");  
  init_leds();
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  acc.powerOn();
  analogReference(DEFAULT);
  led_off();
}

void init_leds()
{
	digitalWrite(LED1_RED, 1);
	digitalWrite(LED1_GREEN, 1);
	digitalWrite(LED1_BLUE, 1);

	pinMode(LED1_RED, OUTPUT);
	pinMode(LED1_GREEN, OUTPUT);
	pinMode(LED1_BLUE, OUTPUT);

	digitalWrite(LED2_RED, 1);
	digitalWrite(LED2_GREEN, 1);
	digitalWrite(LED2_BLUE, 1);

	pinMode(LED2_RED, OUTPUT);
	pinMode(LED2_GREEN, OUTPUT);
	pinMode(LED2_BLUE, OUTPUT);
}

void led_on()
{
  analogWrite(LED1_RED, 240);
  analogWrite(LED1_GREEN, 240);
  analogWrite(LED1_BLUE, 240);
  Serial.println("LED On");
}

void led_off()
{
  analogWrite(LED1_RED, 255);
  analogWrite(LED1_GREEN, 255);
  analogWrite(LED1_BLUE, 255);
  Serial.println("LED Off");
}

void led_temp(float t)
{
  float x = t;
  if (x < 0.0) { x = 0.0; }
  if (x > 30.0) { x = 30.0; }
  analogWrite(LED1_RED, 223+((30.0-x)*32)/30.0);
  analogWrite(LED1_BLUE, 231+(x*24.0)/30.0);
}

void breathe_delay()
{
    float bright = 0.0;
    for(int i = 0; i < 100; i++) {
      bright = sin(i*PI/100.0)*32.0;
      analogWrite(LED2_RED, 221+bright);
      analogWrite(LED2_GREEN, 221+bright);
      analogWrite(LED2_BLUE, 221+bright);
      delay(20);
    }
}

float temp = 0.0;
float mVout;
int val = 0;

void loop() {
  val = 0;
  for (byte j=0;j<10;j++)
  {
    val += analogRead(A3);
  }
  val=val/10;
  mVout=(float) val*5000.0/1023.0;
  temp=(mVout-490.0)/19.5;
  Serial.print("Temp: ");
  Serial.println(temp);
  led_temp(temp);
  val = analogRead(A2);
  Serial.print("Light: ");
  Serial.println(val);
  breathe_delay();
}
