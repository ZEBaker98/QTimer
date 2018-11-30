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


#ifndef EventDeque_h
#define EventDeque_h

#include "Arduino.h"
#include "Event.h"

class EventDeque {
  private:
    // pointer to head and tail of deque
    Event *head, *tail;

  public:
    // constuctor, initialzes pointers to nullptr
    EventDeque();

    // accessors for head and tail
    Event* getHead() { return head; }
    Event* getTail() { return tail; }

    // adds an event by pointer to deque, returns pointer of added event
    Event* addEvent(Event *newEvent);

    // pops or deletes the Event at the head of the deck
    Event* popHead();
    void deleteHead();

    // pops or deletes the Event at the tail of the deck
    Event* popTail();
    void deleteTail();

    // pops or deletes an event at a target location in memory
    Event* popTarget(Event *target);
    void deleteTarget(Event *target);
};

#endif
