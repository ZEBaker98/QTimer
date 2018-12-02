/*
 *    This file is part of QTimer.

 *    QTimer is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.

 *    QTimer is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.

 *    You should have received a copy of the GNU General Public License
 *    along with QTimer.  If not, see <https://www.gnu.org/licenses/>.
 */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Written by Zach Baker
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "Arduino.h"
#include "QTimer.h"
#include "EventDeque.h"
#include "Events.h"

// adds a new callback event to the deque and returns a pointer to it
CallbackEvent* QTimer::newCallbackEvent(uint32_t period, void (*callback)(), uint16_t repeatCount) {
  CallbackEvent *newEvent = new CallbackEvent(callback, millis(), period, repeatCount);
  ed.addEvent(newEvent);
  return newEvent;
}

// adds a new pin event to the deque and returns a pointer to it
PinEvent* QTimer::newPinEvent(uint32_t  period, uint8_t pin, uint8_t startingState, uint16_t toggleCount) {
  PinEvent *newEvent = new PinEvent(pin, startingState, millis(), period, toggleCount);
  ed.addEvent(newEvent);
  return newEvent;
}

// creates a one time event that will call the callback after duration
BaseEvent* QTimer::after(unsigned long duration, void (*callback)()) {
  return newCallbackEvent(duration, callback, 1);
}

// creates an event that calls the callback every period
BaseEvent* QTimer::every(unsigned long period, void (*callback)()) {
  return newCallbackEvent(period, callback, -1);
}

// creates an event that calls the callback every period repeatCount times
BaseEvent* QTimer::every(unsigned long period, void (*callback)(), int repeatCount) {
  return newCallbackEvent(period, callback, repeatCount);
}

// creates a pin event that oscilates
BaseEvent* QTimer::oscillate(uint8_t pin, unsigned long period, uint8_t startingState) {
  return newPinEvent(period, pin, startingState, -1);
}

// creates a pin event that oscilates
BaseEvent* QTimer::oscillate(uint8_t pin, unsigned long period, uint8_t startingState, uint16_t repeatCount) {
  return newPinEvent(period, pin, startingState, repeatCount * 2); // repeat count is doubled because each repeat requires toggling the pin twice
}

// cancels an event at a pointer
void QTimer::stop(BaseEvent *target) {
  target->repeatCount = 0;
}

// updates all Events in the deque at the current time
void QTimer::update() {
  update(millis());
}

// updates all Events in the deque at a given time
void QTimer::update(unsigned long now) {
  update(now, ed.getHead());
}

// updates all Events after target at a given time
void QTimer::update(unsigned long now, BaseEvent *target) {

  // if target is null, return
  if(target == nullptr) return;

  // if target should be triggered at current time, trigger it, otherwise update next event
  if(now - target->start >= target->period) {

    // if the target has remaining repeats and does not repeat forever (negative repeat value), call its callback
    if(target->repeatCount != 0) target->trigger();

    // if target has a next, update it
    if(target->next != nullptr) update(now, target->next);

    // after returning from updating next, decrease repeatCount for target by one
    if(target->repeatCount > 0) target->repeatCount--;

    // if no more repeats required remove the object from the deque, otherwise move its start to one cycle later
    if(target->repeatCount == 0) ed.deleteTarget(target);
    else target->start = target->start + target->period;

  } else {
    update(now, target->next);
  }
}

