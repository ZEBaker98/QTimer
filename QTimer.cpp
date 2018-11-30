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
#include "Event.h"

// adds a new event to the deque and returns a pointer to it
Event* QTimer::newEvent(unsigned long period, void (*callback)(), int repeatCount) {
  Event *newEvent = new Event;
  newEvent->start = millis();
  newEvent->period = period;
  newEvent->repeatCount = repeatCount;
  newEvent->callback = callback;
  
  // sets prev and next pointers to null
  newEvent->prev = nullptr;
  newEvent->next = nullptr;

  // adds event to deque
  ed.addEvent(newEvent);

  // returns pointer
  return newEvent;
}

// creates a one time event that will call the callback after duration
Event* QTimer::after(unsigned long duration, void (*callback)()) {
  return newEvent(duration, callback, 1);
}

// creates an event that calls the callback every period
Event* QTimer::every(unsigned long period, void (*callback)()) {
  return newEvent(period, callback, -1);
}

// creates an event that calls the callback every period repeatCount times
Event* QTimer::every(unsigned long period, void (*callback)(), int repeatCount) {
  return newEvent(period, callback, repeatCount);
}

// cancels an event at a pointer
void QTimer::stop(Event *target) {
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
void QTimer::update(unsigned long now, Event *target) {

  // if target is null, return
  if(target == nullptr) return;

  // if target should be triggered at current time, trigger it, otherwise update next event
  if(now - target->start >= target->period) {

    // if the target has remaining repeats and does not repeat forever (negative repeat value), call its callback
    if(target->repeatCount != 0) (*target->callback)();

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

