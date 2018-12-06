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
#include "stdint.h"

#include "QTimer.h"
#include "EventDeque.h"
#include "Events.h"

// finds the lowest unused eventID
// searches using a modified counting sort to ensure ids are found within a reasonable amount of time
uint8_t QTimer::nextEventID() {
  uint32_t usedIDs[] = {0, 0, 0, 0, 0, 0, 0, 0}; // 256 bit memory space for storing used ids
  BaseEvent *index = ed.getHead();

  // go through all events in deque and mark their ids as used
  while(index != nullptr) {
    usedIDs[index->id / 32] |= (1 << (index->id % 32));
    index = index->next;
  }

  // find the first index greater than 0 in the used id memory space that is marked as unused
  for(uint8_t i = 1; i != 0; i++) {
    if (!(usedIDs[i / 32] & 1 << i % 32)) return i; // return available id
  }
  return 0; // no event ids available
}

// adds a new callback event to the deque and returns a pointer to it
CallbackEvent* QTimer::newCallbackEvent(uint32_t interval, void (*callback)(), uint16_t repeatCount) {
  uint8_t id = nextEventID(); // only create event if id is available
  if (id) {
    CallbackEvent *newEvent = new CallbackEvent(id, callback, millis(), interval, repeatCount);
    ed.addEvent(newEvent);
    return newEvent;
  } else {
    return nullptr;
  }
}

// adds a new pin event to the deque and returns a pointer to it
PinEvent* QTimer::newPinEvent(uint8_t pin, uint32_t  interval, uint8_t startingState, uint16_t toggleCount) {
  uint8_t id = nextEventID(); // only create event if id is available
  if (id) {
    PinEvent *newEvent = new PinEvent(id, pin, startingState, millis(), interval, toggleCount);
    ed.addEvent(newEvent);
    return newEvent;
  } else {
    return nullptr;
  }
}

// creates a one time event that will call the callback after duration
uint8_t QTimer::after(uint32_t duration, void (*callback)()) {
  CallbackEvent *newEvent = newCallbackEvent(duration, callback, 1);
  if (newEvent != nullptr) {
    return newEvent->id;
  } else {
    return 0;
  }
}

// creates an event that calls the callback every interval
uint8_t QTimer::every(uint32_t interval, void (*callback)()) {
  CallbackEvent *newEvent = newCallbackEvent(interval, callback, -1);
  if (newEvent != nullptr) {
    return newEvent->id;
  } else {
    return 0;
  }
}

// creates an event that calls the callback every interval repeatCount times
uint8_t QTimer::every(uint32_t interval, void (*callback)(), uint16_t repeatCount) {
  CallbackEvent *newEvent = newCallbackEvent(interval, callback, repeatCount);
  if (newEvent != nullptr) {
    return newEvent->id;
  } else {
    return 0;
  }
}

// creates a pin event that oscilates
uint8_t QTimer::oscillate(uint8_t pin, uint32_t interval, uint8_t startingState) {
  PinEvent *event = newPinEvent(pin, interval, startingState, -1);
  if (event != nullptr) {
    event->trigger();
    return event->id;
  } else {
    return 0;
  }
}

// creates a pin event that oscilates a number of times
uint8_t QTimer::oscillate(uint8_t pin, uint32_t interval, uint8_t startingState, uint16_t repeatCount) {
  PinEvent* event = newPinEvent(pin, interval, startingState, repeatCount*2 - 1);
  if (event != nullptr) {
    event->trigger();
    return event->id;
  } else {
    return 0;
  }
}

// creates a pulse of length interval
uint8_t QTimer::pulse(uint8_t pin, uint32_t interval, uint8_t startingState) {
  PinEvent* event = newPinEvent(pin, interval, startingState, 1);
  if (event != nullptr) {
    event->trigger();
    return event->id;
  } else {
    return 0;
  }
}

// marks an event with id of targetID as completed
void QTimer::stop(uint8_t targetID) {
  BaseEvent *index = ed.getHead();
  while(index != nullptr) {
    if(index->id == targetID) {
      index->repeatCount = 0;
      return;
    }
    index = index->next;
  }
}
// marks all events as completed
void QTimer::stopAll() {
  BaseEvent *index = ed.getHead();
  while(index != nullptr) {
    index->repeatCount = 0;
    index = index->next;
  }
}

// updates all Events in the deque at the current time
void QTimer::update() {
  this->now = millis();
  update(ed.getHead());
}

// updates all Events in the deque at a given time
void QTimer::update(uint32_t now) {
  this->now = now;
  update(ed.getHead());
}

// updates all Events after target at a given time
void QTimer::update(BaseEvent *target) {
  
  if(target == nullptr) return;

  // if target should be triggered at current time, trigger it, otherwise update next event
  if(now - target->start >= target->interval) {

    // if the target has remaining repeats and does not repeat forever (negative repeat value), call its callback
    if(target->repeatCount != 0) target->trigger();

    // if target has a next, update it
    if(target->next != nullptr) update(target->next);

    // after returning from updating next, decrease repeatCount for target by one
    if(target->repeatCount > 0) target->repeatCount--;

    // if no more repeats required remove the object from the deque, otherwise move its start to one cycle later
    if(target->repeatCount == 0) ed.deleteTarget(target);
    else target->start = target->start + target->interval;

  } else {
    update(target->next);
  }
}

