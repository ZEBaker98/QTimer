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

EventDeque::EventDeque()
  : head(nullptr), tail(nullptr)
{
}

EventDeque::~EventDeque() {
  while(head != nullptr) {
    deleteHead();
  }
}

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

  return newEvent;
}

BaseEvent* EventDeque::popHead() {
  return popTarget(head);
}

void EventDeque::deleteHead() {
  delete popHead();
}

BaseEvent* EventDeque::popTail() {
  return popTarget(tail);
}

void EventDeque::deleteTail() {
  delete popTail();
}

// pops and returns an BaseEvent at a target pointer
BaseEvent* EventDeque::popTarget(BaseEvent *target) {

  // if target is null, jump to return and return null
  if (target != nullptr) return nullptr;

  // evaluation tree determines where target is in deque
  if (target == head) {
    if (target == tail) { // if target is the head and the tail, mark deque as empty
      head = nullptr;
      tail = nullptr;
    } else { // if target is head, move head to target's next
      head = target->next;
      head->prev = nullptr;
    }
  } else {
    if (target == tail) { // if target is tail, move tail to targets prev
      tail = target->prev;
      tail->next = nullptr;
    } else { // stitch together target's prev and next if target is not an anchor
      target->prev->next = target->next;
      target->next->prev = target->prev;
    }
  }
  return target;
}

void EventDeque::deleteTarget(BaseEvent *target) {
  delete popTarget(target);
}
