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
 * This sketch demonstrateds what happens when QTimer runs out of memory
 *
 * To demo this sketch, upload it to your board and watch the serial monitor
 *
 * When the sketch starts it will print out how much memory each object in the program uses
 * 
 * BaseEvent is a class that other events inherit from, it is not used directly
 * CallbackEvent is the class used for events that call functions
 * PinEvent it the class used for events that change pin states
 * EventDeque is a structure used by QTimer to store events
 * QTimer is the main timer object of this library, it contains one EventDeque
 * 
 * After printing out the memory uage of the objects the sketch will start creating events
 * None of the events will ever trigger, but they will take up memory space
 * Every second a new event is created, the timer is then updated
 * The time taken to update is measured and printed to the console
 * If the sketch runs out of memory, the program will restart or corrupt the output to the console
 * This lets you find out how many events can be created
 * My Arduino Uno can support about 60
 * Im Pretty sure an arduino Mega can run this sketch and create a full 255 events
 * 
 * When events are created they are placed on the heap and use exactly as many bytes as this sketch says
 * Events also use memroy on the call stack when the timer is updated
 * 4 bytes are needed for each event
 * Events are stored in the heap, the update information is store in the call stack
 * If these memory spaces colloide the program will crash
 * Learn more here: https://learn.adafruit.com/memories-of-an-arduino/arduino-memories
 *
 */

#include <QTimer.h>
#include <Events.h>
#include <EventDeque.h>

// Create a timer
QTimer timer;

// dummy callback used by events, does nothing
void dummyCallback() {};

int eventCount = 0;

// used to time how long updates take
unsigned long updateStart;
unsigned long updateDuration;

void printObjectSizes() {
  Serial.print("BaseEvent size: ");
  Serial.print(sizeof (BaseEvent));
  Serial.println(" bytes");
  
  Serial.print("CallbackEvent size: ");
  Serial.print(sizeof (CallbackEvent));
  Serial.println(" bytes");
  
  Serial.print("PinEvent size: ");
  Serial.print(sizeof (PinEvent));
  Serial.println(" bytes");
  
  Serial.print("EventDeque size: ");
  Serial.print(sizeof (EventDeque));
  Serial.println(" bytes");
  
  Serial.print("QTimer size: ");
  Serial.print(sizeof (QTimer));
  Serial.println(" bytes");
}

void setup() {
  Serial.begin(9600);

  Serial.println("Program restarting");

  printObjectSizes();
  delay(10000);
}

void loop() {
  // Add an event to the timer
  // creates an event if a slot is available
  // none of the test events created will trigger
  Serial.println("\nAdding Event...");
  if (timer.after(-1, dummyCallback)) { // -1 duration of event underflows to maximum duration
    eventCount++;
    Serial.print("There are now ");
    Serial.print(eventCount);
    Serial.println(" events.");
  } else {
    Serial.println("Maximum events reached");
  }

  // Update all events to test how long it takes
  Serial.println("Updating events... ");
  updateStart = micros();
  timer.update();
  updateDuration = micros() - updateStart;
  Serial.print("Update took ");
  Serial.print(updateDuration);
  Serial.println(" mircoseconds.");

  // Limit event creation to one per second
  delay(1000);
}
