/*
 *    This file is part of Qtimer.

 *    QTimer is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.

 *    QTimer is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.

 *    You should have received a copy of the GNU General Public License
 *    along with Qtimer.  If not, see <https://www.gnu.org/licenses/>.
 */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Written by Zach Baker
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * This sketch demonstrated how to blink leds on pins using QTimer
 *
 * To demo this sketch, put leds with resitors on pins 10 - 13
 *
 * pin 10 will blink once for 2 seconds then turn off
 * pin 11 will blink 20 times for a forth of a second and then turn off
 * pin 12 will change state every half second as long as the arduino runs
 * pin 13 will change state every second second as long as the arduino runs
 *
 */

#include <QTimer.h>
#include <Events.h>
#include <EventDeque.h>

// Create timer
QTimer timer;

// create variable to store state of pin 13
bool toggle = false;

// define a toggle function to be called by the timer
void toggle13() {
  toggle = !toggle;
  digitalWrite(13, toggle);

  // This is how to blink an led using the pulse function
  // If given a pin number and starting value it will pulse the pin once for the period,
  // timer.pulse(byte pin, unsigned long period, bool startingValue);
  // This also demonstrated how you can create events from within a callback function
  timer.pulse(10, 100, HIGH);
}

void setup() {
  Serial.begin(9600);
  // set all pins to output
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);

  // This is an example of how to blink an led using a callback function
  // the function must return void and take no arguments
  // timer.every(unsigned long period, callbackFunction);
  timer.every(1000, toggle13);

  // This is how to blink an led using the oscillate function
  // If given a pin number and starting value this will toggle the led every period
  // timer.oscilate(byte pin, unsigned long period, bool startingValue);
  timer.oscillate(12, 500, HIGH);

  // This is how to blink an led using the oscillate function for a number of times
  // If given a pin number and starting value
  // timer.oscilate(byte pin, unsigned long period, bool startingValue, int repeatCount);
  timer.oscillate(11, 250, HIGH, 20);

}

void loop() {
  // update the timer every cycle
  timer.update();
}
