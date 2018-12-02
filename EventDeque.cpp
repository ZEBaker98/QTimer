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

#include "EventDeque.h"
#include "Events.h"

// initialized head and tail to null
EventDeque::EventDeque() {
  head = nullptr;
  tail = nullptr;
}

// addes an event by pointer to deque
BaseEvent* EventDeque::addEvent(BaseEvent *newEvent) {

  // if head is null (Empty Deque), set newEvent as head and tail
  // else, add newEvent to current tail as next and set tail to newEvent
  if (head == nullptr) {
    head = newEvent;
    head->prev = nullptr;
    tail = newEvent;
    tail->next = nullptr;
  } else {
    tail->next = newEvent;
    newEvent->prev = tail;
    tail = newEvent;
  }

  // return pointer to newEvent
  return newEvent;
}

// pops and returns the BaseEvent at the head
BaseEvent* EventDeque::popHead() {
  return popTarget(head);
}

// deletes the BaseEvent at the head
void EventDeque::deleteHead() {
  delete popHead();
}

// pops and returns the BaseEvent at the tail
BaseEvent* EventDeque::popTail() {
  return popTarget(tail);
}

// deletes the BaseEvent at the tail
void EventDeque::deleteTail() {
  delete popTail();
}

// pops and returns an BaseEvent at a target pointer
BaseEvent* EventDeque::popTarget(BaseEvent *target) {

  // if target is null, jump to return and return null
  if (target != nullptr) {

    // evaluation tree determines if target is the head & tail, just head, just tail, or neither
    if (target == head) {
      if (target == tail) {
        // if target is the head and the tail, empty the deque and return
        head = nullptr;
        tail = nullptr;
      } else {
        // if target is just head, move head to targets next
        head = target->next;
        head->prev = nullptr;
      }
    } else {
      if (target == tail) {
        // if target is just tail, move tail to targets prev
        tail = target->prev;
        tail->next = nullptr;
      } else {
        // if target is in the middle of the deck, stitch together targets prev and next
        target->prev->next = target->next;
        target->next->prev = target->prev;
      }
    }
  }
  // return popped target
  return target;
}

// deletes an BaseEvent at a target pointer
void EventDeque::deleteTarget(BaseEvent *target) {
  delete popTarget(target);
}
